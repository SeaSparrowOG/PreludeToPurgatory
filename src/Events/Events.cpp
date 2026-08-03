#include "Events.h"

#include "Death/DeathEvent.h"
#include "Key/KeyEvent.h"
#include "Race/RaceEvent.h"

namespace Events
{
	bool Register([[maybe_unused]] const SKSE::MessagingInterface::Message* msg) {
		logger::info("Registering Event Listeners..."sv);
		bool success = true;

		success &= Race::ReadyRaceSwitchListener();
		success &= Death::ReadyDeathListener();
		success &= Key::ReadyButtonListener();

		logger::info("  - Done!"sv);
		return success;
	}
}