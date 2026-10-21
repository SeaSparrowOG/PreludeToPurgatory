#include "ModObjects.hpp"

#include "Common/FormUtils.hpp"

namespace Data {
    bool ModObjectManager::PreLoad() {
        const auto* quest = RE::TESForm::LookupByEditorID<RE::TESQuest>(QuestName);
        if (!quest) {
            REX::CRITICAL("  - Failed to look up quest [{}]."sv, QuestName);
            return false;
        }

        const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm) {
			REX::CRITICAL("  - Failed to get the game's VM."sv);
			return false;
		}

		const auto bindPolicy = vm->GetObjectBindPolicy();
		const auto handlePolicy = vm->GetObjectHandlePolicy();
		if (!bindPolicy || !handlePolicy) {
			REX::CRITICAL("  - Failed to get the VM object policies"sv);
			return false;
		}

        size_t longest = 0u;
		const auto handle = handlePolicy->GetHandleForObject(RE::TESQuest::FORMTYPE, quest);
		RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable> properties;
		std::uint32_t nonConverted;
		bindPolicy->GetInitialPropertyValues(handle, ScriptName, properties, nonConverted);

        REX::INFO("  - Found {} variables."sv, properties.size());
        for (const auto& [name, var] : properties) {
            if (var.IsObjectArray()) {
                auto varArray = var.GetArray();
                for (RE::BSScript::Array::size_type i = 0u; i < varArray->size(); ++i) {
                    auto* form = (*varArray)[i].Unpack<RE::TESForm*>();
                    if (!form) {
                        REX::WARN("    >Element at position {} in array {} is NONE."sv, i, name.c_str());
                        continue;
                    }
                    objects.emplace(fmt::format("{}|{}", name.c_str(), i), form);
                    longest = name.size() > longest ? name.size() : longest;
                }
            }
            else if (var.IsObject()) {
                auto* form = var.Unpack<RE::TESForm*>();
                if (!form) {
                    REX::WARN("    >Property {} is NONE."sv, name.c_str());
                    continue;
                }
                objects.emplace(name.c_str(), form);
                    longest = name.size() > longest ? name.size() : longest;
            }
        }

        REX::INFO("  - Finished. Parsed {} properties."sv, objects.size());
        if (!objects.empty()) {
            for (const auto& [name, form] : objects) {
                REX::INFO("    >{}:{} {}", name.c_str(), std::string(longest - name.size(), ' '), FormUtils::GetFormName(form));
            }
        }
        return true;
    }
}