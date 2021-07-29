#pragma once

// Standard libraries
#include <string>
#include <vector>

// Open-source libraries
#include "vendor/json/json.h" // A JSON library provided by https://github.com/nlohmann/json

// Curl
#define CURL_STATICLIB
#include "curl/curl.h"

// Library defined values
#define KIDCURL_DEFAULT_TIMEOUT 10L
#define KIDCURL_DEFAULT_USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.8741.272 Safari/537.36"

class kidCurl
{
public:
    kidCurl();
    ~kidCurl();

    // Response structure
    struct Response
    {
        // Response
        std::string content{};

        // Status code
        long r_status{};
    };

    // URL parameter structure
    struct Parameter
    {
        std::string name{};
        std::string value{};
    };

    // Header structure
    struct Header
    {
        std::string name{};
        std::string value{};
    };

    // Proxy structure
    struct Proxy
    {
        std::string proxy{};

        std::string username{};
        std::string password{};
    };

    // Discord embed structure
    struct Embed
    {
        std::string title{};
        std::string description{};
        unsigned int color{};
    };

    Response Get(std::string rUrl, const std::vector<Parameter>& parameters = {}, const std::vector<Header>& headers = {}, const Proxy& proxy = {}, const std::string& user_agent = KIDCURL_DEFAULT_USER_AGENT, long timeout = KIDCURL_DEFAULT_TIMEOUT);
    Response Post(std::string url, const std::string& content, const std::vector<Parameter>& parameters = {}, const std::vector<Header>& headers = {}, const Proxy& proxy = {}, const std::string& user_agent = KIDCURL_DEFAULT_USER_AGENT, long timeout = KIDCURL_DEFAULT_TIMEOUT);
    Response Put(std::string url, const std::string& content, const std::vector<Parameter>& parameters = {}, const std::vector<Header>& headers = {}, const Proxy& proxy = {}, const std::string& user_agent = KIDCURL_DEFAULT_USER_AGENT, long timeout = KIDCURL_DEFAULT_TIMEOUT);
    Response Delete(std::string rUrl, const std::vector<Parameter>& parameters = {}, const std::vector<Header>& headers = {}, const Proxy& proxy = {}, const std::string& user_agent = KIDCURL_DEFAULT_USER_AGENT, long timeout = KIDCURL_DEFAULT_TIMEOUT);
    bool webhookExecute(const std::string& webhookUrl, const std::string& content, const std::string& username = "", const std::string& avatarUrl = "", const std::vector<Embed>& embeds = {});


private:
    CURL* curl;

    //Callback function
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

private:
    void add_url_parameters(const std::vector<kidCurl::Parameter>& parameters, std::string& url);
    void sanitize_url(std::string& url);
};