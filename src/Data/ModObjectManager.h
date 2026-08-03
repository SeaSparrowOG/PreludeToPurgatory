#pragma once

namespace Data
{
	bool PreloadModObjects();

	class ModObjectManager :
		public REX::Singleton<ModObjectManager>
	{
	public:
		const std::string QuestName = fmt::format("{}_ModObjectsQuest", Plugin::NAME);
		const std::string ScriptName = fmt::format("{}_ModObjectsScript", Plugin::NAME);

		bool PreLoad();

		[[nodiscard]] RE::TESForm* Get(std::string_view a_key) const;
	private:
		bool Verify();

		util::istring_map<RE::TESForm*> objects;
	};

	template <typename T>
	[[nodiscard]] inline T* ModObject(std::string_view a_key)
	{
		if (const auto object = ModObjectManager::GetSingleton()->Get(a_key))
			return object->As<T>();
		return nullptr;
	}

	inline static constexpr std::size_t EXPECTED_MOD_OBJECT_COUNT = 1;

	inline static constexpr std::string_view LICH_RACE = "necroLichRace"sv;

	inline static constexpr std::array<std::string_view, EXPECTED_MOD_OBJECT_COUNT> EXPECTED_OBJECTS = {
		LICH_RACE
	};
}