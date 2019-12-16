#pragma once

#include <string>

class GameDayInfo;

class UIManager
{
public:
	UIManager(GameDayInfo* gameDayInfo);
	~UIManager();

	bool AreThereGamesToDisplay();
	unsigned int GetCurrentlySelectedGameIndex();
	unsigned int GetTotalNumberOfGamesToDisplay();
	const std::string& GetGameImageFilePath(unsigned int gameIndex);
	const std::string& GetGameHeaderText(unsigned int gameIndex);
	const std::string& GetGameFooterText(unsigned int gameIndex);
	void IncrementCurrentlySelectedGameIndex();
	void DecrementCurrentlySelectedGameIndex();

private:
	GameDayInfo* m_gameDayInfo;
	unsigned int m_currentlySelectedGameIndex = 0;
};

