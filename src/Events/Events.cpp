#include "Events.h"

namespace Events
{
	bool Register([[maybe_unused]] const SKSE::MessagingInterface::Message* msg) {
		logger::info("Registering Event Listeners..."sv);
		bool success = true;
		logger::info("  - Done!"sv);
		return success;
	}
}