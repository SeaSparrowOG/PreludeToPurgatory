#include "hooks.h"

#include "RE/Offset.h"
#include "defaultObjects.h"
#include "xbyak/xbyak.h"

namespace Hooks {
	void Install()
	{
		SKSE::AllocTrampoline(28);
		HandleSpellCollisions::Install();
		CombatHit::Install();
	}

	void CombatHit::Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<std::uintptr_t> target{ REL::ID(38627), 0x4A8 };
		_originalCall = trampoline.write_call<5>(target.address(), &Hit);
	}

	void HandleSpellCollisions::Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<std::uintptr_t> target{ REL::ID(44204), 0x21F };
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

		if (shooter && baseSpell && baseProjectile && targetActor &&
			targetActor->HasPerk(magickaWeaveEffect) && 
			targetActor->GetRace() == lichRace &&
			targetActor->IsWeaponDrawn() &&
			baseSpell != retalliationSpell) {

			if (!magickaWeaveEffect || !retalliationSpell || !lichRace) {
				return _originalCall(a_this, a_target, arHitLocation, arHitNormal, aeCollisionLayer, auiMaterial, a7);
			}

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

	void CombatHit::HandleJoinTheMarch(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		_loggerDebug("Join the March:");
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* marchPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Necromancer_PRK_JoinTheMarch"sv);
		auto* marchSpell = DefaultObjects::ModObject<RE::SpellItem>("PTP_Necromancer_SPL_JoinTheMarchListener"sv);
		if (!lichRace || !marchPerk || !marchSpell) {
			_loggerError("  Handle Join the March Error: Failed to resolve default objects.");
			_loggerError("    March spell: {}", marchSpell ? _debugEDID(marchSpell) : "NULL");
			_loggerError("    March perk: {}", marchPerk ? _debugEDID(marchPerk) : "NULL");
			_loggerError("    Lich race: {}", lichRace ? _debugEDID(lichRace) : "NULL");
			return;
		}

		auto* zombie = a_hitData->aggressor.get().get();
		auto* commandingActor = zombie ? zombie->GetCommandingActor().get() : nullptr;
		if (!commandingActor) {
			_loggerDebug("  {} is not a commanded actor", zombie ? zombie->GetName() : "NULL");
			return;
		}
		if (!commandingActor->HasPerk(marchPerk) || commandingActor->GetRace() != lichRace) {
			_loggerDebug("  {} does not fit the criterial.", commandingActor->GetName());
			return;
		}

		auto* zombieMC = zombie->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!zombieMC || zombie->actorState1.lifeState != RE::ACTOR_LIFE_STATE::kReanimate) {
			_loggerDebug("  {} is not a valid zombie.", zombie->GetName());
		}

		_loggerDebug("  Faux onhit");
		zombieMC->CastSpellImmediate(marchSpell, false, a_this, 1.0f, false, 0.0f, zombie);
	}

	void CombatHit::HandleMagickaMitigation(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		_loggerDebug("Hardened Weave:");
		auto* redirectPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Entry_PRK_HardenedWeave"sv);
		auto* redirectDamage = DefaultObjects::ModObject<RE::SpellItem>("PTP_Entry_SPL_HardenedWeaveMagickaDamage"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* silverPerk = DefaultObjects::ModObject<RE::BGSKeyword>("WeapMaterialSilver");
		if (!redirectPerk || !lichRace || !silverPerk || !redirectDamage) {
			_loggerError("WARNING! Could not resolve default objects for Combat Hit");
			return;
		}
		if (!a_this->HasPerk(redirectPerk) || a_this->GetRace() != lichRace) {
			_loggerDebug("  Requirements not met");
			return;
		}
		if (a_hitData->weapon && a_hitData->weapon->HasKeyword(silverPerk)) {
			_loggerDebug("  Weapon is silver");
			return;
		}
		auto max = a_this->GetPermanentActorValue(RE::ActorValue::kMagicka);
		auto current = a_this->GetActorValue(RE::ActorValue::kMagicka);

		if (current < (max / 2.0f)) {
			_loggerDebug("  Magicka below 50%");
			return;
		}

		auto* lichMC = a_this->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!lichMC) {
			_loggerDebug("  No lich Magic Caster");
			return;
		}

		auto damage = a_hitData->totalDamage;
		if (damage < current) {
			auto* firstEffect = *redirectDamage->effects.begin();
			if (!firstEffect) {
				_loggerError("WARNING! First effect item not found for redirectDamage!");
				return;
			}

			firstEffect->effectItem.magnitude = damage;
			lichMC->CastSpellImmediate(redirectDamage, false, a_this, 1.0f, false, 0.0f, a_this);
			a_hitData->totalDamage = 0.0f;
			_loggerDebug("  Full mitigation");

		}
		else {
			auto* firstEffect = *redirectDamage->effects.begin();
			if (!firstEffect) {
				_loggerError("WARNING! First effect item not found for redirectDamage!");
				return;
			}

			firstEffect->effectItem.magnitude = damage - current;
			a_hitData->totalDamage = damage - current;
			lichMC->CastSpellImmediate(redirectDamage, false, a_this, 1.0f, false, 0.0f, a_this);
			_loggerDebug("  Partial mitigation");
		}
	}

	void CombatHit::HandleDamageDistribution(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		_loggerDebug("One Mind:");
		auto* redirectPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_Necromancer_PRK_OneBody"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* silverPerk = DefaultObjects::ModObject<RE::BGSKeyword>("WeapMaterialSilver");
		if (!redirectPerk || !lichRace || !silverPerk) {
			_loggerError("WARNING! Could not resolve default objects for Combat Hit");
			return;
		}
		if (!a_this->HasPerk(redirectPerk) || a_this->GetRace() != lichRace) {
			_loggerDebug("  Requirements not met");
			return;
		}
		if (a_hitData->weapon && a_hitData->weapon->HasKeyword(silverPerk)) {
			_loggerDebug("  Weapon is silver");
			return;
		}

		if (const auto middleHigh = a_this->GetMiddleHighProcess()) {
			auto it = middleHigh->commandedActors.begin();
			auto end = middleHigh->commandedActors.end();
			float remaining = a_hitData->totalDamage;

			for (; it != end && remaining > 0.0f; ++it) {
				const auto commandedActor = (*it).commandedActor.get().get();
				if (commandedActor->actorState1.lifeState == RE::ACTOR_LIFE_STATE::kReanimate) {
					_loggerDebug("  Valid minion");
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