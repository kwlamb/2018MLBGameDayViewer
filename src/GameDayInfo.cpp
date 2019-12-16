#include "GameDayInfo.h"

#include "HTTP.h"

#include "curl/curl.h"

#include <string>
#include <iostream>

#include <direct.h>

GameDayInfo::GameDayInfo(std::string dateOfGames)
	: m_dateOfGames(dateOfGames)
	, m_numberOfGames(0)
{}

GameDayInfo::~GameDayInfo()
{
	Terminate();
}

bool GameDayInfo::Init()
{
	if (!FetchGameData())
	{
		return false;
	}

	if (!InitializeGameInfo())
	{
		return false;
	}

	return true;
}

void GameDayInfo::Terminate()
{
	if (m_allGameInfo != NULL)
	{
		delete[] m_allGameInfo;
		m_allGameInfo = NULL;
	}
}

unsigned int GameDayInfo::GetNumberOfGames()
{
	return m_numberOfGames;
}

SingleGameInfo& GameDayInfo::GetSingleGameInfo(unsigned int index)
{
	return m_allGameInfo[index];
}

std::size_t getJSONCallback(
	const char* in,
	std::size_t size,
	std::size_t num,
	std::string* out)
{
	const std::size_t totalBytes(size * num);
	out->append(in, totalBytes);
	return totalBytes;
}

bool GameDayInfo::FetchGameData()
{
	const std::string mlbURLPrefix = "https://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=";
	const std::string mlbURLSuffix = "&sportId=1";

	// curl code taken from https://gist.github.com/connormanning/41efa6075515019e499c

	const std::string mlbURL = mlbURLPrefix + m_dateOfGames + mlbURLSuffix;

	std::string fetchedData;

	if (!HTTP::GetString(mlbURL, fetchedData))
	{
		return false;
	}

	m_gameDayJSONData = json::parse(fetchedData);

	return true;
}

bool GameDayInfo::InitializeGameInfo()
{
	json listOfDates = GetListOfDates();

	if (listOfDates == NULL)
	{
		return false;
	}

	json targetDateData = GetTargetDateData(listOfDates);
	if (targetDateData == NULL)
	{
		return false;
	}

	m_numberOfGames = targetDateData["totalGames"];

	m_allGameInfo = new SingleGameInfo[m_numberOfGames];

	json listOfGames = targetDateData["games"];

	int gameIndex = 0;

	for (json::iterator it = listOfGames.begin(); it != listOfGames.end(); ++it)
	{
		json nextGameData = *it;

		std::string gameImageFile = FetchGameImage(nextGameData, gameIndex);

		if (gameImageFile.empty())
		{
			return false;
		}

		m_allGameInfo[gameIndex++] = 
		{
			GetGameHeadline(nextGameData),
			GetGameSubheadline(nextGameData),
			gameImageFile.c_str()
		};
	}

	return true;
}

json GameDayInfo::GetListOfDates() const
{
	if (m_gameDayJSONData["dates"].is_null())
	{
		std::cout << "JSON data: 'dates' missing." << std::endl;
		return NULL;
	}

	if (!m_gameDayJSONData["dates"].is_array())
	{
		std::cout << "JSON data: 'dates' is not an array." << std::endl;
		return NULL;
	}

	if (m_gameDayJSONData["dates"].size() < 1)
	{
		std::cout << "JSON data: 'dates' array is empty." << std::endl;
		return NULL;
	}

	return m_gameDayJSONData["dates"];
}

json GameDayInfo::GetTargetDateData(const json& listOfDates) const
{
	for (json::const_iterator it = listOfDates.begin(); it != listOfDates.end(); ++it)
	{
		json nextDate = *it;
		if (nextDate["date"] == m_dateOfGames)
		{
			return nextDate;
		}
	}

	std::cout << "Unable to find target date '" << m_dateOfGames.c_str() << "' in game data." << std::endl;
	return NULL;
}

const std::string GameDayInfo::GetGameHeadline(const json& gameData)
{
	return gameData["content"]["editorial"]["recap"]["mlb"]["headline"];
}

const std::string GameDayInfo::GetGameSubheadline(const json& gameData)
{
	return gameData["content"]["editorial"]["recap"]["mlb"]["blurb"];
}

size_t getImageCallback(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	FILE* stream = (FILE*)userdata;
	if (!stream)
	{
		printf("!!! No stream\n");
		return 0;
	}

	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	return written;
}

const std::string GameDayInfo::FetchGameImage(
	const json& gameData,
	int gameIndex)
{
	// Build the URL
	std::string gameImageURL = gameData["content"]["editorial"]["recap"]["mlb"]["photo"]["cuts"]["320x180"]["src"];

	// Build the local file name
	std::string gameImageFileName = "game" + std::to_string(gameIndex);
	gameImageFileName += "image.jpg";

	// Build the cache folder file path
	std::string gameImageLocalPath = "cache/" + gameImageFileName;

	// Create the cache folder if necessary
	struct stat st;
	int statResult;

	statResult = stat("cache", &st);

	if (statResult != 0)
	{
		if ((statResult == -1) && (errno == ENOENT))
		{
			if (_mkdir("cache") != 0)
			{
				std::cout << "Failed to create directory '" << "cache" << "'." << std::endl;
				return "";
			}
		}
		else
		{
			std::cout << "Failed to get info on directory '" << "cache" << "'." << std::endl;
			return "";
		}
	}

	FILE* filePointer;
	errno_t fopenStatus = 
		fopen_s(
			&filePointer,
			gameImageLocalPath.c_str(), 
			"wb");

	if ((fopenStatus != 0) || (!filePointer))
	{
		std::cout << "Failed to create file '" << gameImageFileName.c_str() << "' on the disk." << std::endl;
		return "";
	}

	if (!HTTP::DownloadFile(gameImageURL, filePointer))
	{
		return "";
	}

	fclose(filePointer);

	return gameImageLocalPath;
}
