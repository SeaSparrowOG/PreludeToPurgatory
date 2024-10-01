#pragma once

namespace RE::Offset {
	//Credits for the following 3:
	//Exit-9B (Parapets)
	//Github: https://github.com/Exit-9B/Constellations/blob/6cc31c1414e121bd30a6cb74975d35e5195b8675/src/RE/Offset.h#L76C3-L90C4
	namespace CombatUtilities
	{
		inline constexpr REL::ID GetAngleToProjectedTarget(static_cast<std::uint64_t>(47286));
	}
}

namespace RE
{
	class TESObjectREFR;

	enum class ACTOR_LOS_LOCATION
	{
		kOrigin = 0,
		kEye = 1,
		kHead = 2,
		kTorso = 3,
		kFeet = 4
	};

	struct CombatUtilities
	{
		static RE::NiPoint3 GetAngleToProjectedTarget(RE::NiPoint3 a_origin, RE::TESObjectREFR* a_target, float a_speed, float a_gravity, RE::ACTOR_LOS_LOCATION a_location)
		{
			using func_t = decltype(&GetAngleToProjectedTarget);
			REL::Relocation<func_t> func{ Offset::CombatUtilities::GetAngleToProjectedTarget };
			return func(a_origin, a_target, a_speed, a_gravity, a_location);
		}
	};
}