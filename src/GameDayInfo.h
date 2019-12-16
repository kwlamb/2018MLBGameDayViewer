#pragma once

#include <nlohmann_JSON/json.hpp>
using json = nlohmann::json;

#include <string>

struct SingleGameInfo
{
	std::string m_headline;
	std::string m_shortDescription;
	std::string m_thumbnailFilePath;
};


class GameDayInfo
{
public:
	GameDayInfo(std::string dateOfGames);
	~GameDayInfo();

	bool Init();

	unsigned int GetNumberOfGames();

	SingleGameInfo& GetSingleGameInfo(unsigned int index);

private:
	unsigned int m_numberOfGames;
	std::string m_dateOfGames;
	SingleGameInfo* m_allGameInfo;
	json m_gameDayJSONData;

	void Terminate();
	bool FetchGameData();
	bool InitializeGameInfo();
	json GetListOfDates() const;
	json GetTargetDateData(const json& listOfDates) const;
	static const std::string GetGameHeadline(const json& gameData);
	static const std::string GetGameSubheadline(const json& gameData);
	static const std::string FetchGameImage(
		const json& nextGameData, 
		int gameIndex);
};