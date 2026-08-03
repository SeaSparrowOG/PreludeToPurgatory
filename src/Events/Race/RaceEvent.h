#pragma once

namespace Events
{
	namespace Race
	{
		class RaceSwitchManager : 
			public REX::Singleton<RaceSwitchManager>,
			public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
		{
		public:
			[[nodiscard]] bool Register();

		private:
			virtual RE::BSEventNotifyControl ProcessEvent(
				const RE::TESSwitchRaceCompleteEvent* a_event, 
				RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*
			) override;


			void Revert();
			void Transform();

			bool wasLich = false;
		};

		[[nodiscard]] inline bool ReadyRaceSwitchListener() {
			auto* listener = RaceSwitchManager::GetSingleton();
			if (!listener) {
				logger::critical("  - Failed to retrieve internal Race Switch Listener."sv);
				return false;
			}
			return listener->Register();
		}
	}
}