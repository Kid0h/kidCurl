# kidCurl: A Curl wrapper for kids!
A libcurl wrapper for simplified requests with a C++-style interface.

## Disclaimer
The kidCurl wrapper is not meant for production, it is simply an example of how one could use libcurl.

## Usage
You'll need libcurl already set up in your project in order to use this wrapper.

To start using the wrapper, include the `include` folder and you can start using `kidCurl.hpp`.
## Example
GET Request:
```c++
#include <iostream>
#include <kidCurl.hpp>

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

		// Request's total time in milliseconds, divided by 1000 to get the value in seconds.					
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
```
For more examples click [here](examples/)!

## Reaching out
If you have any issues or questions - you can open an issue [here](https://github.com/Kid0h/kidCurl/issues/new)!
