#include "events.h"

#include "defaultObjects.h"

namespace {
	void HandleWraithform(RE::Actor* a_deadActor, RE::Actor* a_killer) {
		_loggerDebug("  Wraithform check");
		auto* wraithformEffect = DefaultObjects::ModObject<RE::EffectSetting>("PTP_Powers_MGF_WraithformFFSelf"sv);
		if (!wraithformEffect) {
			_loggerError("WARNING! Could not resolve wraithform form in ActorDeathListener.");
			return;
		}
		if (!a_killer->HasMagicEffect(wraithformEffect)) {
			_loggerDebug("  Actor does not have wraithform effect {}", _debugEDID(wraithformEffect));
			return;
		}

		a_killer->ModActorValue(RE::ActorValue::kHealth, 50.0f);
		a_killer->ModActorValue(RE::ActorValue::kMagicka, 50.0f);
		a_killer->ModActorValue(RE::ActorValue::kStamina, 50.0f);
		//TODO: This doesn't actually work, possibly because of the ethereal archetype.
		/*
		auto* playerMC = a_killer->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!playerMC) return;

		_loggerDebug("  Casting {}", _debugEDID(healProc));
		playerMC->CastSpellImmediate(healProc, false, a_killer, 1.0f, false, 0.0f, a_killer);
		*/
	}
}
namespace Events {
	void SoulTrapListener::RegisterListener()
	{
		RE::SoulsTrapped::GetEventSource()->AddEventSink<RE::SoulsTrapped::Event>(this);
	}

