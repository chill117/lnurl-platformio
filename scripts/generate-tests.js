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

const inputFile = process.argv[2];
const outputFile = process.argv[3];
if (!inputFile || !outputFile) {
	console.error('Usage: SCRIPT <input-file> <output-file>`');
	process.exit(1);
}

const _ = require('underscore');
const fs = require('fs');
const lnurl = require('lnurl');
const path = require('path');

const tests = [
	{
		name: 'create_url_withdraw_msat',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_withdraw_msat',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 40000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
			fiatCurrency: '',
		},
	},
	{
		name: 'create_url_withdraw_fiat',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_withdraw_fiat',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 40,
				maxWithdrawable: 40,
				defaultDescription: 'description: test_withdraw_fiat',
			},
			fiatCurrency: 'EUR',
		},
	},
	{
		name: 'create_url_withdraw_msat_shortened',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: true,
			nonce: 'test_w_msat_s',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
			fiatCurrency: '',
		},
	},
	{
		name: 'create_url_withdraw_fiat_shortened',
		args: {
			apiKey: {
				id: '2bd84343e7',
				key: '6e778c37ed08882a934ad1a038d4e967b8a31dc2dbee9dba91de2ab6ded357db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/u',
			shorten: true,
			nonce: 'test_w_fiat_s',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50,
				maxWithdrawable: 50,
				defaultDescription: '',
			},
			fiatCurrency: 'EUR',
		},
	},
	{
		name: 'create_url_base64_encoded_key',
		args: {
			apiKey: {
				id: '9Pfv1wY=',
				key: 'zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=',
				encoding: 'base64',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_base64_encoded_key',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
			fiatCurrency: '',
		},
	},
	{
		name: 'create_url_unspecified_encoding',
		args: {
			apiKey: {
				id: '9Pfv1wY=',
				key: 'zc5UwZHCrug1GuOTIodVQKFii40gXEDbydClsqs/tIM=',
				encoding: '',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_base64_unspecified_encoding',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
			fiatCurrency: '',
		},
	},
	{
		name: 'create_url_unescaped_escaped_reserved_chars',
		args: {
			apiKey: {
				id: '2bd84343e7',
				key: '6e778c37ed08882a934ad1a038d4e967b8a31dc2dbee9dba91de2ab6ded357db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_unescaped_escaped_reserved_chars',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 12,
				maxWithdrawable: 12,
				defaultDescription: 'abcABC0123 ESCAPED # UNESCAPED -_.!~*\'() RESERVED ;,/?:@&=+$',
			},
			fiatCurrency: 'EUR',
		},
	},
];

const replacements = {
	SIGNER_TESTS: _.chain(tests).map(function(test) {
		const { name } = test;
		let { apiKey, callbackUrl, shorten, nonce, tag, params, fiatCurrency } = test.args;
		let options = {
			baseUrl: callbackUrl,
			encode: false,
			shorten,
		};
		if (fiatCurrency) {
			params.f = fiatCurrency;
		}
		if (nonce) {
			params.nonce = nonce;
		}
		const signedUrl = lnurl.createSignedUrl(apiKey, tag, params, options);
		return `void test_signer_${name}(void) {
	LnurlSignerConfig config;
	config.apiKey.id = "${apiKey.id}";
	config.apiKey.key = "${apiKey.key}";
	config.apiKey.encoding = "${apiKey.encoding}";
	config.callbackUrl = "${callbackUrl}";
	config.fiatCurrency = "${fiatCurrency}";
	config.shorten = ${shorten};
	LnurlSigner signer(config);
	const std::string nonce = "${nonce}";
	LnurlWithdrawParamsMSat params;
	params.minWithdrawable = ${params.minWithdrawable};
	params.maxWithdrawable = ${params.maxWithdrawable};
	params.defaultDescription = "${params.defaultDescription}";
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "${signedUrl}";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}`;
	}).value().join('\n\n'),
	SIGNER_RUN_TESTS: _.chain(tests).pluck('name').map(function(name) {
		return `\tRUN_TEST(test_signer_${name});`;
	}).value().join('\n'),
	DO_NOT_MODIFY: `// ---------------------------------------------------
// ---       AUTO-GENERATED // DO NOT MODIFY       ---
// ---------------------------------------------------
// ---        See ./test/templates/main.cpp        ---
// ---------------------------------------------------`,
};

let template = fs.readFileSync(path.resolve(inputFile)).toString();
_.each(replacements, function(value, key) {
	template = template.replace(`// {{${key}}}`, value);
});
fs.writeFileSync(path.resolve(outputFile), template);
