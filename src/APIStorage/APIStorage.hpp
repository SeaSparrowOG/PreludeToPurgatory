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

#include <API_ActorValueGenerator.h>
namespace API
{
    class Storage final : 
        public REX::TSingleton<Storage>
    {
    public:
        AVG::API::InterfaceVersion3*         actorValueGenerator = nullptr;
        CustomSkills::CustomSkillsInterface* customSkills = nullptr;
    };
}