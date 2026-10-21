#pragma once

#include "ButtonEvent/ButtonEvent.hpp"

namespace Events 
{
    [[nodiscard]] inline bool RegisterForGameEvents() noexcept {
        REX::INFO("Registering for game events..."sv);
        bool success = true;
        REX::INFO("  - Button Events"sv);
        success &= ButtonEvent::Register();

        return success;
    }
}