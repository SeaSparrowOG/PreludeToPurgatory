#pragma once

namespace SKSE::log
{
    template <class... Args>
    void debug(Args&&... args)
    {
        REX::DEBUG(std::forward<Args>(args)...);
    }
    template <class... Args>
    void info(Args&&... args)
    {
        REX::INFO(std::forward<Args>(args)...);
    }
    template <class... Args>
    void warn(Args&&... args)
    {
        REX::WARN(std::forward<Args>(args)...);
    }
    template <class... Args>
    void error(Args&&... args)
    {
        REX::ERROR(std::forward<Args>(args)...);
    }
    template <class... Args>
    void critical(Args&&... args)
    {
        REX::CRITICAL(std::forward<Args>(args)...);
    }
}

#include <CustomSkills/Interfaces.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#define RelocationID(se, ae) ID(ae)
#include <API_ActorValueGenerator.h>
#undef RelocationID

namespace APIStorage
{
    class Storage final : 
        public REX::TSingleton<Storage>
    {
    public:
        AVG::API::CurrentInterface*          actorValueGenerator = nullptr;
        CustomSkills::CustomSkillsInterface* customSkills = nullptr;
    };

    [[nodiscard]]
    inline bool GetAVGInterface() {
        auto* storage = Storage::GetSingleton();
        if (!storage) {
            REX::CRITICAL("  - Failed to get the internal API Storage."sv);
            return false;
        }

        auto* actorValueGenerator = AVG::API::RequestInterface();
        if (!actorValueGenerator) {
            REX::CRITICAL("  - Failed to retrieve AVG API."sv);
        }
        if (!storage->customSkills) {
            REX::CRITICAL("  - Failed to retrive custom skills interface."sv);
        }

        storage->actorValueGenerator = actorValueGenerator;
        return true;
    }
}