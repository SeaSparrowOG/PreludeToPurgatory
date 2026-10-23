#include "hooks.h"

#include "RE/Offset.h"
#include "defaultObjects.h"
#include "xbyak/xbyak.h"

namespace Hooks {
	void Install()
	{
		HandleSpellCollisions::Install();
		CombatHit::Install();
	}

	void CombatHit::Install()
	{
		auto& trampoline = REL::GetTrampoline();
		REL::Relocation<std::uintptr_t> target{ REL::ID(38627), 0x4A8 };
		_originalCall = trampoline.write_call<5>(target.address(), &Hit);
	}

	void HandleSpellCollisions::Install()
	{
		auto& trampoline = REL::GetTrampoline();
#ifdef GOG // seems like something above was inlined?
		REL::Relocation<std::uintptr_t> target{ REL::ID(44204), 0x21F };
#else
		REL::Relocation<std::uintptr_t> target{ REL::ID(44204), 0x239 };
#endif
		_originalCall = trampoline.write_call<5>(target.address(), &HandleCollision);
	}

	bool HandleSpellCollisions::HandleCollision(RE::Projectile* a_this, RE::TESObjectREFR* a_target, const RE::NiPoint3* arHitLocation, const RE::NiPoint3* arHitNormal, RE::COL_LAYER aeCollisionLayer, unsigned int auiMaterial, BYTE* a7)
	{
		auto* shooter = a_this ? a_this->shooter.get().get() : nullptr;
		auto* baseProjectile = a_this ? a_this->GetProjectileBase() : nullptr;
		auto* baseMagic = a_this ? a_this->spell : nullptr;
		auto* baseSpell = baseMagic ? baseMagic->As<RE::SpellItem>() : nullptr;
		auto* targetActor = a_target ? a_target->As<RE::Actor>() : nullptr;
		auto* magickaWeaveEffect = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Entry_PRK_MirrorWeave"sv);
		auto* retalliationSpell = DefaultObjects::ModObject<RE::SpellItem>("PTP_Entry_SPL_MagickaWeaveRetaliation"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);

		if (!magickaWeaveEffect || !retalliationSpell || !lichRace) {
			return _originalCall(a_this, a_target, arHitLocation, arHitNormal, aeCollisionLayer, auiMaterial, a7);
		}
		if (shooter && baseSpell && baseProjectile && targetActor &&
			targetActor->HasPerk(magickaWeaveEffect) && 
			targetActor->GetRace() == lichRace &&
			baseSpell != retalliationSpell) {

			auto magicka = RE::ActorValue::kMagicka;

			auto currentMagicka = targetActor->GetActorValue(magicka);
			auto maxMagicka = targetActor->GetPermanentActorValue(magicka);
			if (maxMagicka <= 0.0f || currentMagicka < maxMagicka / 2.0f) {
				return _originalCall(a_this, a_target, arHitLocation, arHitNormal, aeCollisionLayer, auiMaterial, a7);
			}

			auto magickaPct = currentMagicka / maxMagicka;
			bool canReflect = magickaPct > clib_util::RNG().generate<float>(0.0f, 0.9f);
			if (!canReflect) {
				return _originalCall(a_this, a_target, arHitLocation, arHitNormal, aeCollisionLayer, auiMaterial, a7);
			}

			const auto angle = RE::CombatUtilities::GetAngleToProjectedTarget(
				a_this->GetPosition(),
				shooter,
				baseProjectile->data.speed,
				baseProjectile->data.gravity,
				RE::ACTOR_LOS_LOCATION::kTorso);

			RE::ProjectileHandle newHandle{};
			RE::Projectile::ProjectileRot rotation{};
			rotation.x = angle.x;
			rotation.z = angle.z;
			RE::Projectile::LaunchSpell(&newHandle, targetActor, retalliationSpell, a_this->GetPosition(), rotation);
		}
		return _originalCall(a_this, a_target, arHitLocation, arHitNormal, aeCollisionLayer, auiMaterial, a7);
	}

