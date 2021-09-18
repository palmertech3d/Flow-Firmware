# @Author: Nick Steele
# @Date:   13:38 Sep 05 2021
# @Last modified by:   Nick Steele
# @Last modified time: 23:29 Sep 08 2021

HARWARE_ENV = megaatmega2560
TEST_ENV = test

build:
	pio run

upload: up

up:
	pio run -t upload

test: FORCE
	pio run  -e $(TEST_ENV) -t upload

FORCE:
