#include "RaceEvent.h"

#include "Data/ModObjectManager.h"

namespace Events::Race
{
	bool RaceSwitchManager::Register() {
		auto* sink = RE::ScriptEventSourceHolder::GetSingleton();
		if (!sink) {
			logger::critical("  - Failed to retrieve the game's event source holder"sv);
			return false;
		}
		sink->AddEventSink(this);
		return true;
	}

	RE::BSEventNotifyControl RaceSwitchManager::ProcessEvent(
		const RE::TESSwitchRaceCompleteEvent* a_event, 
		RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*)
	{
		static const auto* lichRace = Data::ModObject<RE::TESRace>(Data::LICH_RACE);
		if (!a_event || !a_event->subject || !a_event->subject->IsPlayerRef()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto* player = a_event->subject->As<RE::Actor>();
		const auto* newRace = player ? player->GetRace() : nullptr;
		if (!newRace) {
			// impossible?
			return RE::BSEventNotifyControl::kContinue;
		}
		const bool isLich = newRace == lichRace;
		if (!wasLich && isLich) {
			Transform();
		}
		else if (wasLich && !isLich) {
			Revert();
		}

		wasLich = isLich;
		return RE::BSEventNotifyControl::kContinue;
	}

	void RaceSwitchManager::Revert() {
	}

	void RaceSwitchManager::Transform() {
	}
}