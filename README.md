# lnurl-platformio

Implementation of [lnurl](https://github.com/btcontract/lnurl-rfc) in C++ for [PlatformIO](https://platformio.org/).

* [Overview](#overview)
* [Requirements](#requirements)
* [Installation](#installation)
* [Usage](#usage)
* [Tests](#tests)
* [Changelog](#changelog)
* [License](#license)


## Overview

What you can do with this library:
* Encode and decode lnurl strings
* Create signed URLs to be used with an lnurl server that supports offline signing - e.g [lnurl-node](https://github.com/chill117/lnurl-node).

Current state of lnurl subprotocol implementation:
* [x] lnurl-withdraw
* [ ] lnurl-pay
* [ ] lnurl-channel
* [ ] lnurl-auth


## Requirements

If you only want to use the library in your own platformio project, then you only need the following:
* [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/)

The following are needed if you want to run this project's tests or to re-generate the test runner file(s):
* [make](https://www.gnu.org/software/make/)
* [nodejs](https://nodejs.org/)
	* On Linux and Mac install node via [nvm](https://github.com/creationix/nvm)


## Installation

To add the lnurl library to your project:
```bash
platformio lib install chill1/lnurl
```


## Usage

Encode:
```cpp
#include <lnurl.h>
#include <iostream> // for std::cout

const std::string input = "https://service.com/api?q=3fc3645b439ce8e7f2553a69e5267081d96dcd340693afabe04be7b0ccd178df";
const std::string output = Lnurl::encode(input);

// Print the result.
std::cout << output << std::endl;
```

Decode:
```cpp
#include <lnurl.h>
#include <iostream> // for std::cout

const std::string input = "lnurl1dp68gurn8ghj7um9wfmxjcm99e3k7mf0v9cxj0m385ekvcenxc6r2c35xvukxefcv5mkvv34x5ekzd3ev56nyd3hxqurzepexejxxepnxscrvwfnv9nxzcn9xq6xyefhvgcxxcmyxymnserxfq5fns";
const std::string output = Lnurl::decode(input);

// Print the result.
std::cout << output << std::endl;
```

Create signed (lnurl-withdraw) URL:
```cpp
#include <lnurl.h>
#include <iostream> // for std::cout

// Prepare the config structure.
Lnurl::SignerConfig config;
config.apiKey.id = "5d4aeb462a";
config.apiKey.key = "ef9901bebc801518e7d862c2edaedd3acd86ec132fb3bd5ac0013c9a5ba478db";
config.apiKey.encoding = "hex";
config.callbackUrl = "https://localhost:3000/lnurl";
config.shorten = true;

// Prepare an instance of the signer class.
Lnurl::Signer signer(config);

// Prepare lnurl-withdraw parameters.
Lnurl::WithdrawParams params;
params.minWithdrawable = "50000";
params.maxWithdrawable = "50000";
params.defaultDescription = "";

// Optionally add your own custom parameters:
// params.custom["custom1"] = "custom parameter";
// params.custom["custom2"] = "another custom parameter";

// Generate a nonce.
// (Pseudo-)randomly generated number and a timestamp should be sufficient.
const std::string nonce = "this_should_be_unique_for_each_signed_url";

// Create the signed URL.
const std::string output = signer.create_url(params, nonce);

// Print the result.
std::cout << output << std::endl;
```

See the [test](https://github.com/chill117/lnurl-platformio/tree/master/test) or [examples](https://github.com/chill117/lnurl-platformio/tree/master/examples) directories for more example usage.


## Tests

To run the automated tests:
```bash
make test ENV=esp32dev DEVICE=/dev/ttyUSB0
```
It is necessary to connect a hardware device via USB.


### Generated Tests

The tests relating to the Lnurl::Signer class are partially auto-generated using a script. The reasoning for this is to ensure compatibility with the [lnurl-node](https://github.com/chill117/lnurl-node) project.

To re-generate the test runner file(s):
```bash
make generateTests
```


## Changelog

See [CHANGELOG.md](https://github.com/chill117/lnurl-platformio/blob/master/CHANGELOG.md)


## License

This project is licensed under the [GNU General Public License v3 (GPL-3)](https://tldrlegal.com/license/gnu-general-public-license-v3-(gpl-3)):
> You may copy, distribute and modify the software as long as you track changes/dates in source files. Any modifications to or software including (via compiler) GPL-licensed code must also be made available under the GPL along with build & install instructions.
