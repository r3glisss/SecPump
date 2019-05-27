/*
 * cureManager.c
 *
 *  Created on: 10 août 2018
 *      Author: breschc
 */
#include "cureManager.h"

void InitDrugCureList(DrugCureList *drugCureList)
{
	*drugCureList = 0x00;
}

/*Add a drug in the Library*/
int AddCureDrug(DrugCureList *drugCureList, const char *name, const unsigned long hours,
		const unsigned long minutes, const unsigned long volume)
{
	int cureHandleTable[2] = {0,0};

	DrugCure *x = (DrugCure *) malloc (sizeof(DrugCure));
	DrugCure *z = *drugCureList;


	if(x == 0x00)
	{
		perror("malloc error");
	}

	x->name = (char *) malloc((strlen(name) + 1) * sizeof(char));
	if(x->name == 0x00)
	{
		perror("malloc error");
	}

	x->hours = hours;
	x->minutes = minutes;
	x->volume = volume;
	x->xSemaphore = xSemaphoreCreateMutex();

	strcpy(x->name, name);

	if(z == 0x00)
	{
		x->taskFunc = vCureTaskCodeA;
	}

	while(z != 0x00)
	{
		if(z->taskFunc == vCureTaskCodeA)
		{
			cureHandleTable[0] = 1;
		}
		else if(z->taskFunc == vCureTaskCodeB)
		{
			cureHandleTable[1] = 1;
		}

		z = z->next;
	}

	if(cureHandleTable[0] == 0)
	{
		x->taskFunc = vCureTaskCodeA;
	}

	else if(cureHandleTable[1] == 0)
	{
		x->taskFunc = vCureTaskCodeB;
	}

	else
	{
		exit(0);
	}

	x->next = *drugCureList;
	*drugCureList = x;

	return 1;
}

int ModifyCure(DrugCureList *drugCureList, const char *name,
		const unsigned long hours, const unsigned long minutes, const unsigned long volume)
{

	DrugCure *x = *drugCureList;
	//char * prev;

	while(x != 0x00)
	{
		if( xSemaphoreTake( x->xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
		{
			if (strcmp(x->name, name) == 0)
				break;
			/* Next element */
			xSemaphoreGive(x->xSemaphore);
			x = x->next;
		}
	}

	if(x != 0x00)
	{

		vTaskSuspend(x->xHandle);

		x->hours = hours;
		x->minutes = minutes;
		x->volume = volume;
		//prev = x->name;

		HAL_Delay(5000);

		/*x->name = (char *) malloc((strlen(name) + 1) * sizeof(char));

		if(x->name == 0x00){
			perror("malloc error");
		}

		strcpy(x->name, name);

		free(prev);*/

		vTaskResume(x->xHandle);
		xSemaphoreGive(x->xSemaphore);
		//LaunchCure(drugCureList, name);

	}
	else
	{
		puts("Not Found!\n");
	}

	return 1;
}

int RemoveCure(DrugCureList *drugCureList, const char *name)
{

	DrugCure *prev = 0x00;
	DrugCure *x = *drugCureList;

	while(x != 0x00)
	{
		if( xSemaphoreTake( x->xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
		{
			if (strcmp(x->name, name) == 0)
			            break;
			        /* Next element */
			        prev = x;
			        xSemaphoreGive( x->xSemaphore);
			        x = x->next;
		 }
	}

	if(x != 0x00)
	{
		/*Top of the list*/
		if(prev==0x00)
		{
			*drugCureList = x->next;
		}
		else
		{
			prev->next = x->next;
		}


		vTaskDelete(x->xHandle);
		vSemaphoreDelete(x->xSemaphore);
		free(x->name);
		free(x);

	}
	else
	{
		//ERROR CASE
	}

	return 1;
}

/*Destroy the entire DrugLibrary*/
void DestroyCureList(DrugCureList *lst)
{
    while (*lst != 0x00)
    {
        DrugCure *x = *lst;
        if( xSemaphoreTake( x->xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
        	 *lst = x->next;

        	 vTaskDelete(x->xHandle);
             vSemaphoreDelete( x->xSemaphore );
             free(x->name);
             free(x);
         }
    }
    return;
}

/*Check that the name is in the library*/
int CheckName(DrugCureList *drugCureList, char *name)
{

	DrugCure *x = *drugCureList;

	while(x != NULL)
	{
		if( xSemaphoreTake( x->xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
		{
			if (strcmp(x->name, name) == 0)
		           break;
		    /* Next element */
			xSemaphoreGive(x->xSemaphore);
			x = x->next;
		}
	}

	if(x != NULL)
	{
		xSemaphoreGive(x->xSemaphore);
		return 1;
	}
	else
	{
		return 0;
	}
}

void LaunchCure(DrugCureList *drugCureList, char *name)
{
	DrugCure *x = *drugCureList;
	BaseType_t xReturned;

		while(x != NULL)
		{
			if( xSemaphoreTake( x->xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
					{
						if (strcmp(x->name, name) == 0)
						            break;
						/* Next element */
						x = x->next;
						xSemaphoreGive( x->xSemaphore);
					 }
		}

		if(x != NULL)
		{
			xReturned = xTaskCreate(	  x->taskFunc,     		  /* Function that implements the task. */
			                  	  	  	  name,          		  /* Text name for the task. */
										  STACK_SIZE,  			  /* Stack size in words, not bytes. */
										  ( void * ) x,    		  /* Parameter passed into the task. */
										  tskIDLE_PRIORITY,		  /* Priority at which the task is created. */
										  &(x->xHandle));         /* Used to pass out the created task's handle. */

			if(xReturned == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
			{
				perror("thread create");
				RemoveCure(drugCureList, name);
			}
			xSemaphoreGive( x->xSemaphore);

		}
		else
		{
			perror("Launcher Issue");
		}
}




