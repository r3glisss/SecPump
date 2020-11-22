/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __METAL_HEADER_SIFIVE_CCACHE0__H
#define __METAL_HEADER_SIFIVE_CCACHE0__H

#include <device.h>

#include <regex>

class sifive_ccache0 : public Device {
public:
  sifive_ccache0(std::ostream &os, const fdt &dtb);
  void include_headers();
  void create_defines();
};

#endif
