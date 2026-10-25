#pragma once

namespace RE::Offset {
	// Credits for the 1.7 ID: ZeCroque (banger name)
	// https://github.com/ZeCroque
	namespace CombatUtilities
	{
#ifdef GOG
		inline constexpr REL::ID GetAngleToProjectedTarget(47286);
#else
		inline constexpr REL::ID GetAngleToProjectedTarget(523983);
#endif
	}
}

namespace RE
{
	// Workaround for commonlib. Combat::GetAngleToProjectedTarget has an invalid ID for 1.7
	static RE::NiPoint3 GetAngleToProjectedTarget(RE::NiPoint3 a_origin, 
		RE::TESObjectREFR* a_target, 
		float a_speed, 
		float a_gravity, 
		RE::ACTOR_LOS_LOCATION a_location)
	{
		using func_t = decltype(&GetAngleToProjectedTarget);
		static REL::Relocation<func_t> func{ Offset::CombatUtilities::GetAngleToProjectedTarget };
		return func(a_origin, a_target, a_speed, a_gravity, a_location);
	}
}