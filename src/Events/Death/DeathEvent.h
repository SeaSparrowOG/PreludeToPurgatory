#pragma once

#include "Data/ModObjectManager.h"

namespace Events
{
	namespace Death
	{
		class DeathManager : 
			public REX::Singleton<DeathManager>,
			public RE::BSTEventSink<RE::TESDeathEvent>
		{
		public:
			[[nodiscard]] bool Register();

		private:
			virtual RE::BSEventNotifyControl ProcessEvent(
				const RE::TESDeathEvent* a_event,
				RE::BSTEventSource<RE::TESDeathEvent>*
			) override;
		};

		[[nodiscard]] inline bool ReadyDeathListener() {
			auto* listener = DeathManager::GetSingleton();
			if (!listener) {
				logger::critical("  - Failed to retrieve internal Death Listener."sv);
				return false;
			}
			return listener->Register();
		}
	}
}