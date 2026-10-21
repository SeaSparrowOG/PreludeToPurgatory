#include "APIStorage/APIStorage.hpp"
#include "Data/ModObjects.hpp"

[[nodiscard]]
std::string FormatFailureMessage(const std::string_view reason) {
    return fmt::format("{}."
                       "Check the log at Documents/My Games/Skyrim Special Edition/SKSE/{}.log for more information."sv, 
                       reason, Plugin::NAME
    );
}

// IDENTICAL to commonlib. I need it to fire BEFORE I init SKSE.
static void SetupLog() {
#ifndef NDEBUG
	#define SPD_LOG_LEVEL spdlog::level::debug
#else
	#define SPD_LOG_LEVEL spdlog::level::info
#endif

	auto path = SKSE::log::log_directory();
	if (!path)
		return;

	*path /= std::format("{}.log", Plugin::NAME);

	std::vector<spdlog::sink_ptr> sinks{
		std::make_shared<spdlog::sinks::msvc_sink_mt>()
	};
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));

	auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
	logger->set_level(SPD_LOG_LEVEL);
	logger->flush_on(SPD_LOG_LEVEL);

	spdlog::set_default_logger(std::move(logger));
	spdlog::set_pattern("[%T.%e] [%=5t] [%L] %v");
}

static void MessageEventCallback(SKSE::MessagingInterface::Message* msg) {
    switch (msg->type) {
    case SKSE::MessagingInterface::kDataLoaded:
		if (!APIStorage::GetAVGInterface()) {
			REX::FAIL(FormatFailureMessage("Failed to request necessary interfaces."sv));
		}
        if (!Data::PreloadModObjects()) {
            REX::FAIL(FormatFailureMessage("Failed to preload mod objects"sv));
        }
        break;
    default:

        break;
    }
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v{};
	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.AuthorName("SeaSparrow"sv);
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	return v;
}();

SKSE_PLUGIN_QUERY(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info) {
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION[0];

	if (a_skse->IsEditor()) {
		REX::CRITICAL("Loaded in editor, marking as incompatible"sv);
		return false;
	}
	return true;
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface * a_skse)
{
	SetupLog();
	SKSE::InitInfo info;
	info.log = false;
	info.hook = true;
	info.trampoline = true;
	info.trampolineSize = 28u;
	
	SKSE::Init(a_skse, info);

	const auto ver = a_skse->RuntimeVersion();

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

	if ((ver < SKSE::RUNTIME_SSE_LATEST) && (!std::ranges::contains(supported, ver))) {
		REX::CRITICAL("Game Version: {}"sv, ver.string());
		REX::CRITICAL("Supported Versions:"sv);
		for (const auto& allowed : supported) {
			REX::CRITICAL("  - {}"sv, allowed.string());
		}
		REX::FAIL(FormatFailureMessage("You are using a version not supported by this plugin"sv));
	}

	const auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener(&MessageEventCallback);

	SKSE::GetMessagingInterface()->RegisterListener(
        "CustomSkills",
        [](auto msg)
        {
			SECTION_SEPARATOR;
			REX::INFO("Resolving Custom Skills Framework API..."sv);
			auto* customSkills = APIStorage::Storage::GetSingleton()->customSkills;
            CustomSkills::QueryCustomSkillsInterface(msg, customSkills);
			if (!customSkills) {
				REX::CRITICAL("  - Custom Skills is still null after resolving."sv);
				REX::FAIL(FormatFailureMessage("Failed to resolve the Customs Skills Framework API."sv));
			}
			REX::INFO("Done."sv);
        });
	return true;
}