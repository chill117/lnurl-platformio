#include <lnurl.h>

void setup() {
	Serial.begin(9600);
	const std::string input = "lnurl1dp68gurn8ghj7um9wfmxjcm99e3k7mf0v9cxj0m385ekvcenxc6r2c35xvukxefcv5mkvv34x5ekzd3ev56nyd3hxqurzepexejxxepnxscrvwfnv9nxzcn9xq6xyefhvgcxxcmyxymnserxfq5fns";
	const std::string output = Lnurl::decode(input);
	Serial.println(output.c_str());
}

void loop() {
	// Not used in this example.
}
