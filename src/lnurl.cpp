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

namespace {

	std::string string_to_hex(const std::string& in) {
		std::stringstream ss;
		ss << std::hex << std::setfill('0');
		for (size_t i = 0; in.length() > i; ++i) {
			ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
		}
		return ss.str();
	}
}

namespace Lnurl {

	const std::string hrp = "lnurl";

	std::string encode(const std::string &unencoded) {
		std::vector<uint8_t> v = ParseHex(string_to_hex(unencoded));
		std::vector<unsigned char> tmp = {};
		tmp.reserve(1 + 32 * 8 / 5);
		ConvertBits<8, 5, true>([&](unsigned char c) { tmp.push_back(c); }, v.begin(), v.end());
		return bech32::Encode(hrp, tmp);
	}

	std::string decode(const std::string &encoded) {
		const std::pair<std::string, std::vector<uint8_t>> decoded = bech32::Decode(encoded);
		std::vector<unsigned char> ret;
		ret.reserve((decoded.second.size() * 5) / 8);
		ConvertBits<5, 8, false>([&](unsigned char c) { ret.push_back(c); }, decoded.second.begin(), decoded.second.end());
		return std::string(ret.begin(), ret.end());
	}
}
