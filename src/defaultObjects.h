#pragma once

namespace DefaultObjects {
	class Storage : public ISingleton<Storage> {
	public:
		[[nodiscard]] RE::TESForm* Get(std::string_view a_key) const;
		bool Initialize();

	private:
		std::unordered_map<std::string_view, RE::TESForm*> objects;
		static constexpr auto QuestName = "PTP_Framework_QST_DefaultObjects"sv;
		static constexpr auto ScriptName = "PTP_Quest_DefaultObjectsScript"sv;
	};

	template <typename T>
	[[nodiscard]] inline T* ModObject(std::string_view a_key)
	{
		if (const auto object = Storage::GetSingleton()->Get(a_key))
			return object->As<T>();
		return nullptr;
	}
}