#  lnurl-platformio - https://github.com/chill117/lnurl-platformio
#
#  Copyright (C) 2020 Charles Hill
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Usage
#
#   $ make test             # run test(s)
#   $ make generateTests    # re-generate test runner file(s)
#

## Variables
SCRIPTS=./scripts
TESTS=./test

## Variables with default values:
ENV ?= esp32dev
DEVICE ?= /dev/ttyUSB0

## Targets
#
# The format goes:
#
#   target: list of dependencies
#     commands to build target
#
# If something isn't re-compiling double-check the changed file is in the
# target's dependencies list.

## Phony targets - these are for when the target-side of a definition
# (such as "test" below) isn't a file but instead a just label. Declaring
# it as phony ensures that it always run, even if a file by the same name
# exists.
.PHONY: test\
generateTests

test:
	platformio test \
		--environment ${ENV} \
		--upload-port ${DEVICE} \
		--test-port ${DEVICE}

generateTests:
	node $(SCRIPTS)/generate-tests.js "$(TESTS)/templates/main.cpp" "$(TESTS)/unit/main.cpp"
