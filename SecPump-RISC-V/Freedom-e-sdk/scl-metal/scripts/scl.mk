#
# Copyright 2020 SiFive, Inc #
# SPDX-License-Identifier: MIT #
#

ifeq ($(SCL_DIR),)
	ERR = $(error Please specify scl-metal directory by using SCL_DIR variable )
endif

# ----------------------------------------------------------------------
# Includes Location
# ----------------------------------------------------------------------
override SCL_INCLUDES := $(SCL_DIR)/include
