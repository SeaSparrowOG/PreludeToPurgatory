#pragma once

namespace Events {
	using EventResult = RE::BSEventNotifyControl;

	class SoulTrapListener :
		public REX::TSingleton<SoulTrapListener>,
		public RE::BSTEventSink<RE::SoulsTrapped::Event> {
	public:
		void RegisterListener();

	private:
		EventResult ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*) override;
	};

	class ActorDeathListener :
		public Utils::EventClass<RE::TESDeathEvent, ActorDeathListener> {
	private:
		EventResult ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override;
	};

	class HitEventListener :
		public Utils::EventClass<RE::TESHitEvent, HitEventListener> {
	private:
		EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override;
	};

	class InputEventListener :
		public REX::TSingleton<InputEventListener>,
		public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		[[nodiscard]]
		void RegisterListener() noexcept;
	private:
		virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
	};
}