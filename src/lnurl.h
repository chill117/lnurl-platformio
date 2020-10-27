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

#ifndef LNURL_H
#define LNURL_H

#include "bech32.h"
#include "crypto/hmac_sha256.h"
#include "util/strencodings.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

	struct LnurlWithdrawParams {
		std::string minWithdrawable;
		std::string maxWithdrawable;
		std::string defaultDescription = "";
	};
}

namespace Lnurl {

	std::string encode(const std::string &unencoded);
	std::string decode(const std::string &encoded);
}

struct LnurlSignerApiKey {
	std::string id;
	std::string key;
	std::string encoding = "";
};

struct LnurlSignerConfig {
	struct LnurlSignerApiKey apiKey;
	std::string callbackUrl;
	std::string fiatCurrency = "";
	bool shorten = false;
};

struct LnurlWithdrawParamsFiat {
	double minWithdrawable;
	double maxWithdrawable;
	std::string defaultDescription = "";
};

struct LnurlWithdrawParamsMSat {
	unsigned long long int minWithdrawable;
	unsigned long long int maxWithdrawable;
	std::string defaultDescription = "";
};

class LnurlSigner {

	private:
		LnurlSignerConfig config;

		void set_config(LnurlSignerConfig t_config) {
			// Encoding equal to "hex", "base64", or "" (unspecified) are permitted.
			if (
				t_config.apiKey.encoding != "hex" &&
				t_config.apiKey.encoding != "base64" &&
				t_config.apiKey.encoding != ""
			) {
				throw std::invalid_argument("Invalid config (\"apiKey.encoding\"): Unsupported encoding - \"hex\", \"base64\", or \"\" are permitted.");
			}
			config = t_config;
		}

		std::string url_encode(const std::string &value) {
			std::ostringstream escaped;
			escaped.fill('0');
			escaped << std::hex;
			for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
				std::string::value_type c = (*i);
				// Keep alphanumeric and other accepted characters intact
				if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
					escaped << c;
					continue;
				}
				// Any other characters are percent-encoded
				escaped << std::uppercase;
				escaped << '%' << std::setw(2) << int((unsigned char) c);
				escaped << std::nouppercase;
			}
			return escaped.str();
		}

		std::string to_string(const double &number) {
			std::ostringstream ss;
			ss << number;
			return ss.str();
		}

		std::string to_string(const unsigned long long int &number) {
			std::ostringstream ss;
			ss << number;
			return ss.str();
		}

		std::string string_to_hex(const std::string& in) {
			std::stringstream ss;
			ss << std::hex << std::setfill('0');
			for (size_t i = 0; in.length() > i; ++i) {
				ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
			}
			return ss.str();
		}

		std::string create_signature(const std::string &t_data) {
			std::vector<unsigned char> data = ParseHex(string_to_hex(t_data));
			std::vector<unsigned char> key;
			if (config.apiKey.encoding == "hex") {
				// Hex-encoded key.
				key = ParseHex(config.apiKey.key);
			} else if (config.apiKey.encoding == "base64") {
				// Base64 encoded key.
				key = ParseHex(string_to_hex(DecodeBase64(config.apiKey.key)));
			} else if (config.apiKey.encoding == "") {
				// Unspecified encoding.
				key = ParseHex(string_to_hex(config.apiKey.key));
			} else {
				// This should never happen.
				throw std::logic_error("Invalid config (\"apiKey.encoding\"): Unsupported encoding - \"hex\", \"base64\", or \"\" are permitted.");
			}
			std::vector<unsigned char> signature;
			signature.resize(32);
			CHMAC_SHA256 hasher = CHMAC_SHA256(key.data(), key.size());
			hasher.Write((unsigned char*)&data[0], data.size());
			hasher.Finalize(&signature[0]);
			return HexStr(signature);
		}

		std::string init_payload(const std::string &tag, const std::string &nonce = "") {
			std::string payload = "";
			if (nonce == "") {
				throw std::invalid_argument("Missing required argument: \"nonce\"");
			}
			payload += "id=";
			payload += url_encode(config.apiKey.id);
			payload += (config.shorten ? "&t=" : "&tag=");
			payload += url_encode(tag);
			payload += (config.shorten ? "&n=" : "&nonce=");
			payload += url_encode(nonce);
			if (config.fiatCurrency != "") {
				payload += (config.shorten ? "&f=" : "&fiatCurrency=");
				payload += url_encode(config.fiatCurrency);
			}
			return payload;
		}

		void sign_payload(std::string &payload) {
			const std::string signature = create_signature(payload);
			payload += (config.shorten ? "&s=" : "&signature=");
			payload += url_encode(signature);
		}

		std::string create_url(const LnurlWithdrawParams &params, const std::string &nonce = "") {
			std::string url = "";
			url += config.callbackUrl;
			const std::string tag = (config.shorten ? "w" : "withdrawRequest");
			std::string payload = init_payload(tag, nonce);
			payload += (config.shorten ? "&pn=" : "&minWithdrawable=");
			payload += url_encode(params.minWithdrawable);
			payload += (config.shorten ? "&px=" : "&maxWithdrawable=");
			payload += url_encode(params.maxWithdrawable);
			payload += (config.shorten ? "&pd=" : "&defaultDescription=");
			payload += url_encode(params.defaultDescription);
			sign_payload(payload);
			url += "?";
			url += payload;
			return url;
		}

	public:
		LnurlSigner(LnurlSignerConfig t_config) {
			set_config(t_config);
		}

		std::string create_url(const LnurlWithdrawParamsMSat &t_params, const std::string &nonce = "") {
			if (t_params.maxWithdrawable < t_params.minWithdrawable) {
				throw std::invalid_argument("\"maxWithdrawable\" must be greater than or equal to \"minWithdrawable\"");
			}
			LnurlWithdrawParams params;
			params.minWithdrawable = to_string(t_params.minWithdrawable);
			params.maxWithdrawable = to_string(t_params.maxWithdrawable);
			params.defaultDescription = t_params.defaultDescription;
			return create_url(params, nonce);
		}

		std::string create_url(const LnurlWithdrawParamsFiat &t_params, const std::string &nonce = "") {
			if (t_params.minWithdrawable < 0) {
				throw std::invalid_argument("\"minWithdrawable\" must be greater than zero");
			}
			if (t_params.maxWithdrawable < t_params.minWithdrawable) {
				throw std::invalid_argument("\"maxWithdrawable\" must be greater than or equal to \"minWithdrawable\"");
			}
			LnurlWithdrawParams params;
			params.minWithdrawable = to_string(t_params.minWithdrawable);
			params.maxWithdrawable = to_string(t_params.maxWithdrawable);
			params.defaultDescription = t_params.defaultDescription;
			return create_url(params, nonce);
		}
};

#endif
