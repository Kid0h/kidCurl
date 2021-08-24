# kidCurl 
A Curl wrapper for kids!

## What is it?
A wrapper for simplified Curl requests with a C++-style interface.


## Building
To build the library you'll need Visual Studio and already built Curl static libraries.


Open the .sln with Visual Studio, and go to `build -> Batch Build`, choose the configuration/s you want, and click on `build`.


***P.S:** I complied Curl's static libraries myself and included them in this project if you don't want to build them yourself.*

## Usage:
#### Including:
You'll need to include the `include` folder in your project's include folders.


#### Linking: 
You'll only need to link against `kidCurl.lib`.

Inside the `libs` folder you'll have the built library for each configuration, for example:

If you're are building your project for `x86` in `Debug` mode, you'll need to link agaist `libs\x86\Debug\kidCurl.lib`.

If you'll be building for `x64` in `Release` mode, you'll need to link agaist `libs\x64\Release\kidCurl.lib`.

## Examples:
GET Request:
```c++
#include <iostream>
#include <kidCurl/kidCurl.hpp>

#define URL "https://www.myexternalip.com/raw"

int main(void) {
	// Our request handler, we will use it to send out requests.
	kidCurl reqHandler;

	auto req = reqHandler.Send(kidCurl::Type::GET, URL);
	if (req) {
		std::cout << "Status code: " << req->status_code << std::endl; // Request's HTTP Status code.
		std::cout << "\nBody: " << req->body << std::endl; // Request's body.
		std::cout << "\nTotal time: " << (float)req->total_time / 1000 << std::endl; // Request's total time in milliseconds, divided by 1000 to get value in seconds.
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
