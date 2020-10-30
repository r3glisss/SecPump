#ifndef INSULIN_CONTROLLER_H
#define INSULIN_CONTROLLER_H

#include "main.h"

/// Controller Function
void InsulinController(uint8_t *GlucoseBuffer);

/// Manual Controller
void InsulinManualController();

/// Reset Controller
void ResetController();

#endif
