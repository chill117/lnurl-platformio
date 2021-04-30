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

// {{WARNING_TEXT}}

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

void test_create_signature(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "5d4aeb462a";
	config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
	config.apiKey.encoding = "hex";
	Lnurl::Signer signer(config);
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "50000";
	params.maxWithdrawable = "60000";
	params.defaultDescription = "";
	const std::string payload = "sign arbitrary data string";
	const std::string result = signer.create_signature(payload);
	const std::string expected = "3dbc16c2c5cd51797211316a5257ad151dafd05347da58da9ca2319a7a669277";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
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

// {{SIGNER_TESTS}}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_encode);
	RUN_TEST(test_decode);
	RUN_TEST(test_create_signature);
	RUN_TEST(test_signer_create_url_missing_nonce);
	RUN_TEST(test_signer_create_url_withdraw_minWithdrawable_lt_zero);
	RUN_TEST(test_signer_create_url_withdraw_maxWithdrawable_lt_minWithdrawable);
	RUN_TEST(test_signer_create_url_custom_params_reserved);
// {{SIGNER_RUN_TESTS}}
	return UNITY_END();
}

void setup() {
	main();
}

void loop() {
}
