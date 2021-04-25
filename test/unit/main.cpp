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
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "60000";
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

void test_signer_create_url_withdraw_minWithdrawable_lt_zero(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "-1";
	params.maxWithdrawable = "10000";
	params.defaultDescription = "";
	try {
		signer.create_url(params, nonce);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"\"minWithdrawable\" must be greater than zero",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw_maxWithdrawable_lt_minWithdrawable(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "75000";
	params.maxWithdrawable = "50000";
	params.defaultDescription = "";
	try {
		signer.create_url(params, nonce);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"\"maxWithdrawable\" must be greater than or equal to \"minWithdrawable\"",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_custom_params_reserved(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	Lnurl::Signer signer(config);
	const std::string nonce = "test";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "50000";
	params.defaultDescription = "";
	params.custom["id"] = "cannot_use_reserved_keys";
	try {
		signer.create_url(params, nonce);
	} catch (const std::invalid_argument& e) {
		TEST_ASSERT_EQUAL_STRING(
			"Invalid custom parameter key (\"id\"): Reserved",
			e.what()
		);
		return;
	}
	TEST_FAIL_MESSAGE("Expected exception to be thrown");
}

void test_signer_create_url_withdraw(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_withdraw";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "40000";
	params.maxWithdrawable = "60000";
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=5d4aeb462a&maxWithdrawable=60000&minWithdrawable=40000&nonce=test_withdraw&signature=73eed1984c403ff10d3ae211fe0dbc880ce16923abe3da08aedea5cd80fb7f8f&tag=withdrawRequest";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_shortened(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = true;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_w_s";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "60000";
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?id=5d4aeb462a&n=test_w_s&pd=&pn=50000&px=60000&s=de03a6590821287d5cba14f99e74fa8fb047118ea2cf1a8f0e812a6c42a99277&t=w";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_base64_encoded_key(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "9Pfv1wY=";
	config.apiKey.key = "zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=";
	config.apiKey.encoding = "base64";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_base64_encoded_key";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "60000";
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=9Pfv1wY%3D&maxWithdrawable=60000&minWithdrawable=50000&nonce=test_base64_encoded_key&signature=b4cfc6a922055b6be83d226a2ea85f12f01de39102b9865bab877e08b06f0927&tag=withdrawRequest";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_unspecified_encoding(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "9Pfv1wY=";
	config.apiKey.key = "zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=";
	config.apiKey.encoding = "";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_base64_unspecified_encoding";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "60000";
	params.defaultDescription = "";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=&id=9Pfv1wY%3D&maxWithdrawable=60000&minWithdrawable=50000&nonce=test_base64_unspecified_encoding&signature=42de7ce7a83fd51ae6eaafc3c72265dc169e3636706946f680c059bb07d8f66b&tag=withdrawRequest";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_unescaped_escaped_reserved_chars(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "2bd84343e7";
	config.apiKey.key = "6e778c37ed08882a934ad1a038d4e967b8a31dc2dbee9dba91de2ab6ded357db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_unescaped_escaped_reserved_chars";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "12";
	params.maxWithdrawable = "12";
	params.defaultDescription = "abcABC0123 ESCAPED # UNESCAPED -_.!~*'() RESERVED ;,/?:@&=+$";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?defaultDescription=abcABC0123%20ESCAPED%20%23%20UNESCAPED%20-_.!~*'()%20RESERVED%20%3B%2C%2F%3F%3A%40%26%3D%2B%24&id=2bd84343e7&maxWithdrawable=12&minWithdrawable=12&nonce=test_unescaped_escaped_reserved_chars&signature=f12453262af4848638811a37f3c4c15db764cc1f915139bb16516602c2ac1908&tag=withdrawRequest";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}

void test_signer_create_url_withdraw_custom_params(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	config.callbackUrl = "https://localhost:3000/lnurl";
	config.shorten = false;
	Lnurl::Signer signer(config);
	const std::string nonce = "test_withdraw_custom_params";
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "50000";
	params.defaultDescription = "";
	params.custom["custom1"] = "custom param";
	params.custom["custom2"] = "another custom param";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "https://localhost:3000/lnurl?custom1=custom%20param&custom2=another%20custom%20param&defaultDescription=&id=5d4aeb462a&maxWithdrawable=50000&minWithdrawable=50000&nonce=test_withdraw_custom_params&signature=d91d2e693075c9e5f2e97bf212593655145508c96c5813c10e3f8f3c86141e1d&tag=withdrawRequest";
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
	RUN_TEST(test_signer_create_url_withdraw_minWithdrawable_lt_zero);
	RUN_TEST(test_signer_create_url_withdraw_maxWithdrawable_lt_minWithdrawable);
	RUN_TEST(test_signer_create_url_custom_params_reserved);
	RUN_TEST(test_signer_create_url_withdraw);
	RUN_TEST(test_signer_create_url_withdraw_shortened);
	RUN_TEST(test_signer_create_url_base64_encoded_key);
	RUN_TEST(test_signer_create_url_unspecified_encoding);
	RUN_TEST(test_signer_create_url_unescaped_escaped_reserved_chars);
	RUN_TEST(test_signer_create_url_withdraw_custom_params);
	return UNITY_END();
}

void setup() {
	main();
}

void loop() {
}
