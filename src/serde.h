#pragma once

#include "defaultObjects.h"

namespace Serialization
{
    inline static constexpr std::uint32_t Version = 1;
	inline static constexpr std::uint32_t ID = 'PTPR';
	static_assert(Plugin::NAME == "TemplateProject"sv || ID != 'TRJT', "Make sure to use a unique ID for serialization.");

    /// @brief This is fucking stupid so it bears some explaining. Back when I first made the mod, I legit had no idea what
    /// I was doing. So, I wrote a maintenance function.... that didn't work. One thing led to another, and now I am doing
    /// BASIC papyrus stuff in C++ because when god made me, he broke the mold after he accidentally dropped it on my head.
    [[nodiscard]]
    inline void on_load(SKSE::SerializationInterface*) {
        auto* maintainance = DefaultObjects::ModObject<RE::TESQuest>("PTP_Framework_QST_MaintenanceQuest"sv);
        if (!maintainance) {
            return;
        }
        
        auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		auto* handlePolicy = vm ? vm->GetObjectHandlePolicy() : nullptr;
		if (!handlePolicy) {
			return;
		}

        RE::VMHandle handle = handlePolicy->GetHandleForObject(RE::TESQuest::FORMTYPE, maintainance);
		auto found = vm->attachedScripts.find(handle);
		if (found == vm->attachedScripts.end()) {
			return;
		}

        for (auto& script : found->second) {
			auto* info = script ? script->GetTypeInfo() : nullptr;
			if (!info || strcmp(info->GetName(), "ptp_questmaintenancescript") != 0) {
				continue;
			}

			auto callback = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>();
			auto args = RE::MakeFunctionArguments();
			const RE::BSFixedString functionName = "Maintenance";
			auto scriptObject = script.get();
			auto object = RE::BSTSmartPointer<RE::BSScript::Object>(scriptObject);
			vm->DispatchMethodCall(object, functionName, args, callback);
			break;
		}
    }

    inline void RegisterSerialization() {
        const auto serialization = SKSE::GetSerializationInterface();
        serialization->SetUniqueID(ID);
        serialization->SetLoadCallback(&on_load);
    }
}