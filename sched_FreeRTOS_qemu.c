/*==================================================================================================

  sched_FreeRTOS_qemu.c

  Used hardware: STM32F4-Discovery evaluation board (QEMU)

  Example of using the FreeRTOS with the ESFree scheduler on the STM32F4-Discovery board.

  ==================================================================================================
*/

#include "stm32f4_discovery.h"
#include "scheduler.h"
#include "diag/Trace.h"
#include "semphr.h"

void meu_delay(int ticks);

TaskHandle_t xHandle1 = NULL, xHandle2 = NULL, xHandle3 = NULL, xHandle4 = NULL;

SemaphoreHandle_t xMutex1 = NULL, xMutex2 = NULL;

void vTask1(void *pvParam){

	trace_printf("%s, inicio no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

	meu_delay(1);

	trace_printf("%s, tentativa de acesso ao Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	if(xSemaphoreTake(xMutex1, 0)==pdTRUE){
		//entrando na seção crítica 1
		trace_printf("%s, obteve o Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		meu_delay(1);
		xSemaphoreGive(xMutex1);
		trace_printf("%s, liberou o Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		//saindo na seção crítica 1
	}
	else{
		trace_printf("%s, acesso ao Mt1 bloqueado no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	}

	trace_printf("%s, tentativa de acesso ao Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	if(xSemaphoreTake(xMutex2, 0)==pdTRUE){
		//entrando na seção crítica 2
		trace_printf("%s, obteve o Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		meu_delay(1);
		xSemaphoreGive(xMutex2);
		trace_printf("%s, liberou o Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		//saindo na seção crítica 2
	}
	else{
		trace_printf("%s, acesso ao Mt2 bloqueado no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	}

	meu_delay(1);

	trace_printf("%s, fim no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

}

void vTask2(void *pvParam){

	trace_printf("%s, inicio no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

	meu_delay(1);

	trace_printf("%s, tentativa de acesso ao Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	if(xSemaphoreTake(xMutex2, 0)==pdTRUE){
		//entrando na seção crítica
		trace_printf("%s, obteve o Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		meu_delay(2);
		xSemaphoreGive(xMutex2);
		trace_printf("%s, liberou o Mt2 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		//saindo na seção crítica
	}
	else{
		trace_printf("%s, acesso ao Mt2 bloqueado no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	}

	meu_delay(1);

	trace_printf("%s, fim no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

}

void vTask3(void *pvParam){

	trace_printf("%s, inicio no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

	meu_delay(1);

	trace_printf("%s, fim no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

}

void vTask4(void *pvParam){

	trace_printf("%s, inicio no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

	meu_delay(1);

	trace_printf("%s, tentativa de acesso ao Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	if(xSemaphoreTake(xMutex1, 0)==pdTRUE){
		//entrando na seção crítica
		trace_printf("%s, obteve o Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		meu_delay(4);
		xSemaphoreGive(xMutex1);
		trace_printf("%s, liberou o Mt1 no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
		//saindo na seção crítica
	}
	else{
			trace_printf("%s, acesso ao Mt1 bloqueado no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());
	}

	meu_delay(1);

	trace_printf("%s, fim no tick:%d \n", pcTaskGetName(NULL), xTaskGetTickCount());

}

void vPrioInfo(void *pvParam){
  trace_printf("Priorities:\n%s: %d\n", pcTaskGetName(xHandle1), uxTaskPriorityGet(xHandle1));
  trace_printf("%s: %d\n", pcTaskGetName(xHandle2), uxTaskPriorityGet(xHandle2));
  trace_printf("%s: %d\n\n", pcTaskGetName(xHandle3), uxTaskPriorityGet(xHandle3));
} // vPrioInfo

void main(void){
  __HAL_RCC_GPIOD_CLK_ENABLE();

  xMutex1 = xSemaphoreCreateMutex();
  xMutex2 = xSemaphoreCreateMutex();


  vSchedulerPeriodicTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 4, &xHandle1, pdMS_TO_TICKS(16), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(8), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 3, &xHandle2, pdMS_TO_TICKS(14), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(11), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, 2, &xHandle3, pdMS_TO_TICKS(14), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(12), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vTask4, "Task 4", configMINIMAL_STACK_SIZE, NULL, 1, &xHandle4, pdMS_TO_TICKS(12), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(15), pdMS_TO_TICKS(1000));

  vSchedulerStart();
  while(1);

} // main

void meu_delay(int ticks){
	int delay=ticks*16000;
	for(int i=0;i<delay;i++);
}

