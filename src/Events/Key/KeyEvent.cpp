#include "KeyEvent.h"

#include "Data/ModObjectManager.h"
#include "LichdomManager/Lichdom.h"

namespace Events::Key
{
	bool KeyPressManager::Register() {
		auto* source = RE::BSInputDeviceManager::GetSingleton();
		if (!source) {
			logger::info("  - Failed to retrieve the game's player control manager."sv);
			return false;
		}
		source->AddEventSink(this);
		return true;
	}

	RE::BSEventNotifyControl KeyPressManager::ProcessEvent(
		RE::InputEvent* const* a_event,
		RE::BSTEventSource<RE::InputEvent*>*)
	{
		static const auto* lichRace = Data::ModObject<RE::TESRace>(Data::LICH_RACE);
		if (!lichRace) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* buttonEvent = a_event && *a_event ? (*a_event)->AsButtonEvent() : nullptr;
		if (!buttonEvent || !buttonEvent->IsUp()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* player = RE::PlayerCharacter::GetSingleton();
		const auto* playerRace = player ? player->GetRace() : nullptr;
		if (playerRace != lichRace) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* events = RE::UserEvents::GetSingleton();
		const auto& userEvent = buttonEvent->userEvent;
		if (userEvent == events->tweenMenu) {
			LichdomManager::OpenSkillMenu();
		}
		else if (userEvent == events->favorites) {

		}

		return RE::BSEventNotifyControl::kContinue;
	}
}