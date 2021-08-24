#pragma once

// Standard libraries
#include <string>
#include <memory>
#include <vector>

// Curl
#define CURL_STATICLIB // Comment this if you have a dynamic build of curl
#include "curl/curl.h"
#include "curl/curlver.h"

// Library defined values
#define KIDCURL_DEFAULT_TIMEOUT_MS 10000L
#define KIDCURL_FAKE_USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.8741.272 Safari/537.36"

// WinAPI definition that can collide with keywords in here
#ifdef DELETE
#undef DELETE 
#endif

#pragma warning(disable:4267)

class kidCurl
{
public:
    kidCurl();
    ~kidCurl();

    enum class Type {
        GET,
        HEAD,
        POST,
        PATCH,
        PUT,
        DELETE
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
        std::string url{};

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

    // Response structure
    struct Response
    {
        // Body
        std::string body;

        // Headers
        std::vector<Header> headers;

        // Status code
        long status_code = 0;

        // Total time in ms
        long long total_time = 0;
    };

    std::shared_ptr<kidCurl::Response> Send(kidCurl::Type type, std::string url, const std::string& content = "", const std::vector<kidCurl::Parameter>& parameters = {}, const std::vector<kidCurl::Header>& headers = {}, long timeout = KIDCURL_DEFAULT_TIMEOUT_MS, const std::string& user_agent = "curl/" LIBCURL_VERSION, const kidCurl::Proxy& proxy = {});
private:
    // Helper functions

    CURL* curl;

    // Callback functions
    static size_t WriteCallback(void* buffer, size_t size, size_t nmemb, void* userp);
    static size_t HeaderCallback(char* buffer, size_t size, size_t nmemb, void* userp);

    static void curl_add_skeleton(CURL* curl, const char* url, const std::string& content, const char* user_agent, const Proxy& proxy, curl_slist* headers, kidCurl::Type type, long& timeout, kidCurl::Response* output);
    static void curl_get_info(CURL* curl, Response* output);
    static void add_url_parameters(CURL* curl, const std::vector<kidCurl::Parameter>& parameters, std::string& url);
    static void parse_raw_header(char* raw_header, size_t len, std::vector<kidCurl::Header>* headers);
};