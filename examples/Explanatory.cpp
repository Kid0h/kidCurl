#include <iostream>
#include <kidCurl/kidCurl.hpp>

int main(void)
{
	kidCurl reqHandler;

	// Complext Request
	auto req = reqHandler.Send(
		kidCurl::Type::POST,										// Request method
		"https://httpbin.org/post",									// Request URL
		"Hey!",														// Body (post fields)
		{ {"Parameter 1", "value"}, {"Parameter 2", "value"} },		// URL parameters
		{ {"Content-Type", "application/json"} },					// Headers
		10000,														// Timeout delay (in milliseconds)
		"curl/" LIBCURL_VERSION										// User agent
		/*,{"127.0.0.1:8080"}*/										// Proxy
	);

	if (req) {
		std::cout << "Status code: " << req->status_code << std::endl;
		std::cout << "\nBody: " << req->body << std::endl;
		std::cout << "\nTotal time: " << (float)req->total_time / 1000 << std::endl;
	}

	// Wait for input
	std::cin.get();
	return 0;
}