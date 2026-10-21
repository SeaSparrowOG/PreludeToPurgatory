#pragma once

namespace util
{
    [[nodiscard]] constexpr int ascii_tolower(int ch) noexcept
    {
        constexpr char diff = 'a' - 'A';
        if (ch >= 'A' && ch <= 'Z')
            ch += diff;
        return ch;
    }

    struct iless
    {
        using is_transparent = int;

        template <std::ranges::contiguous_range S1, std::ranges::contiguous_range S2>
            requires(
        std::is_same_v<std::ranges::range_value_t<S1>, char>&&
            std::is_same_v<std::ranges::range_value_t<S2>, char>)
            constexpr bool operator()(S1&& a_str1, S2&& a_str2) const
        {
            std::size_t count = std::ranges::size(a_str2);
            const std::size_t len1 = std::ranges::size(a_str1);
            const bool shorter = len1 < count;
            if (shorter)
                count = len1;

            if (count) {
                const char* p1 = std::ranges::data(a_str1);
                const char* p2 = std::ranges::data(a_str2);

                do {
                    const int ch1 = ascii_tolower(*p1++);
                    const int ch2 = ascii_tolower(*p2++);
                    if (ch1 != ch2)
                        return ch1 < ch2;
                } while (--count);
            }

            return shorter;
        }
    };

    template <class T>
    using istring_map = std::map<std::string, T, iless>;
}

namespace Data
{
	class ModObjectManager :
		public REX::TSingleton<ModObjectManager>
	{
	public:
		const std::string QuestName = "PTP_Framework_QST_DefaultObjects";
		const std::string ScriptName = "PTP_Quest_DefaultObjectsScript";

        [[nodiscard]]
		bool PreLoad();

        template <typename T>
		[[nodiscard]] 
        T* Get(std::string_view a_key) const {
            auto it = objects.find(a_key);
            if (it == objects.end()) {
                return nullptr;
            }
            return it->second->As<T>();
        }
	private:
		util::istring_map<RE::TESForm*> objects;
	};
    
    [[nodiscard]]
    inline bool PreloadModObjects() {
        REX::INFO("Preloading mod objects..."sv);
        auto* cache = ModObjectManager::GetSingleton();
        if (!cache) {
            REX::CRITICAL("  - Failed to look up internal Mod Object Manager."sv);
            return false;
        }
        return cache->PreLoad();
    }

	template <typename T>
	[[nodiscard]] inline T* ModObject(std::string_view a_key) {
        static const auto* manager = ModObjectManager::GetSingleton();
        return manager ? manager->Get<T>(a_key) : nullptr;
	}
}