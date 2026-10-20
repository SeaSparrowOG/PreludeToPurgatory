#pragma once

namespace SkillDecay
{
	constexpr auto PluginName = "SkillDecay";

	/// Available interface versions.
	enum class InterfaceVersion : uint8_t
	{
		kV1
	};

	/// <summary>
	/// Information passed to custom skill decay callback.
	/// </summary>
	struct CustomSkillDecayInfo
	{
		RE::TESGlobal* levelGlobal;      // Pointer to level global (already updated)
		RE::TESGlobal* xpGlobal;         // Pointer to XP global (already updated)
		float          previousLevel;    // Level before decay
		float          previousXP;       // XP before decay
		float          xpDecayed;        // Amount of XP that was decayed
		int            levelsLost;       // Number of full levels lost (if any)
	};

	/// <summary>
	/// Callback invoked after a custom skill decays.
	/// All globals have already been updated when this is called.
	/// </summary>
	/// <param name="info">Information about the decay that occurred</param>
	/// <param name="userData">User-provided context pointer from RegisterCustomSkill</param>
	using CustomSkillDecayCallback = void(*)(const CustomSkillDecayInfo* info, void* userData);

	class API
	{
	public:
		static inline API* instance;

		/// <summary>
		/// Checks whether the specified skill is currently decaying.
		/// </summary>
		/// <param name="avSkill">ActorValue of a skill to be checked. The ActorValue must point to a valid skill, otherwise false is returned.</param>
		/// <returns>Flag indicating whether the skill is currently decaying</returns>
		virtual bool IsDecaying(RE::ActorValue avSkill) noexcept = 0;

		/// <summary>
		/// Checks whether the specified skill is currently decaying.
		/// </summary>
		/// <param name="skill">Skill to be checked</param>
		/// <returns>Flag indicating whether the skill is currently decaying</returns>
		virtual bool IsDecaying(RE::PlayerCharacter::PlayerSkills::Data::Skill skill) noexcept = 0;

		/// <summary>
		/// Checks whether the specified custom skill is currently decaying.
		/// If skillId matches a default skill name (case-insensitive), returns decay status of that skill.
		/// </summary>
		/// <param name="skillId">Custom skill identifier or default skill name</param>
		/// <returns>Flag indicating whether the skill is currently decaying</returns>
		virtual bool IsDecaying(const char* skillId) noexcept = 0;

		/// <summary>
		/// Applies decay to a specified skill, reducing its XP and optionally its levels.
		/// </summary>
		/// <param name="avSkill">The skill to decay, specified as an ActorValue. The ActorValue must point to a valid skill.</param>
		/// <param name="decayXP">The amount of XP to subtract from the skill.</param>
		/// <param name="decayLevels">If true and decayXP is larger than the skill's current XP, skill level will be reduced and remaining decayXP subtracted.</param>
		virtual void DecaySkill(RE::ActorValue avSkill, float decayXP, bool decayLevels) noexcept = 0;

		/// <summary>
		/// Applies decay to a specified skill, reducing its XP and optionally its levels.
		/// </summary>
		/// <param name="skill">The skill to decay.</param>
		/// <param name="decayXP">The amount of XP to subtract from the skill.</param>
		/// <param name="decayLevels">If true and decayXP is larger than the skill's current XP, skill level will be reduced and remaining decayXP subtracted.</param>
		virtual void DecaySkill(RE::PlayerCharacter::PlayerSkills::Data::Skill skill, float decayXP, bool decayLevels) noexcept = 0;

		/// <summary>
		/// Applies decay to a specified custom skill, reducing its XP and optionally its levels.
		/// If skillId matches a default skill name (case-insensitive), decays that skill instead.
		/// </summary>
		/// <param name="skillId">Custom skill identifier or default skill name</param>
		/// <param name="decayXP">The amount of XP to subtract from the skill.</param>
		/// <param name="decayLevels">If true and decayXP is larger than the skill's current XP, skill level will be reduced and remaining decayXP subtracted.</param>
		virtual void DecaySkill(const char* skillId, float decayXP, bool decayLevels) noexcept = 0;

		/// <summary>
		/// Resets decaying state of a specified skill, preventing it from decaying until it is applied again.
		///
		/// If the skill is not currently decaying, this function does nothing.
		/// </summary>
		/// <param name="avSkill">The skill to reset, specified as an ActorValue. The ActorValue must point to a valid skill.</param>
		virtual void ResetDecay(RE::ActorValue avSkill) noexcept = 0;

		/// <summary>
		/// Resets decaying state of a specified skill, preventing it from decaying until it is applied again.
		///
		/// If the skill is not currently decaying, this function does nothing.
		/// </summary>
		/// <param name="skill">The skill to reset.</param>
		virtual void ResetDecay(RE::PlayerCharacter::PlayerSkills::Data::Skill skill) noexcept = 0;

