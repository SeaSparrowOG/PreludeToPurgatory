#pragma once

namespace Events
{
	namespace Key
	{
		class KeyPressManager :
			public REX::Singleton<KeyPressManager>,
			public RE::BSTEventSink<RE::InputEvent*>
		{
		public:
			[[nodiscard]] bool Register();

		private:
			RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event,
				RE::BSTEventSource<RE::InputEvent*>*) override;
		};

		[[nodiscard]] inline bool ReadyButtonListener() {
			auto* listener = KeyPressManager::GetSingleton();
			if (!listener) {
				logger::critical("  - Failed to retrieve internal Button listener."sv);
				return false;
			}
			return listener->Register();
		}
	}
}