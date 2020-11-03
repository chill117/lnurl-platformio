#include <lnurl.h>

void setup() {
	Serial.begin(9600);
	// Prepare the config structure.
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.fiatCurrency = "CZK";
	config.shorten = true;
	// Prepare an instance of the signer class.
	LnurlSigner signer(config);
	// Prepare lnurl-withdraw parameters.
	LnurlWithdrawParamsFiat params;
	params.minWithdrawable = 50.00;
	params.maxWithdrawable = 50.00;
	params.defaultDescription = "";
	// Generate a nonce.
	// (Pseudo-)randomly generated number and a timestamp should be sufficient.
	const std::string nonce = "this_should_be_unique_for_each_signed_url";
	// Create the signed URL.
	const std::string output = signer.create_url(params, nonce);
	Serial.println(output.c_str());
}

void loop() {
	// Not used in this example.
}
