#pragma once

namespace LichdomManager
{
	class Lichdom : public REX::Singleton<Lichdom>
	{
	public:
		[[nodiscard]] bool Initialize(const SKSE::MessagingInterface::Message* msg);
		[[nodiscard]] bool HasInitialized() const { return _csfIntfc != nullptr; };

	private:
		CustomSkills::CustomSkillsInterface* _csfIntfc = nullptr;
	};

	[[nodiscard]] bool RegisterLichdom(const SKSE::MessagingInterface::Message* msg);
}