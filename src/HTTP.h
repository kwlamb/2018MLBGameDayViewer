#pragma once

#include "curl/curl.h"

#include <string>

class HTTP
{
public:
	static bool GetString(
		const std::string& url,
		std::string& returnString);

	static bool DownloadFile(
		const std::string& url,
		FILE* file);

private:
	static std::size_t GetStringCallback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out);

	static size_t DownloadFileCallback(
		void* ptr,
		size_t size,
		size_t nmemb,
		void* userdata);

	static bool PerformCURLOperation(CURL* curl);
};

