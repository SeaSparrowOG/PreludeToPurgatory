#pragma once

namespace Hooks {
	class HandleSpellCollisions {
	public:
		static void Install();
	private:
		static bool HandleCollision(RE::Projectile* a_this, RE::TESObjectREFR* a_target,
			const RE::NiPoint3* arHitLocation, const RE::NiPoint3* arHitNormal,
			RE::COL_LAYER aeCollisionLayer, unsigned int auiMaterial, BYTE* a7);

		static inline REL::Relocation<decltype(&HandleCollision)> _originalCall;
	};

	class CombatHit {
	public:
		static void Install();

	private:
		static void HandleJoinTheMarch(RE::Actor* a_this, RE::HitData* a_hitData);

		static void HandleMagickaMitigation(RE::Actor* a_this, RE::HitData* a_hitData);

		static void HandleDamageDistribution(RE::Actor* a_this, RE::HitData* a_hitData);

		static void Hit(RE::Actor* a_this, RE::HitData* a_hitData);

		static inline REL::Relocation<decltype(&Hit)> _originalCall;
	};

	void Install();
}