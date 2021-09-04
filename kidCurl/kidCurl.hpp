#pragma once

#include <string>
#include <memory>
#include <vector>

// Curl
#include "curl/curl.h"
#include "curl/curlver.h"

// Library definitions
#define KIDCURL_DEFAULT_TIMEOUT_MS 10000L
#define KIDCURL_FAKE_USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.8741.272 Safari/537.36"

// WinAPI definition that can collide with keywords in here
#undef DELETE 

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

    inline std::shared_ptr<kidCurl::Response> Send(kidCurl::Type type, std::string url, const std::string& content = "", const std::vector<kidCurl::Parameter>& parameters = {}, const std::vector<kidCurl::Header>& headers = {}, long timeout = KIDCURL_DEFAULT_TIMEOUT_MS, const std::string& user_agent = "curl/" LIBCURL_VERSION, const kidCurl::Proxy& proxy = {"", "", ""});
private:
    // Curl handle
    CURL* curl;

    // Callback functions
    inline static size_t WriteCallback(void* buffer, size_t size, size_t nmemb, void* userp);
    inline static size_t HeaderCallback(char* buffer, size_t size, size_t nmemb, void* userp);

    // Helper functions
    inline static void curl_add_skeleton(CURL* curl, const char* url, const std::string& content, const char* user_agent, const Proxy& proxy, curl_slist* headers, kidCurl::Type type, long& timeout, kidCurl::Response* output);
    inline static void curl_get_info(CURL* curl, Response* output);
    inline static void add_url_parameters(CURL* curl, const std::vector<kidCurl::Parameter>& parameters, std::string& url);
    inline static void parse_raw_header(char* raw_header, size_t len, std::vector<kidCurl::Header>* headers);
};

kidCurl::kidCurl() {
    curl = curl_easy_init();
}
kidCurl::~kidCurl() {
    curl_easy_cleanup(curl);
}

void kidCurl::curl_add_skeleton(CURL* curl, const char* url, const std::string& content, const char* user_agent, const Proxy& proxy, curl_slist* headers, kidCurl::Type type, long& timeout, kidCurl::Response* output)
{
    // URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Headers
    if (headers)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // User agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);

    // Proxy
    if (proxy.url != "") { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.url.c_str()); if (proxy.username != "") { curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, proxy.username.c_str()); curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, proxy.password.c_str()); } }

    // Request type
    std::string t;
    switch (type) {
    case kidCurl::Type::GET:      t = "GET"; break;
    case kidCurl::Type::POST:     t = "POST"; break;
    case kidCurl::Type::PATCH:    t = "PATCH"; break;
    case kidCurl::Type::PUT:      t = "PUT"; break;
    case kidCurl::Type::DELETE:   t = "DELETE"; break;
    }
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, t.c_str());

    // Content
    if (content != "") curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());

    // Timeout
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout);

    // Output
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, output);
}

void kidCurl::curl_get_info(CURL* curl, kidCurl::Response* output) {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &output->status_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &output->total_time);
    output->total_time /= 1000; // Convert from microseconds to milliseconds
}

// Handling URL parameters
void kidCurl::add_url_parameters(CURL* curl, const std::vector<kidCurl::Parameter>& parameters, std::string& url)
{
    size_t parameters_num = parameters.size();
    if (parameters_num > 0)
    {
        url += "?";
        for (size_t i{ 0 }; i < parameters_num; i++)
        {
            url.append(curl_easy_escape(curl, parameters[i].name.c_str(), parameters[i].name.length()));
            url.push_back('=');
            url.append(curl_easy_escape(curl, parameters[i].value.c_str(), parameters[i].value.length()));
            url.push_back('&');
        }
        url.pop_back();
    }
}


// Easy request
std::shared_ptr<kidCurl::Response> kidCurl::Send(kidCurl::Type type, std::string url, const std::string& content, const std::vector<kidCurl::Parameter>& parameters, const std::vector<kidCurl::Header>& headers, long timeout, const std::string& user_agent, const kidCurl::Proxy& proxy)
{
    // Response
    std::shared_ptr<Response> response;

    if (curl)
    {
        // Allocate memory for response
        response = std::make_shared<Response>();

        // URL Parameters
        add_url_parameters(curl, parameters, url);

        //Headers
        struct curl_slist* slist = nullptr;
        for (auto& header : headers) {
            std::string line = header.name + ": " + header.value;
            slist = curl_slist_append(slist, line.c_str());
        }

        // Curl options
        curl_add_skeleton(curl, url.c_str(), content, user_agent.c_str(), proxy, slist, type, timeout, response.get());

        // Preform
        curl_easy_perform(curl);

        // Get info
        curl_get_info(curl, response.get());

        //Cleanup
        curl_easy_reset(curl);
        curl_slist_free_all(slist);
    }

    return response;
}

void kidCurl::parse_raw_header(char* raw_header, size_t len, std::vector<Header>* headers)
{
    std::string_view header(raw_header, len);
    size_t colon_pos = header.find_first_of(':');

    if (colon_pos != std::string::npos) {
        headers->push_back({ { raw_header, colon_pos }, { raw_header + colon_pos + 2, len - colon_pos - 4 } });
        // headers->emplace_back((raw_header, colon_pos), (raw_header + colon_pos + 2, len - colon_pos - 4));
    }
}

size_t kidCurl::WriteCallback(void* buffer, size_t size, size_t mem, void* ptr)
{
    kidCurl::Response* out = (kidCurl::Response*)ptr;
    out->body.append((const char*)buffer, size * mem);

    return size * mem;
}

size_t kidCurl::HeaderCallback(char* buffer, size_t size, size_t mem, void* ptr)
{
    kidCurl::Response* out = (kidCurl::Response*)ptr;
    parse_raw_header((char*)buffer, size * mem, &out->headers);

    return size * mem;
}

#pragma warning(default:4267)