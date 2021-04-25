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
		name: 'create_url_withdraw',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_withdraw',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 40000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
		},
	},
	{
		name: 'create_url_withdraw_shortened',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: true,
			nonce: 'test_w_s',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50000,
				maxWithdrawable: 60000,
				defaultDescription: '',
			},
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
		},
	},
	{
		name: 'create_url_withdraw_custom_params',
		args: {
			apiKey: {
				id: '5d4aeb462a',
				key: 'ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db',
				encoding: 'hex',
			},
			callbackUrl: 'https://localhost:3000/lnurl',
			shorten: false,
			nonce: 'test_withdraw_custom_params',
			tag: 'withdrawRequest',
			params: {
				minWithdrawable: 50000,
				maxWithdrawable: 50000,
				defaultDescription: '',
			},
			customParams: {
				custom1: 'custom param',
				custom2: 'another custom param',
			},
		},
	},
];

const replacements = {
	SIGNER_TESTS: _.chain(tests).map(function(test) {
		const { name } = test;
		let { apiKey, callbackUrl, shorten, nonce, tag, params, customParams } = test.args;
		let options = {
			baseUrl: callbackUrl,
			encode: false,
			shorten,
		};
		if (nonce) {
			params.nonce = nonce;
		}
		let SIGNER_TEST = '';
		SIGNER_TEST += `void test_signer_${name}(void) {
	Lnurl::SignerConfig config;
	config.apiKey.id = "${apiKey.id}";
	config.apiKey.key = "${apiKey.key}";
	config.apiKey.encoding = "${apiKey.encoding}";
	config.callbackUrl = "${callbackUrl}";
	config.shorten = ${shorten};
	Lnurl::Signer signer(config);
	const std::string nonce = "${nonce}";`;
	SIGNER_TEST += `
	Lnurl::WithdrawParams params;
	params.minWithdrawable = "${params.minWithdrawable}";
	params.maxWithdrawable = "${params.maxWithdrawable}";
	params.defaultDescription = "${params.defaultDescription}";`;
	if (customParams) {
		params = _.defaults(params, customParams);
		_.each(customParams, function(value, key) {
			SIGNER_TEST += `
	params.custom["${key}"] = "${value}";`;
		});
	}
	const signedUrl = lnurl.createSignedUrl(apiKey, tag, params, options);
	SIGNER_TEST += `
	const std::string result = signer.create_url(params, nonce);
	const std::string expected = "${signedUrl}";
	TEST_ASSERT_EQUAL_STRING(
		expected.c_str(),
		result.c_str()
	);
}`;
		return SIGNER_TEST;
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
