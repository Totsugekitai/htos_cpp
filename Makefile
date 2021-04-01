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

.PHONY: run
run: Makefile
> ./scripts/make_image.sh disk.img mnt BOOTX64.EFI kernel/htos.elf
> ./scripts/run_image.sh disk.img

.PHONY: kernel
kernel: Makefile
> make -C kernel/

.PHONY: clean
clean: Makefile
> make -C kernel/ clean
