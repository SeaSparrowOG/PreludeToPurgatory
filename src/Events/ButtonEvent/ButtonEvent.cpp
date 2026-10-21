#include "ButtonEvent.hpp"

#include "APIStorage/APIStorage.hpp"
#include "Data/ModObjects.hpp"

namespace Events::ButtonEvent {
    bool InputEventListener::RegisterListener() noexcept
	{
		auto source = RE::BSInputDeviceManager::GetSingleton();
		if (!source)
		{
			REX::CRITICAL("Failed to get the game's UI singleton."sv);
			return false;
		}
		source->AddEventSink(this);
		return true;
	}

	RE::BSEventNotifyControl InputEventListener::ProcessEvent(RE::InputEvent* const* a_event, 
		RE::BSTEventSource<RE::InputEvent*>*)
	{
		static const auto* lichRace = Data::ModObject<RE::TESRace>("NecroLichRace"sv);

		if (!a_event || !*a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!lichRace || !player || player->GetRace() != lichRace) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto* ui = RE::UI::GetSingleton();
		if (!ui) {
			return RE::BSEventNotifyControl::kContinue;
		}

		static constexpr std::array<std::string_view, 34> menus = {
			RE::BarterMenu::MENU_NAME, RE::MapMenu::MENU_NAME, RE::Console::MENU_NAME,
			RE::MainMenu::MENU_NAME, RE::TweenMenu::MENU_NAME, RE::MagicMenu::MENU_NAME,
			RE::FaderMenu::MENU_NAME, RE::CursorMenu::MENU_NAME, RE::LoadingMenu::MENU_NAME,
			RE::LevelUpMenu::MENU_NAME, RE::JournalMenu::MENU_NAME, RE::CreditsMenu::MENU_NAME,
			RE::TrainingMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME, RE::DialogueMenu::MENU_NAME,
			RE::SafeZoneMenu::MENU_NAME, RE::FavoritesMenu::MENU_NAME, RE::SleepWaitMenu::MENU_NAME,
			RE::ModManagerMenu::MENU_NAME, RE::TutorialMenu::MENU_NAME, RE::ContainerMenu::MENU_NAME,
			RE::InventoryMenu::MENU_NAME, RE::LoadWaitSpinner::MENU_NAME, RE::StatsMenu::MENU_NAME,
			RE::CreationClubMenu::MENU_NAME, RE::RaceSexMenu::MENU_NAME, RE::TitleSequenceMenu::MENU_NAME,
			RE::ConsoleNativeUIMenu::MENU_NAME, RE::MessageBoxMenu::MENU_NAME, RE::GiftMenu::MENU_NAME,
			RE::MapMenu::MENU_NAME, RE::MistMenu::MENU_NAME, RE::BookMenu::MENU_NAME,
			RE::LockpickingMenu::MENU_NAME};

		const bool anyMenuOpen = 
			std::ranges::any_of(menus, [&](const std::string_view str) { 
				return ui->IsMenuOpen(str); 
			});

		if (anyMenuOpen) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* controlMap = RE::ControlMap::GetSingleton();
		const auto* userEvents = RE::UserEvents::GetSingleton();
		const RE::BSFixedString openTween = userEvents ? userEvents->tweenMenu : "";
		if (!controlMap || openTween.empty()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		for (const auto *event = *a_event; event; event = event->next) {
			const auto device = event->GetDevice();
			if (device != RE::INPUT_DEVICE::kKeyboard && device != RE::INPUT_DEVICE::kGamepad) {
				continue;
			}

			const auto *buttonEvent = event->AsButtonEvent();
			if (!buttonEvent || !(buttonEvent->IsUp() && buttonEvent->HasIDCode())) {
				continue; // unlikely.
			}

			const auto keyCode = buttonEvent->GetIDCode();
			const auto openTweenCode = controlMap->GetMappedKey(openTween, device);
			if (keyCode == openTweenCode) {
				SKSE::GetTaskInterface()->AddTask([](){
					auto* csf = APIStorage::Storage::GetSingleton()->customSkills;
					if (!csf) {
						return;
					}
					csf->ShowStatsMenu("Lichdom"); 
				});
				return RE::BSEventNotifyControl::kContinue;
			}
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}
