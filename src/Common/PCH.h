#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>

#include <ClibUtil/simpleINI.hpp>
#undef ERROR

#include <ClibUtil/distribution.hpp>
#include <ClibUtil/editorID.hpp>
#include <ClibUtil/rng.hpp>

#include <json/json.h>

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;

#ifdef SKYRIM_AE
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#else
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#endif

#include "Plugin.h"

#define _debugEDID clib_util::editorID::get_editorID
#define _loggerDebug REX::DEBUG
#define _loggerInfo REX::INFO
#define _loggerError REX::ERROR

#include "CustomSkillsFramework/CustomSkills/Interfaces.h"

class PluginAPIStorage
{
public:
	static PluginAPIStorage &get()
	{
		static PluginAPIStorage singleton{};
		return singleton;
	}

	CustomSkills::CustomSkillsInterface *customSkills;
};

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
	class EventClass : 
		public RE::BSTEventSink<T>,
		public REX::TSingleton<E> 
	{
	public:
		bool RegisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
			return true;
		}

		bool UnregisterListener() {
			RE::ScriptEventSourceHolder::GetSingleton()->RemoveEventSink(this);
			return true;
		}
	};
}