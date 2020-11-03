#include <lnurl.h>

void setup() {
	Serial.begin(9600);
	const std::string input = "https://service.com/api?q=3fc3645b439ce8e7f2553a69e5267081d96dcd340693afabe04be7b0ccd178df";
	const std::string output = Lnurl::encode(input);
	Serial.println(output.c_str());
}

void loop() {
	// Not used in this example.
}