		/// <summary>
		/// Resets decaying state of a specified custom skill, preventing it from decaying until it is applied again.
		/// If skillId matches a default skill name (case-insensitive), resets that skill instead.
		///
		/// If the skill is not currently decaying, this function does nothing.
		/// </summary>
		/// <param name="skillId">Custom skill identifier or default skill name</param>
		virtual void ResetDecay(const char* skillId) noexcept = 0;

		/// <summary>
		/// Registers a custom skill for decay tracking using TESGlobal variables.
		///
		/// IMPORTANT: Registration will FAIL if skillId matches any default skill name:
		/// OneHanded, TwoHanded, Archery, Block, Smithing, HeavyArmor, LightArmor,
		/// Pickpocket, Lockpicking, Sneak, Alchemy, Speech, Alteration, Conjuration,
		/// Destruction, Illusion, Restoration, Enchanting.
		/// 
		/// WerewolfPerks and VampirePerks are also reserved and cannot be used as skillIds.
		///
		/// BEHAVIOR:
		/// - SkillDecay READs from levelGlobal, xpGlobal, and legendaryGlobal to track usage
		/// - SkillDecay WRITEs to levelGlobal and xpGlobal when decay occurs
		/// - Skill improvement curve is derived from avi->skill->improveMult and avi->skill->improveOffset using default game formula.
		/// </summary>
		/// <param name="skillId">Unique identifier for the custom skill. Must not be null or empty. Must not match a default skill name.</param>
		/// <param name="av">ActorValue associated with the custom skill</param>
		/// <param name="avi">ActorValueInfo for the custom skill. Must not be nullptr and must have valid skill info (avi->skill != nullptr).</param>
		/// <param name="levelGlobal">TESGlobal storing current skill level. Must not be nullptr.</param>
		/// <param name="xpGlobal">TESGlobal storing current skill XP. Must not be nullptr.</param>
		/// <param name="xpNormalized">If true, xpGlobal contains 0-1 progress; if false, absolute XP.</param>
		/// <param name="legendaryGlobal">Optional TESGlobal for legendary level counter. Can be nullptr.</param>
		/// <param name="raceBonuses">Optional array of race FormID to skill bonus pairs. Can be nullptr.</param>
		/// <param name="raceBonusesCount">Number of entries in raceBonuses array. Ignored if raceBonuses is nullptr.</param>
		/// <returns>True if registration succeeded, otherwise false</returns>
		virtual bool RegisterCustomSkill(
			const char*                       skillId,
			RE::ActorValue					  av,
			RE::ActorValueInfo*               avi,
			RE::TESGlobal*                    levelGlobal,
			RE::TESGlobal*                    xpGlobal,
			bool                              xpNormalized,
			RE::TESGlobal*                    legendaryGlobal,
			const std::pair<RE::FormID, int>* raceBonuses,
			size_t                            raceBonusesCount) noexcept = 0;

		/// <summary>
		/// Unregisters a custom skill, stopping decay tracking.
		/// Safe to call even if skillId is not registered.
		/// Does nothing if skillId matches a default skill name.
		/// </summary>
		/// <param name="skillId">The skill identifier to unregister</param>
		virtual void UnregisterCustomSkill(const char* skillId) noexcept = 0;

		/// <summary>
		/// Checks if a custom skill is currently registered.
		/// Returns false if skillId matches a default skill name.
		/// </summary>
		/// <param name="skillId">The skill identifier to check</param>
		/// <returns>True if the custom skill is registered (not a default skill)</returns>
		virtual bool IsCustomSkillRegistered(const char* skillId) noexcept = 0;
	};

	inline void RegisterListener()
	{
		SKSE::GetMessagingInterface()->RegisterListener(
			"SkillDecay",
			[](auto msg)
			{
				if (auto api = reinterpret_cast<SkillDecay::API*>(msg->data)) {
					API::instance = api;
					REX::INFO("Registered SkillDecay.");
				}
			});
	}

	inline bool RegisterCustomSkill(
		const char* skillId,
		RE::ActorValue av,
		RE::ActorValueInfo* avi,
		RE::TESGlobal* levelGlobal,
		RE::TESGlobal* xpGlobal,
		bool xpNormalized,
		RE::TESGlobal* legendaryGlobal,
		const std::pair<RE::FormID, int>* raceBonuses,
		size_t raceBonusesCount)
	{
		if (API::instance) {
			return API::instance->RegisterCustomSkill(
				skillId,
				av,
				avi,
				levelGlobal,
				xpGlobal,
				xpNormalized,
				legendaryGlobal,
				raceBonuses,
				raceBonusesCount);
		}
		return false;
	}
}
