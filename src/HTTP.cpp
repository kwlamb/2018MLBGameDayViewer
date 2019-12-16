#include "HTTP.h"

#include <iostream>

bool HTTP::GetString(
	const std::string& url,
	std::string& returnString)
{
	// Response information.
	std::unique_ptr<std::string> httpData(new std::string());

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetStringCallback);

	if (!PerformCURLOperation(curl))
	{
		std::cout << "CURL operation to get string from URL '" << url.c_str() << "' failed." << std::endl;
		return false;
	}

	curl_easy_cleanup(curl);

	returnString = httpData->c_str();

	return true;
}

bool HTTP::DownloadFile(
	const std::string& url,
	FILE* file)
{
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadFileCallback);

	if (!PerformCURLOperation(curl))
	{
		std::cout << "CURL operation to download files from URL '" << url.c_str() << "' failed." << std::endl;
		return false;
	}

	curl_easy_cleanup(curl);

	return true;
}

std::size_t HTTP::GetStringCallback(
	const char* in,
	std::size_t size,
	std::size_t num,
	std::string* out)
{
	const std::size_t totalBytes(size * num);
	out->append(in, totalBytes);
	return totalBytes;
}

size_t HTTP::DownloadFileCallback(
	void* ptr, 
	size_t size, 
	size_t nmemb, 
	void* userdata)
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

bool HTTP::PerformCURLOperation(CURL* curl)
{
	CURLcode curlCode = curl_easy_perform(curl);
	if (curlCode != CURLE_OK)
	{
		std::cout << "CURL operation failed with error code '" << curlCode << "'." << std::endl;
		return false;
	}

	long httpResponseCode = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
	if (!((httpResponseCode == 200 || httpResponseCode == 201) && httpResponseCode != CURLE_ABORTED_BY_CALLBACK))
	{
		std::cout << "Received HTTP response code: '" << httpResponseCode << "'." << std::endl;
		return false;
	}

	return true;
}