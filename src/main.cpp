#include <spdlog/sinks/basic_file_sink.h>

#include "defaultObjects.h"
#include "events.h"
#include "hooks.h"
#include "papyrus.h"
#include "serde.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_message) {
    if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
        REX::INFO("Registering new papyrus functions..."sv);
        SKSE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);
        REX::INFO("Registering soul trap listener..."sv);
        Events::SoulTrapListener::GetSingleton()->RegisterListener();
        REX::INFO("Registering actor death listener..."sv);
        Events::ActorDeathListener::GetSingleton()->RegisterListener();
        REX::INFO("Registering hit listener..."sv);
        Events::HitEventListener::GetSingleton()->RegisterListener();
        REX::INFO("Registering player input listener..."sv);
        Events::InputEventListener::GetSingleton()->RegisterListener();

        REX::INFO("Initializing mod default objects..."sv);
        if (!DefaultObjects::Storage::GetSingleton()->Initialize()) {
            _loggerError("Failed to get the default objects. Aborting load...");
            REX::FAIL("Failed to get the default objects for Prelude to Purgatory.");
        }
        REX::INFO("Startup tasks completed, enjoy your game!"sv);
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

#ifdef SKYRIM_GOG
	static constexpr std::array<REL::Version, 4> supported = 
	{
		SKSE::RUNTIME_SSE_1_6_1130,
		SKSE::RUNTIME_SSE_1_6_1170,
		SKSE::RUNTIME_SSE_1_6_1179,
		REL::Version(1, 6, 1179, 1) // no idea what this is still
	};
#else
	static constexpr std::array<REL::Version, 2> supported = 
	{
		SKSE::RUNTIME_SSE_1_7_104,
		SKSE::RUNTIME_SSE_1_7_99
	};	
#endif

    const auto ver = a_skse->RuntimeVersion();
    if ((ver < SKSE::RUNTIME_SSE_LATEST) && (!std::ranges::contains(supported, ver))) {
		REX::CRITICAL("Game Version: {}"sv, ver.string());
		REX::CRITICAL("Supported Versions:"sv);
		for (const auto& allowed : supported) {
			REX::CRITICAL("  - {}"sv, allowed.string());
		}
		REX::FAIL(
			fmt::format("You are using a version not supported by this plugin. Check the log at (Documents/My Games/Skyrim Special Edition/{}.log for more information."sv, Plugin::NAME)
		);
	}
    REX::INFO("Prelude running in Skyrim version: {}"sv, ver.string());
    REX::INFO("======================================================"sv);
    REX::INFO("Installing hooks..."sv);
    Hooks::Install();
    REX::INFO("Registering serialization..."sv);
    Serialization::RegisterSerialization();

    REX::INFO("Registering SKSE message callback..."sv);
    auto messaging = SKSE::GetMessagingInterface();
    messaging->RegisterListener(MessageHandler);

    REX::INFO("Registering Custom Skills Framework listener..."sv);
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