#include "hooks.h"

#include "RE/Offset.h"
#include "defaultObjects.h"
#include "xbyak/xbyak.h"

namespace Hooks {
	void Install()
	{
		SKSE::AllocTrampoline(42);
		ActiveEffectApply::Install();
		HandleSpellCollisions::Install();
		HandleHealthDamage::Install();
	}

	void HandleSpellCollisions::Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<std::uintptr_t> target{ REL::ID(44204), 0x21F };
		_originalCall = trampoline.write_call<5>(target.address(), &HandleCollision);
	}

	bool HandleSpellCollisions::HandleCollision(RE::Projectile* a_this, RE::TESObjectREFR* a_target, const RE::NiPoint3* arHitLocation, const RE::NiPoint3* arHitNormal, RE::COL_LAYER aeCollisionLayer, unsigned int auiMaterial, BYTE* a7)
	{
		auto* shooter = a_this->shooter.get().get();
		auto* baseProjectile = a_this->GetProjectileBase();
		auto* baseMagic = a_this->spell;
		auto* baseSpell = baseMagic ? baseMagic->As<RE::SpellItem>() : nullptr;
		auto* targetActor = a_target->As<RE::Actor>();
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

	void HandleHealthDamage::Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<std::uintptr_t> target{ REL::ID(38627), 0x4A8 };
		_originalCall = trampoline.write_call<5>(target.address(), &Damage);
	}

	void HandleHealthDamage::Damage(RE::Actor* a_this, RE::HitData* a_hitData)
	{
		auto* redirectPerk = DefaultObjects::ModObject<RE::BGSPerk>("PTP_SpiritCaller_WailsOfTheDamned"sv);
		auto* lichRace = DefaultObjects::ModObject<RE::TESRace>("NecroLichRace"sv);
		auto* silverPerk = DefaultObjects::ModObject<RE::BGSKeyword>("WeapMaterialSilver");
		if (!redirectPerk || !lichRace || !silverPerk) {
			return _originalCall(a_this, a_hitData);
		}
		if (!a_this->HasPerk(redirectPerk) || a_this->GetRace() != lichRace) {
			return _originalCall(a_this, a_hitData);
		}
		if (a_hitData->weapon && a_hitData->weapon->HasKeyword(silverPerk)) {
			return _originalCall(a_this, a_hitData);
		}

		if (const auto middleHigh = a_this->GetMiddleHighProcess()) {
			auto it = middleHigh->commandedActors.begin();
			auto end = middleHigh->commandedActors.end();
			float remaining = a_hitData->physicalDamage;

			for (; it != end && remaining > 0.0f; ++it) {
				const auto commandedActor = (*it).commandedActor.get().get();
				if (commandedActor->CalculateCachedOwnerIsUndead()) {
					auto health = commandedActor->GetActorValue(RE::ActorValue::kHealth);
					if (health >= remaining) {
						remaining -= health;
						_originalCall(commandedActor, a_hitData);
						a_hitData->physicalDamage = 0.0f;
					}
					else {
						remaining -= health;
						_originalCall(commandedActor, a_hitData);
						a_hitData->physicalDamage = remaining;
					}
				}
			}
		}
		_originalCall(a_this, a_hitData);
	}

	void ActiveEffectApply::Install()
	{
		REL::Relocation<std::uintptr_t> vtbl{ RE::ReanimateEffect::VTABLE[0]};
		_originalCall = vtbl.write_vfunc(0x4, OnAdd);
	}

	void ActiveEffectApply::OnAdd(RE::ReanimateEffect* a_this, float a_delta)
	{
		_originalCall(a_this, a_delta);
	}
}