	void CombatHit::HandleJoinTheMarch(RE::Actor* a_this, RE::HitData* a_hitData) {
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* marchPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Necromancer_PRK_JoinTheMarch"sv);
		auto* marchSpell = DefaultObjects::ModObject<RE::SpellItem>("PTP_Necromancer_SPL_JoinTheMarchListener"sv);
		if (!lichRace || !marchPerk || !marchSpell) {
			return;
		}

		auto* zombie = a_hitData->aggressor.get().get();
		auto* commandingActor = zombie ? zombie->GetCommandingActor().get() : nullptr;
		if (!commandingActor) {
			return;
		}
		if (!commandingActor->HasPerk(marchPerk) || commandingActor->GetRace() != lichRace) {
			return;
		}
		
		auto* dobj = RE::BGSDefaultObjectManager::GetSingleton();
		auto* undeadKeyword = dobj ? dobj->GetObject<RE::BGSKeyword>(RE::DEFAULT_OBJECTS::kKeywordUndead) : nullptr;
		const bool isVictimUndeadMinion = zombie->IsCommandedActor() &&
			zombie->IsReanimated() || (undeadKeyword && zombie->HasKeyword(undeadKeyword));

		if (!isVictimUndeadMinion) {
			return;
		}

		auto* zombieMC = zombie->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!zombieMC) {
			return;
		}
		zombieMC->CastSpellImmediate(marchSpell, false, a_this, 1.0f, false, 0.0f, zombie);
	}

	void CombatHit::HandleMagickaMitigation(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		auto* redirectPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Entry_PRK_HardenedWeave"sv);
		auto* redirectDamage = DefaultObjects::ModObject<RE::SpellItem>("PTP_Entry_SPL_HardenedWeaveMagickaDamage"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* silverPerk = DefaultObjects::ModObject<RE::BGSKeyword>("WeapMaterialSilver");
		if (!redirectPerk || !lichRace || !silverPerk || !redirectDamage) {
			return;
		}
		if (!a_this->HasPerk(redirectPerk) || a_this->GetRace() != lichRace) {
			return;
		}
		if (a_hitData->weapon && a_hitData->weapon->HasKeyword(silverPerk)) {
			return;
		}
		auto max = a_this->GetPermanentActorValue(RE::ActorValue::kMagicka) + a_this->magickaModifiers.modifiers[RE::ACTOR_VALUE_MODIFIERS::kTemporary];
		auto current = a_this->GetActorValue(RE::ActorValue::kMagicka);

		if (current < (max / 2.0f)) {
			return;
		}

		auto* lichMC = a_this->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!lichMC) {
			return;
		}

		auto damage = a_hitData->totalDamage;
		if (damage < current) {
			auto* firstEffect = *redirectDamage->effects.begin();
			if (!firstEffect) {
				return;
			}

			firstEffect->effectItem.magnitude = damage;
			lichMC->CastSpellImmediate(redirectDamage, false, a_this, 1.0f, false, 0.0f, a_this);
			a_hitData->totalDamage = 0.0f;

		}
		else {
			auto* firstEffect = *redirectDamage->effects.begin();
			if (!firstEffect) {
				return;
			}

			firstEffect->effectItem.magnitude = damage - current;
			a_hitData->totalDamage = damage - current;
			lichMC->CastSpellImmediate(redirectDamage, false, a_this, 1.0f, false, 0.0f, a_this);
		}
	}

	void CombatHit::HandleDamageDistribution(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		auto* redirectPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Necromancer_PRK_OneBody"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* silverPerk = DefaultObjects::ModObject<RE::BGSKeyword>("WeapMaterialSilver");
		auto* frozenCadaverMagicEffect = DefaultObjects::ModObject<RE::EffectSetting>("PTP_Necromancer_FrozenCadaverArmorProc");
		if (!redirectPerk || !lichRace || !silverPerk || !frozenCadaverMagicEffect) {
			return;
		}
		if (!a_this->HasPerk(redirectPerk) || a_this->GetRace() != lichRace) {
			return;
		}
		if (a_hitData->weapon && a_hitData->weapon->HasKeyword(silverPerk)) {
			return;
		}

		if (const auto middleHigh = a_this->GetMiddleHighProcess()) {
			auto it = middleHigh->commandedActors.begin();
			auto end = middleHigh->commandedActors.end();
			float remaining = a_hitData->totalDamage;

			for (; it != end && remaining > 0.0f; ++it) {
				const auto commandedActor = (*it).commandedActor.get().get();
				if (commandedActor->HasMagicEffect(frozenCadaverMagicEffect)) {
					auto health = commandedActor->GetActorValue(RE::ActorValue::kHealth);
					if (health >= remaining) {
						remaining -= health;
						_originalCall(commandedActor, a_hitData);
						a_hitData->totalDamage = 0.0f;
					}
					else {
						remaining -= health;
						_originalCall(commandedActor, a_hitData);
						a_hitData->totalDamage = remaining;
					}
				}
			}
		}
	}

	void CombatHit::Hit(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		HandleDamageDistribution(a_this, a_hitData);
		HandleMagickaMitigation(a_this, a_hitData);
		_originalCall(a_this, a_hitData);
		HandleJoinTheMarch(a_this, a_hitData);
	}
}