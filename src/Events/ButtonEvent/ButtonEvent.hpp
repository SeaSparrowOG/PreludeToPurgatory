#pragma once

namespace Events
{
    namespace ButtonEvent
    {
        class InputEventListener :
		    public REX::TSingleton<InputEventListener>,
		    public RE::BSTEventSink<RE::InputEvent*>
        {
        public:
            [[nodiscard]]
            bool RegisterListener() noexcept;
        private:
            virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
        };

        [[nodiscard]]
        inline bool Register() noexcept {
            auto* listener = InputEventListener::GetSingleton();
            if (!listener) {
                REX::CRITICAL("    >Failed to register for Input Events."sv);
                return false;
            }
            return listener->RegisterListener();
        }
    }
}