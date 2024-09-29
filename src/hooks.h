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

	class HandleHealthDamage {
	public:
		static void Install();
	private:
		static void Damage(RE::Actor* a_this, RE::HitData* a_hitData);

		static inline REL::Relocation<decltype(&Damage)> _originalCall;
	};

	void Install();
}