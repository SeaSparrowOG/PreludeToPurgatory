#pragma once

#include <ClibUtil/editorID.hpp>

namespace FormUtils
{
    template <typename T>
    struct form_type
    {
        static constexpr RE::FormType value = RE::FormType::None;
    };

    template <>
    struct form_type<RE::BGSKeyword> { static constexpr RE::FormType value = RE::FormType::Keyword; };
    template <>
    struct form_type<RE::BGSLocationRefType> { static constexpr RE::FormType value = RE::FormType::LocationRefType; };
    template<>
    struct form_type<RE::TESGlobal> { static constexpr RE::FormType value = RE::FormType::Global; };
    template<>
    struct form_type<RE::TESRace> { static constexpr RE::FormType value = RE::FormType::Race; };
    template<>
    struct form_type<RE::TESSound> { static constexpr RE::FormType value = RE::FormType::Sound; };
    template<>
    struct form_type<RE::TESObjectCELL> { static constexpr RE::FormType value = RE::FormType::Cell; };
    template<>
    struct form_type<RE::TESWorldSpace> { static constexpr RE::FormType value = RE::FormType::WorldSpace; };
    template<>
    struct form_type<RE::TESQuest> { static constexpr RE::FormType value = RE::FormType::Quest; };
    template<>
    struct form_type<RE::TESIdleForm> { static constexpr RE::FormType value = RE::FormType::Idle; };
    template<>
    struct form_type<RE::TESObjectANIO> { static constexpr RE::FormType value = RE::FormType::AnimatedObject; };
    template<>
    struct form_type<RE::TESImageSpaceModifier> { static constexpr RE::FormType value = RE::FormType::ImageAdapter; };
    template<>
    struct form_type<RE::BGSVoiceType> { static constexpr RE::FormType value = RE::FormType::VoiceType; };
    template<>
    struct form_type<RE::BGSMusicType> { static constexpr RE::FormType value = RE::FormType::MusicType; };
    template<>
    struct form_type<RE::BGSSoundDescriptorForm> { static constexpr RE::FormType value = RE::FormType::SoundRecord; };

    constexpr bool supports_edids_without_tweaks(RE::FormType type)
    {
        switch (type) {
        case RE::FormType::Keyword:
        case RE::FormType::LocationRefType:
        case RE::FormType::Global:
        case RE::FormType::Race:
        case RE::FormType::Sound:
        case RE::FormType::Cell:
        case RE::FormType::WorldSpace:
        case RE::FormType::Quest:
        case RE::FormType::Idle:
        case RE::FormType::AnimatedObject:
        case RE::FormType::ImageAdapter:
        case RE::FormType::VoiceType:
        case RE::FormType::MusicType:
        case RE::FormType::SoundRecord:
            return true;
        default:
            return false;
        }
    }

    template <typename T>
    [[nodiscard]] 
    inline std::string GetFormName(T* form)
    {
        assert(form);
        constexpr bool supportsEDID = supports_edids_without_tweaks(form_type<T>::value);
        static auto* tweaks = REX::W32::GetModuleHandleW(L"po3_Tweaks.dll");

        const auto formID = form->GetFormID();
        const bool runtimeForm = (formID & 0xFF000000) == 0xFF;
        const bool lightForm = (formID & 0xFE0000) == 0xFE;
        std::string definedIn{};
        if (runtimeForm) {
            definedIn = "Runtime Form";
        }
        else {
            const auto* parents = form->sourceFiles.array;
            if (parents) {
                const auto* start = parents->front();
                if (start) {
                    definedIn = start->GetFilename();
                }
            }
            else {
                definedIn = "Dynamic Form"; // Well not really, but idk what it is
            }
        }

        const RE::FormID localID = runtimeForm ? formID :
                                    lightForm ? (formID & 0xFFF) :
                                    (formID & 0xFFFFFF);
        std::string formattedSource = fmt::format("[{}|0x{:06X}]", definedIn, localID);
        if (tweaks) {
            return formattedSource + ": " + clib_util::editorID::get_editorID(form);
        }
        else if (supportsEDID) {
            return formattedSource + ": " + form->GetFormEditorID();
        }

        const char* name = form->GetName();
        if (strcmp(name, "") != 0) {
            return formattedSource + ": " + std::string(name);
        }
        return formattedSource;
    }
}