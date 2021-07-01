#pragma once

#include "kidCurl/kidCurl.hpp"

kidCurl::kidCurl() {
    curl = curl_easy_init();
}
kidCurl::~kidCurl() {
    curl_easy_cleanup(curl);
}

// Handling URL parameters
void kidCurl::add_url_parameters(const std::vector<kidCurl::Parameter>& parameters, std::string& url)
{
    size_t parameters_num = parameters.size();
    if (parameters_num > 0)
    {
        url += "?";
        for (size_t i{ 0 }; i < parameters_num; i++)
            url += (parameters[i].name + "=" + parameters[i].value + "&");

        url.pop_back();
    }
}

// Sanitize URL
void kidCurl::sanitize_url(std::string& url)
{
    for (uint32_t i{}; i < url.length(); i++)
    {
        switch (url[i])
        {
        case ' ':
            url[i] = '%';
            url.insert(i + 1, "20");
            break;

        case '\'':
            url[i] = '%';
            url.insert(i + 1, "27");
            break;

        default:
            break;
        }
    }
}

// Easy GET request
kidCurl::Response kidCurl::Get(std::string url, const std::vector<Parameter>& parameters, const std::vector<Header>& headers, const Proxy& proxy, const char* user_agent, long timeout)
{
    // Response
    Response response;

    // URL Parameters & URL Sanitization
    add_url_parameters(parameters, url);
    sanitize_url(url);

    //Headers
    struct curl_slist* slist;
    slist = NULL;

    for (size_t i{ 0 }; i < headers.size(); i++)
        slist = curl_slist_append(slist, (headers[i].name + ": " + headers[i].value).c_str());

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        if (proxy.proxy != "") { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.proxy.c_str()); if (proxy.username != "") { curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, proxy.username.c_str()); curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, proxy.password.c_str()); } }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.content);

        curl_easy_perform(curl);

        // Get info
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.r_status);

        //Cleanup
        curl_easy_reset(curl);
        curl_slist_free_all(slist);
        slist = NULL;
        return response;
    }

    response.r_status = 0;
    return response;
}

// Easy POST request
kidCurl::Response kidCurl::Post(std::string url, const std::string& content, const std::vector<Parameter>& parameters, const std::vector<Header>& headers, const Proxy& proxy, const char* user_agent, long timeout)
{
    // Response
    Response response;

    // URL Parameters & URL Sanitization
    add_url_parameters(parameters, url);
    sanitize_url(url);

    //Headers
    struct curl_slist* slist;
    slist = NULL;

    for (size_t i{ 0 }; i < headers.size(); i++)
        slist = curl_slist_append(slist, (headers[i].name + ": " + headers[i].value).c_str());

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        if (proxy.proxy != "") { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.proxy.c_str()); if (proxy.username != "") { curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, proxy.username.c_str()); curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, proxy.password.c_str()); } }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.content);

        curl_easy_perform(curl);

        // Get info
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.r_status);

        // Cleanup
        curl_easy_reset(curl);
        curl_slist_free_all(slist);
        slist = NULL;

        return response;
    }

    response.r_status = 0;
    return response;
}

// Simplified Discord webhook message
bool kidCurl::webhookExecute(const std::string& webhookUrl, const std::string& content, const std::string& username, const std::string& avatar_url, const std::vector<Embed>& embeds)
{
    // Content
    json final = {};
    final["content"] = content;

    // Headers
    std::vector<Header> headers = { {"Content-Type", "application/json"} };

    // Username
    if (username != "")
        final["username"] = username;

    // Avatar
    if (avatar_url != "")
        final["avatar_url"] = avatar_url;

    // Embeds
    if (embeds.size() > 0)
    {
        final["embeds"] = json::array({});
        for (size_t i{ 0 }; i < embeds.size(); i++)
        {
            final["embeds"].push_back(json::object({ {"title", embeds[i].title}, {"description", embeds[i].description}, {"color", embeds[i].color} }));
        }
    }

    Response request = kidCurl::Post(webhookUrl, final.dump(), {}, headers);

    // Success?
    if (request.r_status == 204)
        return true;
    return false;
}

size_t kidCurl::WriteCallback(void* contents, size_t size, size_t mem, void* user)
{
    ((std::string*)user)->append((char*)contents, size * mem);

    return size * mem;
}