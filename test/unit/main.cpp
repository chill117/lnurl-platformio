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

// ---------------------------------------------------
// ---       AUTO-GENERATED // DO NOT MODIFY       ---
// ---------------------------------------------------
// ---        See ./test/templates/main.cpp        ---
// ---------------------------------------------------

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
	config.fiatCurrency = "";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_withdraw_msat";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 40000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=5d4aeb462a&maxWithdrawable=60000&minWithdrawable=40000&nonce=test_withdraw_msat&signature=07a0debb17fcf406c0882629fc9057ac73776bc63afe0a204bb4317e21016def&tag=withdrawRequest";
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
	config.fiatCurrency = "EUR";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_withdraw_fiat";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 40;
	params.maxWithdrawable = 40;
	params.defaultDescription = "description: test_withdraw_fiat";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=description%3A%20test_withdraw_fiat&f=EUR&id=5d4aeb462a&maxWithdrawable=40&minWithdrawable=40&nonce=test_withdraw_fiat&signature=b5a385b4f7f8b7f1b14c550eb50175009959aaff3bc8e23d52743d9c416889eb&tag=withdrawRequest";
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
	config.fiatCurrency = "";
	config.shorten = true;
	LnurlSigner signer(config);
	const std::string nonce = "test_w_msat_s";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&n=test_w_msat_s&pd=&pn=50000&px=60000&s=b6181679ca0d512905e0463b1008154c0f6d9c7aa464e996e101234b0a9fba0a&t=w";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_fiat_shortened(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "2bd84343e7";
	config.apiKey.key = "6e778c37ed08882a934ad1a038d4e967b8a31dc2dbee9dba91de2ab6ded357db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/u";
	config.fiatCurrency = "EUR";
	config.shorten = true;
	LnurlSigner signer(config);
	const std::string nonce = "test_w_fiat_s";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50;
	params.maxWithdrawable = 50;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/u?f=EUR&id=2bd84343e7&n=test_w_fiat_s&pd=&pn=50&px=50&s=1874cd9be51df827f2fb20b254e83f31f3f99505cf551538987e0b4c29cb2261&t=w";
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
	config.fiatCurrency = "";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_base64_encoded_key";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=9Pfv1wY%3D&maxWithdrawable=60000&minWithdrawable=50000&nonce=test_base64_encoded_key&signature=b4cfc6a922055b6be83d226a2ea85f12f01de39102b9865bab877e08b06f0927&tag=withdrawRequest";
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
	config.fiatCurrency = "";
	config.shorten = false;
	LnurlSigner signer(config);
	const std::string nonce = "test_base64_unspecified_encoding";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = 50000;
	params.maxWithdrawable = 60000;
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=9Pfv1wY%3D&maxWithdrawable=60000&minWithdrawable=50000&nonce=test_base64_unspecified_encoding&signature=42de7ce7a83fd51ae6eaafc3c72265dc169e3636706946f680c059bb07d8f66b&tag=withdrawRequest";
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
