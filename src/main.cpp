#include <spdlog/sinks/basic_file_sink.h>

#include "defaultObjects.h"
#include "events.h"
#include "hooks.h"
#include "papyrus.h"
#include "serde.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_message) {
    if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
        SKSE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);
        Events::SoulTrapListener::GetSingleton()->RegisterListener();
        Events::ActorDeathListener::GetSingleton()->RegisterListener();
        Events::HitEventListener::GetSingleton()->RegisterListener();
        Events::InputEventListener::GetSingleton()->RegisterListener();

        if (!DefaultObjects::Storage::GetSingleton()->Initialize()) {
            _loggerError("Failed to get the default objects. Aborting load...");
            REX::FAIL("Failed to get the default objects for Prelude to Purgatory.");
        }
        Hooks::Install();
    }
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(Plugin::VERSION);
    v.PluginName(Plugin::NAME);
    v.AuthorName("SeaSparrow");
    v.UsesAddressLibrary();
    v.UsesUpdatedStructs();
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
    return v;
    }();

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse) {
    // alloc
    SKSE::InitInfo info{};
    info.hook = true;
    info.log = true;
    info.logLevel = REX::ELogLevel::Info;
    info.trampoline = true;
    info.trampolineSize = 280u; // temporary, FIX LATER
    SKSE::Init(a_skse, info);

    Serialization::RegisterSerialization();

    auto messaging = SKSE::GetMessagingInterface();
    messaging->RegisterListener(MessageHandler);

    SKSE::GetMessagingInterface()->RegisterListener(
        "CustomSkills",
        [](auto msg)
        {
            CustomSkills::QueryCustomSkillsInterface(msg, PluginAPIStorage::get().customSkills);
            const auto customSkills = PluginAPIStorage::get().customSkills;
            if (!customSkills)
                return;
        });
    return true;
}