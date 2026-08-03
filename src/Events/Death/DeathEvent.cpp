#include "DeathEvent.h"

namespace Events::Death
{
	bool DeathManager::Register() {
		auto* sink = RE::ScriptEventSourceHolder::GetSingleton();
		if (!sink) {
			logger::critical("  - Failed to retrieve the game's event source holder"sv);
			return false;
		}
		sink->AddEventSink(this);
		return true;
	}

	RE::BSEventNotifyControl DeathManager::ProcessEvent(
		const RE::TESDeathEvent* a_event,
		RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}
