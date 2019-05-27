/*
 * cureManager.h
 *
 *  Created on: 10 août 2018
 *      Author: breschc
 */

#ifndef CUREMANAGER_H_
#define CUREMANAGER_H_

#include "main.h"

#define CONV_HOUR(min) min/60;
#define DEB(dose, hour) dose/hour;
#define NB_CURE 2
#define STACK_SIZE 128

typedef struct DrugCure{

	char *name;
	unsigned long minutes;
	unsigned long hours;
	unsigned long volume;

	void (* taskFunc)(void * pvParameters);
	void * xHandle;

	SemaphoreHandle_t xSemaphore;

	struct DrugCure *next;

} DrugCure;
typedef DrugCure *DrugCureList;


void InitDrugCureList(DrugCureList *drugCureList);
int AddCureDrug(DrugCureList *drugCureList, const char *name, const unsigned long hours, const unsigned long minutes, const unsigned long volume);
int ModifyCure(DrugCureList *drugCureList, const char *name, const unsigned long hours, const unsigned long minutes, const unsigned long volume);
int RemoveCure(DrugCureList *drugCureList, const char *name);
void DestroyCureList(DrugCureList *lst);
void LaunchCure(DrugCureList *drugCureList, char *name);
int CheckName(DrugCureList *drugCureList, char *name);
void vCureTaskCode( void * pvParameters );

#endif /* CUREMANAGER_H_ */

