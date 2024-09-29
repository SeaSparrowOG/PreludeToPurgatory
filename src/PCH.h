#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>

#include <ClibUtil/simpleINI.hpp>
#include <ClibUtil/singleton.hpp>
#include <ClibUtil/distribution.hpp>
#include <ClibUtil/editorID.hpp>
#include <ClibUtil/rng.hpp>

#include <json/json.h>

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace clib_util::singleton;

#ifdef SKYRIM_AE
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#else
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#endif

#include "Version.h"

#define _debugEDID clib_util::editorID::get_editorID
#define _loggerDebug SKSE::log::debug
#define _loggerInfo SKSE::log::info
#define _loggerError SKSE::log::error

namespace stl {
    template <class T>
    void write_thunk_call(std::uintptr_t a_src)
    {
        SKSE::AllocTrampoline(14);

        auto& trampoline = SKSE::GetTrampoline();
        T::func = trampoline.write_call<5>(a_src, T::thunk);
    }

    template <typename TDest, typename TSource>
    constexpr auto write_vfunc() noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ TDest::VTABLE[0] };
        TSource::func = vtbl.write_vfunc(TSource::idx, TSource::Thunk);
    }

    template <typename T>
    constexpr auto write_vfunc(const REL::ID variant_id) noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ variant_id };
        T::func = vtbl.write_vfunc(T::idx, T::Thunk);
    }
}

namespace Utils {
	template <typename T>
	T* ParseForm(const std::string& a_identifier) {
		if (const auto splitID = clib_util::string::split(a_identifier, "|"); splitID.size() == 2) {
			if (!clib_util::string::is_only_hex(splitID[1])) return nullptr;
			const auto  formID = clib_util::string::to_num<RE::FormID>(splitID[1], true);

			const auto& modName = splitID[0];
			if (!RE::TESDataHandler::GetSingleton()->LookupModByName(modName)) return nullptr;

			return RE::TESDataHandler::GetSingleton()->LookupForm<T>(formID, modName);
		}
		return RE::TESForm::LookupByEditorID<T>(a_identifier);
	}

	template <typename T, typename E>
	class EventClass : public RE::BSTEventSink<T> {
	public:
		static E* GetSingleton() {
			static E singleton;
			return std::addressof(singleton);
		}

		bool RegisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
			return true;
		}

		bool UnregisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->RemoveEventSink(this);
			return true;
		}

	protected:
		EventClass() = default;
		~EventClass() = default;

		EventClass(const EventClass&) = delete;
		EventClass(EventClass&&) = delete;
		EventClass& operator=(const EventClass&) = delete;
		EventClass& operator=(EventClass&&) = delete;
	};
}