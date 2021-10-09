# @Author: Nick Steele
# @Date:   13:38 Sep 05 2021
# @Last modified by:   Nick Steele
# @Last modified time: 20:01 Oct 08 2021

HARWARE_ENV = megaatmega2560
TEST_ENV = test

build: generate
	pio run

upload: up

up: generate
	pio run -t upload

test: generate FORCE
	pio run  -e $(TEST_ENV) -t upload

generate:
	python ./src/tabular/generated/gen-tabular.py './src/tabular/generated/'

FORCE:
