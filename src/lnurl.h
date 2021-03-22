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
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

	struct LnurlWithdrawParams {
		std::map<std::string, std::string> custom;
		std::string minWithdrawable;
		std::string maxWithdrawable;
		std::string defaultDescription = "";
	};

	typedef std::map<std::string, std::string> LnurlQuery;

	const std::map<std::string, std::string> shortenKeysLookupTable {
		{ "id", "id" },
		{ "nonce", "n" },
		{ "signature", "s" },
		{ "tag", "t" },
		{ "minWithdrawable", "pn" },
		{ "maxWithdrawable", "px" },
		{ "defaultDescription", "pd" }
	};

	const std::map<std::string, std::string> shortenTagsLookupTable {
		{ "withdrawRequest", "w" }
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
	std::map<std::string, std::string> custom;
	double minWithdrawable;
	double maxWithdrawable;
	std::string defaultDescription = "";
};

struct LnurlWithdrawParamsMSat {
	std::map<std::string, std::string> custom;
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

		// Same functionality as JavaScript's encodeURIComponent.
		// Keeps alphanumeric (and a few other) characters intact (NOT encoded). See:
		// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/encodeURIComponent#description
		const std::string encodeUriComponentSafeChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()";
		std::string url_encode(const std::string &value) {
			std::ostringstream escaped;
			escaped.fill('0');
			escaped << std::hex;
			for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
				std::string::value_type c = (*i);
				// Keep alphanumeric and other accepted characters intact.
				if (encodeUriComponentSafeChars.find(c) != std::string::npos) {
					escaped << c;
					continue;
				}
				// Any other characters are percent-encoded.
				escaped << std::uppercase;
				escaped << '%' << std::setw(2) << int((unsigned char) c);
				escaped << std::nouppercase;
			}
			return escaped.str();
		}

		bool is_reserved_param_key(const std::string &paramKey) {
			for (auto const &it : shortenKeysLookupTable) {
				std::string key = it.first;
				std::string value = it.second;
				if (paramKey == key || paramKey == value) {
					return true;
				}
			}
			return false;
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

		LnurlQuery shorten_query(const LnurlQuery &query) {
			LnurlQuery shortenedQuery;
			for (auto const &it : query) {
				std::string key = it.first;
				std::string value = it.second;
				if (key == "tag") {
					const auto shortTag = shortenTagsLookupTable.find(value);
					if (shortTag != shortenTagsLookupTable.end()) {
						value = shortTag->second;
					}
				}
				const auto shortKey = shortenKeysLookupTable.find(key);
				if (shortKey != shortenKeysLookupTable.end()) {
					key = shortKey->second;
				}
				shortenedQuery[key] = value;
			}
			return shortenedQuery;
		}

		std::string stringify_query(const LnurlQuery &query) {
			std::string str = "";
			// The default behavior of std::map is to sort alphabetically on the first element.
			// In this case the first element is the query parameter's key.
			// No extra sorting is required here.
			for (auto const &it : query) {
				std::string key = it.first;
				std::string value = it.second;
				str += "&";
				str += url_encode(key);
				str += "=";
				str += url_encode(value);
			}
			// Erase the first delimiter.
			if (str != "") {
				str.erase(0, 1);
			}
			return str;
		}

		void sign_query(LnurlQuery &query) {
			// The query object should be stringified in a standardized way.
			// This is needed to ensure consistent signing between device and server.
			const std::string payload = stringify_query(query);
			const std::string signature = create_signature(payload);
			query["signature"] = signature;
		}

		LnurlQuery init_query(const std::string &tag, const std::string &nonce) {
			LnurlQuery query;
			query["id"] = config.apiKey.id;
			if (config.fiatCurrency != "") {
				query["f"] = config.fiatCurrency;
			}
			query["tag"] = tag;
			query["nonce"] = nonce;
			return query;
		}

		std::string create_url(const LnurlWithdrawParams &params, const std::string &nonce = "") {
			if (nonce == "") {
				throw std::invalid_argument("Missing required argument: \"nonce\"");
			}
			std::string url = "";
			url += config.callbackUrl;
			LnurlQuery query = init_query("withdrawRequest", nonce);
			query["minWithdrawable"] = params.minWithdrawable;
			query["maxWithdrawable"] = params.maxWithdrawable;
			query["defaultDescription"] = params.defaultDescription;
			for (auto const &it : params.custom) {
				std::string customKey = it.first;
				std::string customValue = it.second;
				if (is_reserved_param_key(customKey)) {
					throw std::invalid_argument("Invalid custom parameter key (\"" + customKey + "\"): Reserved");
				}
				query[customKey] = customValue;
			}
			sign_query(query);
			if (config.shorten) {
				query = shorten_query(query);
			}
			url += "?" + stringify_query(query);
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
			params.custom = t_params.custom;
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
			params.custom = t_params.custom;
			return create_url(params, nonce);
		}
};

#endif
