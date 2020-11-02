# lnurl-platformio

Implementation of [lnurl](https://github.com/btcontract/lnurl-rfc) for [PlatformIO](https://platformio.org/).

* [Installation](#installation)
* [Usage](#usage)
* [Tests](#tests)
* [Changelog](#changelog)
* [License](#license)


## Installation

To add the lnurl library to your project:
```bash
platformio lib install chill1/lnurl
```


## Usage

See the [test](https://github.com/chill117/lnurl-platformio/tree/master/test) directory for example usage.


## Tests

To run the automated tests:
```bash
platformio test \
	--environment esp32dev \
	--upload-port /dev/ttyUSB0 \
	--test-port /dev/ttyUSB0
```
It is necessary to connect a hardware via USB.


## Changelog

See [CHANGELOG.md](https://github.com/chill117/lnurl-platformio/blob/master/CHANGELOG.md)


## License

This project is licensed under the [GNU General Public License v3 (GPL-3)](https://tldrlegal.com/license/gnu-general-public-license-v3-(gpl-3)):
> You may copy, distribute and modify the software as long as you track changes/dates in source files. Any modifications to or software including (via compiler) GPL-licensed code must also be made available under the GPL along with build & install instructions.
