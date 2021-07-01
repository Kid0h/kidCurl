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

If you're are building your project for `Win32` in `Debug` mode, you'll need to link agaist `libs\Win32\Debug\kidCurl.lib`.

If you'll be building for `x64` in `Release` mode, you'll need to link agaist `libs\x64\Release\kidCurl.lib`.

## Examples:
Sending a GET & POST requests + a bonus Discord Webhook message:
```c++
#include <iostream>
#include "kidCurl/kidCurl.hpp"

#define URL "https://www.myexternalip.com/raw"
#define DISCORD_WEBHOOK_URL ""

int main()
{
	// Request handler
	kidCurl reqHandle;

	// Get request
	kidCurl::Response request_get = reqHandle.Get(URL);
	std::cout << "GET Request:\n"
		<< "Status: " << request_get.r_status
		<< "\nContent: " << request_get.content
		<< "\n\n";


	// Post request
	kidCurl::Response request_post = reqHandle.Post(URL, "Hey!");
	std::cout << "POST: Request\n"
		<< "Status: " << request_post.r_status
		<< "\n\n";


	// Discord webhook request
	bool request_discord_webhook = reqHandle.webhookExecute(DISCORD_WEBHOOK_URL, "Hey!");
	std::cout << "Discord Webhook: Request\n"
		<< "Success: " << std::boolalpha << request_discord_webhook
		<< "\n\n";

	return 0;
}
```

## Reaching out
If you have any issues or questions - you can open an issue [here](https://github.com/Kid0h/kidCurl/issues/new)!
