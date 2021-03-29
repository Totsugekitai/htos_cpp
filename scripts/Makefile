### Makefile_template ###

# don't use TAB
.RECIPEPREFIX = >
# change shell to bash
SHELL := bash
# shell flags
.SHELLFLAGS := -eu -o pipefail -c
# one shell for one target rule
.ONESHELL:
# warning undefined variables
MAKEFLAGS += --warn-undefined-variables
# delete intermediate files on error
.DELETE_ON_ERROR:
# delete implicit rules
MAKEFLAGS += -r

# MAKEFILE_DIR is directory Makefile located in
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

### Makefile_template end ###

.PHONY: install-stdlib
install-stdlib: Makefile
> export HTOS_THIRD_PARTY=$(MAKEFILE_DIR)/../third_party
> ./install_newlib.sh
> ./install_libcxxabi.sh
> ./install_libcxx.sh
