#include "defaultObjects.h"

#include "RE/Offset.h"

namespace DefaultObjects {
	RE::TESForm* Storage::Get(std::string_view a_key) const
	{
		if (const auto it = objects.find(a_key); it != objects.end())
			return it->second;
		return nullptr;
	}

	bool Storage::Initialize()
	{
		const auto* quest = RE::TESForm::LookupByEditorID(QuestName);
		if (!quest) {
			return false;
		}

		const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm) {
			return false;
		}

		const auto bindPolicy = vm->GetObjectBindPolicy();
		const auto handlePolicy = vm->GetObjectHandlePolicy();

		if (!bindPolicy || !handlePolicy) {
			return false;
		}

		const auto handle = handlePolicy->GetHandleForObject(RE::TESQuest::FORMTYPE, quest);

		RE::BSTScrapHashMap<RE::BSFixedString, RE::BSScript::Variable> properties;
		std::uint32_t nonConverted;
		bindPolicy->GetInitialPropertyValues(handle, ScriptName, properties, nonConverted);

		objects.clear();

		for (const auto& [name, var] : properties) {
			if (const auto value = var.Unpack<RE::TESForm*>()) {
				objects.emplace(name, value);
			}
		}
		
		vm->ResetAllBoundObjects(handle);
		return true;
	}
}