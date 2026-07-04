#include "Lichdom.h"

namespace LichdomManager
{
	bool RegisterLichdom(const SKSE::MessagingInterface::Message* msg) {
		logger::info("Registering Lichdom Manager..."sv);
		if (!msg) {
			logger::critical("  - Message is null, aborting!"sv);
			return false;
		}
		auto* lichdom = Lichdom::GetSingleton();
		if (!lichdom) {
			logger::critical("  - Failed to get internal Lichdom manager!"sv);
			return false;
		}
		return lichdom->Initialize(msg);
	}

	bool Lichdom::Initialize(const SKSE::MessagingInterface::Message* msg) {
		CustomSkills::QueryCustomSkillsInterface(msg, _csfIntfc);
		if (!_csfIntfc) {
			logger::critical("  - Failed to query for the CSF interface. Custom skills likely not installed."sv);
			return false;
		}
		logger::info("  - Done!"sv);
		return true;
	}
}