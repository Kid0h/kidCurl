#include <iostream>
#include <kidCurl/kidCurl.hpp>

#define URL "https://www.myexternalip.com/raw"

int main(void) {
	// Our request handler, we will use it to send our requests.
	kidCurl reqHandler;

	// Perform request
	auto req = reqHandler.Send(kidCurl::Type::GET, URL);
	if (req) {
		// Request's HTTP Status code.
		std::cout << "Status code: " << req->status_code << std::endl;	

		// Request's body.				
		std::cout << "\nBody: " << req->body << std::endl;			

		// Request's total time in milliseconds, divided by 1000 to get value in seconds.					
		std::cout << "\nTotal time: " << (float)req->total_time / 1000 << std::endl;	

		// Print each header's name and it's value
		std::cout << "\nHeaders: \n";
		for (auto& header : req->headers)												
			std::cout << header.name << ": " << header.value << "\n";
	}

	// Wait for input
	std::cin.get();
	return 0;
}