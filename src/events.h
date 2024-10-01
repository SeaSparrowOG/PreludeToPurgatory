#pragma once

namespace Events {
	using EventResult = RE::BSEventNotifyControl;

	class SoulTrapListener :
		public ISingleton<SoulTrapListener>,
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
		public Utils::EventClass<RE::TESHitEvent, ActorDeathListener> {
	private:
		EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override;
	};
}