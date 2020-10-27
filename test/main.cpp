//  lnurl-platformio - https://github.com/chill117/lnurl-platformio
//
//  Copyright (C) 2020 Charles Hill
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "lnurl.h"

#include <unity.h>
#include <stdexcept>

void test_encode(void) {
	const std::string unencoded = "https://service.com/api?q=3fc3645b439ce8e7f2553a69e5267081d96dcd340693afabe04be7b0ccd178df";
	const std::string expected = "lnurl1dp68gurn8ghj7um9wfmxjcm99e3k7mf0v9cxj0m385ekvcenxc6r2c35xvukxefcv5mkvv34x5ekzd3ev56nyd3hxqurzepexejxxepnxscrvwfnv9nxzcn9xq6xyefhvgcxxcmyxymnserxfq5fns";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		Lnurl::encode(unencoded).c_str()
	);
}

void test_decode(void) {
	const std::string encoded = "lnurl1dp68gurn8ghj7um9wfmxjcm99e3k7mf0v9cxj0m385ekvcenxc6r2c35xvukxefcv5mkvv34x5ekzd3ev56nyd3hxqurzepexejxxepnxscrvwfnv9nxzcn9xq6xyefhvgcxxcmyxymnserxfq5fns";
	const std::string expected = "https://service.com/api?q=3fc3645b439ce8e7f2553a69e5267081d96dcd340693afabe04be7b0ccd178df";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		Lnurl::decode(encoded).c_str()
	);
}

void test_signer_create_url_missing_nonce(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	LnurlSigner signer(config);
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	try {
		signer.create_url(params);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"Missing required argument: \"nonce\"",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw_fiat_minWithdrawable_lt_zero(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.fiatCurrency = "CZK";
	config.shorten = false;
	LnurlSigner signer(config);
	LnurlWithdrawParamsFiat params;
	params.minWithdrawable = -1;
	params.maxWithdrawable = 10000;
	params.defaultDescription = "";
	try {
		signer.create_url(params);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"\"minWithdrawable\" must be greater than zero",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw_msat_maxWithdrawable_lt_minWithdrawable(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	LnurlSigner signer(config);
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 80000;
	params.maxWithdrawable = 70000;
	params.defaultDescription = "";
	try {
		signer.create_url(params);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"\"maxWithdrawable\" must be greater than or equal to \"minWithdrawable\"",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw_fiat_maxWithdrawable_lt_minWithdrawable(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.fiatCurrency = "CZK";
	config.shorten = false;
	LnurlSigner signer(config);
	LnurlWithdrawParamsFiat params;
	params.minWithdrawable = 75.00;
	params.maxWithdrawable = 70.00;
	params.defaultDescription = "";
	try {
		signer.create_url(params);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"\"maxWithdrawable\" must be greater than or equal to \"minWithdrawable\"",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw_msat(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_withdraw_msat";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&tag=withdrawRequest&nonce=test_withdraw_msat&minWithdrawable=50000&maxWithdrawable=60000&defaultDescription=&signature=bc84dea0d79193208cbcb3a8cf848620016306d4bd01254ef2c6d6bd75e83ab7";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_fiat(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.fiatCurrency = "CZK";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_withdraw_fiat";
	LnurlWithdrawParamsFiat params;
	params.minWithdrawable = 50.00;
	params.maxWithdrawable = 50.00;
	params.defaultDescription = "description: test_withdraw_fiat";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&tag=withdrawRequest&nonce=test_withdraw_fiat&fiatCurrency=CZK&minWithdrawable=50&maxWithdrawable=50&defaultDescription=description%3A%20test_withdraw_fiat&signature=99bdbe1f273732640b1c02f83f3bf54e7bba8bd95000c6782375006d0ea08d3a";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_msat_shortened(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = true;
	LnurlSigner signer(config);
	const std::string nonce = "test_w_msat_s";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&t=w&n=test_w_msat_s&pn=50000&px=60000&pd=&s=6b07f932343090197a962a2dc6f9ba480ba30fd91554dc40259db0304d77a001";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_fiat_shortened(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.fiatCurrency = "CZK";
	config.shorten = true;
	LnurlSigner signer(config);
	const std::string nonce = "test_w_fiat_s";
	LnurlWithdrawParamsFiat params;
	params.minWithdrawable = 50.00;
	params.maxWithdrawable = 50.00;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&t=w&n=test_w_fiat_s&f=CZK&pn=50&px=50&pd=&s=a96605f2e13c3211f0752b8c9870def55e0e854d57254ab0f8fae4975e85e1d3";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_base64_encoded_key(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "9Pfv1wY=";
	config.apiKey.key = "zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=";
	config.apiKey.encoding = "base64";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_base64_encoded_key";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=9Pfv1wY%3D&tag=withdrawRequest&nonce=test_base64_encoded_key&minWithdrawable=50000&maxWithdrawable=60000&defaultDescription=&signature=d4c92e08259b5fe9dd41f94433f0416d703f36e108d5eeb00dc075b2d255f5a5";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_unspecified_encoding(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "9Pfv1wY=";
	config.apiKey.key = "zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=";
	config.apiKey.encoding = "";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_base64_unspecified_encoding";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=9Pfv1wY%3D&tag=withdrawRequest&nonce=test_base64_unspecified_encoding&minWithdrawable=50000&maxWithdrawable=60000&defaultDescription=&signature=7ef98c9c2b874d4f4b58746bb105bc8eef9993f25332362f8fe87384b0b6cc26";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_encode);
	RUN_TEST(test_decode);
	RUN_TEST(test_signer_create_url_missing_nonce);
	RUN_TEST(test_signer_create_url_withdraw_fiat_minWithdrawable_lt_zero);
	RUN_TEST(test_signer_create_url_withdraw_msat_maxWithdrawable_lt_minWithdrawable);
	RUN_TEST(test_signer_create_url_withdraw_fiat_maxWithdrawable_lt_minWithdrawable);
	RUN_TEST(test_signer_create_url_withdraw_msat);
	RUN_TEST(test_signer_create_url_withdraw_fiat);
	RUN_TEST(test_signer_create_url_withdraw_msat_shortened);
	RUN_TEST(test_signer_create_url_withdraw_fiat_shortened);
	RUN_TEST(test_signer_create_url_base64_encoded_key);
	RUN_TEST(test_signer_create_url_unspecified_encoding);
	return UNITY_END();
}

void setup() {
	main();
}

void loop() {
}