	EventResult SoulTrapListener::ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*)
	{
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		if (!lichRace) {
			_loggerError("WARNING! Could not resolve Lich Race in ActorDeathListener.");
			return EventResult::kContinue;
		}

		if (a_event && a_event->target && a_event->trapper) {
			_loggerDebug("Soul trap: {} trapped {}", a_event->target->GetName(), a_event->trapper->GetName());
			auto* trapper = a_event->trapper;
			auto* trapped = a_event->target;
			if (trapper != RE::PlayerCharacter::GetSingleton()) {
				return EventResult::kContinue;
			}
			if (trapper->GetRace() != lichRace) return EventResult::kContinue;

			float fMultiplier = 0.0f;
			switch (trapped->GetSoulSize()) {
			case RE::SOUL_LEVEL::kPetty:
			case RE::SOUL_LEVEL::kLesser:
				fMultiplier = 0.5;
				break;
			case RE::SOUL_LEVEL::kCommon:
				fMultiplier = 0.75;
				break;
			case RE::SOUL_LEVEL::kGreater:
				fMultiplier = 1.00f;
				break;
			case RE::SOUL_LEVEL::kGrand:
				fMultiplier = 1.25;
				break;
			default:
				break;
			}

			_loggerDebug("  Actor is soul trapped.");
			auto* defaultQuest = DefaultObjects::ModObject<RE::TESQuest>("PTP_Framework_QST_MaintenanceQuest"sv);
			if (!defaultQuest) return EventResult::kContinue;

			_loggerDebug("  Querrying vm");
			auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			auto* handlePolicy = vm->GetObjectHandlePolicy();
			RE::VMHandle handle = handlePolicy->GetHandleForObject(defaultQuest->GetFormType(), defaultQuest);
			if (!handle || !vm->attachedScripts.contains(handle)) {
				_loggerDebug("  No handle");
				return EventResult::kContinue;
			}

			for (auto& foundScript : vm->attachedScripts.find(handle)->second) {
				_loggerDebug("  Found");
				if (foundScript->GetTypeInfo()->GetName() != "PTP_QuestMaintenanceScript"sv) continue;
				_loggerDebug("  Is valid");

				float fExperience = 0.5f * fMultiplier;
				auto callback = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
				auto args = RE::MakeFunctionArguments((float)fExperience);
				const RE::BSFixedString functionName = "CalculateExperience"sv;
				auto scriptObject = foundScript.get();
				auto object = RE::BSTSmartPointer<RE::BSScript::Object>(scriptObject);
				vm->DispatchMethodCall(object, functionName, args, callback);
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	EventResult ActorDeathListener::ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		if (!lichRace) {
			_loggerError("WARNING! Could not resolve Lich Race in ActorDeathListener.");
			return EventResult::kContinue;
		}

		if (a_event && a_event->actorDying.get() && a_event->actorKiller.get()) {
			_loggerDebug("Actor death: {} killed by {}", a_event->actorDying.get()->GetName(), a_event->actorKiller.get()->GetName());
			auto* dyingRef = a_event->actorDying.get();
			auto* dyingActor = dyingRef->As<RE::Actor>();
			auto* killerRef = a_event->actorKiller.get();
			auto* killerActor = killerRef ? killerRef->As<RE::Actor>() : nullptr;
			bool isActorDead = dyingRef->IsDead();
			if (!isActorDead || !dyingRef || !dyingActor || !killerActor) return EventResult::kContinue;
			if (dyingActor->IsCommandedActor()) return EventResult::kContinue;
			if (killerActor->GetRace() != lichRace) return EventResult::kContinue;

			HandleWraithform(dyingActor, killerActor);

			auto distanceFromPlayer = RE::PlayerCharacter::GetSingleton()->GetPosition().GetDistance(dyingActor->GetPosition());
			_loggerDebug("  Distance: {}", distanceFromPlayer);
			if (distanceFromPlayer > 1750.0f) return EventResult::kContinue;

			_loggerDebug("  Actor is dead dead.");
			auto* defaultQuest = DefaultObjects::ModObject<RE::TESQuest>("PTP_Framework_QST_MaintenanceQuest"sv);
			if (!defaultQuest) return EventResult::kContinue;

			_loggerDebug("  Querrying vm");
			auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			auto* handlePolicy = vm->GetObjectHandlePolicy();
			RE::VMHandle handle = handlePolicy->GetHandleForObject(defaultQuest->GetFormType(), defaultQuest);
			if (!handle || !vm->attachedScripts.contains(handle)) {
				_loggerDebug("  No handle");
				return EventResult::kContinue;
			}

			for (auto& foundScript : vm->attachedScripts.find(handle)->second) {
				_loggerDebug("  Found {}", foundScript->GetTypeInfo()->GetName());
				if (foundScript->GetTypeInfo()->GetName() != "ptp_questmaintenancescript"sv) continue;
				_loggerDebug("  Is valid");

				auto deadActorLevel = dyingActor->GetLevel();
				if (deadActorLevel < 10) {
					deadActorLevel = 0;
				}
				else if (deadActorLevel > 50) {
					deadActorLevel = 50;
				}
				float fExperience = deadActorLevel / 5.0f + 3.0f;
				fExperience *= 0.05f;
				auto callback = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
				auto args = RE::MakeFunctionArguments((float)fExperience); //WHY WON'T YOU COMPILE WITHOUT THE (float)?!?!?!
				const RE::BSFixedString functionName = "CalculateExperience"sv;
				auto scriptObject = foundScript.get();
				auto object = RE::BSTSmartPointer<RE::BSScript::Object>(scriptObject);
				vm->DispatchMethodCall(object, functionName, args, callback);
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	EventResult HitEventListener::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*)
	{
		_loggerDebug("Hit event");
		if (!a_event) return EventResult::kContinue;

		auto* frozenCadaverPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Necromancer_PRK_FrozenCadavers"sv);
		auto* frozenCadaverProc = DefaultObjects::ModObject<RE::SpellItem>("PTP_Necromancer_SPL_FrozenCadaverIceHIt"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		if (!lichRace || !frozenCadaverPerk || !frozenCadaverProc) {
			_loggerError("Could not resolve default objects for Hit Event.");
			_loggerError("  >Lich Race: {}", lichRace ? _debugEDID(lichRace) : "NULL");
			_loggerError("  >Cadaver Perk: {}", frozenCadaverPerk ? _debugEDID(frozenCadaverPerk) : "NULL");
			_loggerError("  >Cadaver Proc: {}", frozenCadaverProc ? _debugEDID(frozenCadaverProc) : "NULL");
			return EventResult::kContinue;
		}

		auto* eventReference = a_event->cause.get();
		auto* eventAggressor = eventReference ? eventReference->As<RE::Actor>() : nullptr;
		auto* hitReference = a_event->target.get();
		auto* eventVictim = hitReference ? hitReference->As<RE::Actor>() : nullptr;
		auto* eventSpell = RE::TESForm::LookupByID(a_event->source) ? RE::TESForm::LookupByID(a_event->source)->As<RE::SpellItem>() : nullptr;

		if (!eventSpell || !eventVictim || !eventAggressor) {
			_loggerDebug("  No suitable spell, victim, or aggressor");
			return EventResult::kContinue;
		}
		if (eventVictim->HasSpell(frozenCadaverProc)) {
			_loggerDebug("  {} already has the proc spell.", eventVictim->GetName());
			return EventResult::kContinue;
		}

		bool isFrostSpell = false;
		for (auto it = eventSpell->effects.begin(); it != eventSpell->effects.end() && !isFrostSpell; ++it) {
			auto* baseEffect = (*it)->baseEffect;
			if (!baseEffect) continue;

			if (baseEffect->HasKeywordString("MagicDamageFrost"sv)) {
				isFrostSpell = true;
			}
		}
		if (!isFrostSpell) {
			_loggerDebug("  Spell is not a frost spell");
			return EventResult::kContinue;
		}

		if (!eventVictim->IsCommandedActor() || eventVictim->actorState1.lifeState != RE::ACTOR_LIFE_STATE::kReanimate) {
			_loggerDebug("  Victim {} is not a commanded zombie", eventVictim->GetName());
			return EventResult::kContinue;
		}
		if (eventAggressor->GetRace() != lichRace || !eventAggressor->HasPerk(frozenCadaverPerk) || eventAggressor != eventVictim->GetCommandingActor().get()) {
			_loggerDebug("  {} does not fulfill the criteria", eventAggressor->GetName());
			return EventResult::kContinue;
		}

		auto* zombieMC = eventVictim->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!zombieMC) return EventResult::kContinue;

		zombieMC->CastSpellImmediate(frozenCadaverProc, false, eventVictim, 1.0f, false, 0.0f, eventVictim);
		return EventResult::kContinue;
	}
}