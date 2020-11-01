#ifndef GCM_H
#define GCM_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmsis_os.h"

extern osMessageQId injectorQueueHandle;
extern uint8_t injectorQueueBuffer[ 64 * sizeof( uint8_t ) ];
extern osStaticMessageQDef_t injectorQueueControlBlock;
extern osMessageQId injectorQueueHandle;

extern osMessageQId cgmQueueHandle;
extern uint8_t cgmQueueBuffer[ 64 * sizeof( uint16_t ) ];
extern osStaticMessageQDef_t cgmQueueControlBlock;

extern osMutexId manualModeKeeperHandle;
extern osStaticMutexDef_t manualModeKeeperControlBlock;

bool auto_mode;

void InitialGlucoseActivity();
void MealGlucoseActivity();
void ConsumInsulin();

#endif