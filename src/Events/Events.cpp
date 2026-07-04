#include "Events.h"

namespace Events
{
	bool Register() {
		logger::info("Registering Event Listeners..."sv);
		logger::info("  - Done!"sv);
		return true;
	}
}