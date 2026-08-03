#pragma once

namespace LichdomManager
{
	class Lichdom : public REX::Singleton<Lichdom>
	{
	public:
		[[nodiscard]] bool Initialize(const SKSE::MessagingInterface::Message* msg);
		[[nodiscard]] bool HasInitialized() const { return _csfIntfc != nullptr; };

		void ShowSkillMenu() const;

	private:
		inline static constexpr std::string_view skill = "lichdom";
		CustomSkills::CustomSkillsInterface* _csfIntfc = nullptr;
	};

	[[nodiscard]] bool RegisterLichdom(const SKSE::MessagingInterface::Message* msg);

	void OpenSkillMenu();
}