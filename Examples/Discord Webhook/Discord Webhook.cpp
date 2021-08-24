#include <iostream>

#include "nlohmann/json.hpp" // A JSON library provided by https://github.com/nlohmann/json
#include <kidCurl/kidCurl.hpp>

using json = nlohmann::json;


int main(void) {
	// Request handler
	kidCurl reqHandler;

	std::string webhook_url;
	while (true) {
		// Get input
		webhook_url = "";
		std::cout << "Enter Webhook URL: ";
		std::getline(std::cin, webhook_url);

		// Validate webhook, first we check if the url is even from discord
		// then if it is we check if the url leads to a valid webhook (http status code == 200)
		if (webhook_url.rfind("https://discord.com/api/webhooks/", 0) == 0
			|| webhook_url.rfind("https://canaray.discord.com/api/webhooks/", 0) == 0
			|| webhook_url.rfind("https://ptb.discord.com/api/webhooks/", 0) == 0) {
			std::cout << "\n";
			if (reqHandler.Send(kidCurl::Type::GET, webhook_url)->status_code == 200)
				break;
			else
				std::cout << "Invalid webhook.\n";
		}
		else
			std::cout << "Invalid webhook.\n";
	}

	std::cout << "\n~ Enter empty message to quit ~ \n\nEnter messages:\n";
	while (true) {
		// Get input
		std::string message;
		std::cout << ": ";
		std::getline(std::cin, message);
		if (message == "") break;

		// Encode body in JSON
		json content;
		content["content"] = message;

		reqHandler.Send(
			kidCurl::Type::POST,
			webhook_url,
			content.dump(),
			{},
			{ {"Content-Type", "application/json"} }
		);
	}

	return 0;
}