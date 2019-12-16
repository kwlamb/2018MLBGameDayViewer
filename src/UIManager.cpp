#include "UIManager.h"

#include "GameDayInfo.h"

UIManager::UIManager(GameDayInfo* gameDayInfo)
	: m_gameDayInfo(gameDayInfo)
	, m_currentlySelectedGameIndex(0)
{}

UIManager::~UIManager()
{}

bool UIManager::AreThereGamesToDisplay()
{
	return m_gameDayInfo->GetNumberOfGames() > 0;
}

unsigned int UIManager::GetCurrentlySelectedGameIndex()
{
	return m_currentlySelectedGameIndex;
}

unsigned int UIManager::GetTotalNumberOfGamesToDisplay()
{
	return m_gameDayInfo->GetNumberOfGames();
}

const std::string& UIManager::GetGameImageFilePath(unsigned int gameIndex)
{
	return m_gameDayInfo->GetSingleGameInfo(gameIndex).m_thumbnailFilePath;
}

const std::string& UIManager::GetGameHeaderText(unsigned int gameIndex)
{
	return m_gameDayInfo->GetSingleGameInfo(gameIndex).m_headline;
}

const std::string& UIManager::GetGameFooterText(unsigned int gameIndex)
{
	return m_gameDayInfo->GetSingleGameInfo(gameIndex).m_shortDescription;
}

void UIManager::IncrementCurrentlySelectedGameIndex()
{
	if (m_currentlySelectedGameIndex < m_gameDayInfo->GetNumberOfGames() - 1)
	{
		m_currentlySelectedGameIndex++;
	}
}

void UIManager::DecrementCurrentlySelectedGameIndex()
{
	if (m_currentlySelectedGameIndex > 0)
	{
		m_currentlySelectedGameIndex--;
	}
}