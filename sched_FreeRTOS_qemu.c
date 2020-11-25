/*==================================================================================================

  sched_FreeRTOS_qemu.c

  Used hardware: STM32F4-Discovery evaluation board (QEMU)

  Example of using the FreeRTOS with the ESFree scheduler on the STM32F4-Discovery board.

  ==================================================================================================
*/

#include "stm32f4_discovery.h"
#include "scheduler.h"
#include "diag/Trace.h"


TaskHandle_t xHandle1 = NULL, xHandle2 = NULL, xHandle3 = NULL;

typedef struct{
	Led_TypeDef led;
	int job;
}job_led;

job_led j_led3, j_led5, j_led6;

void vBlink(void *pvParam){
  static int tick=0;

  job_led *job_led_t=(job_led *) pvParam;

  trace_printf("%s, job:%d, tick:%d \n", pcTaskGetName(NULL), (*job_led_t).job++, tick++);
  BSP_LED_Toggle((*job_led_t).led);
} // vBlink


void main(void){
  __HAL_RCC_GPIOD_CLK_ENABLE();
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  j_led3.led=LED3;
  j_led3.job=1;

  j_led5.led=LED5;
  j_led5.job=1;

  j_led6.led=LED6;
  j_led6.job=1;

  vSchedulerInit();

  vSchedulerPeriodicTaskCreate(vBlink, "Task1", configMINIMAL_STACK_SIZE, &j_led3, 1, &xHandle1, pdMS_TO_TICKS(000), pdMS_TO_TICKS(250), pdMS_TO_TICKS(001), pdMS_TO_TICKS(250));
  vSchedulerPeriodicTaskCreate(vBlink, "Task2", configMINIMAL_STACK_SIZE, &j_led5, 1, &xHandle2, pdMS_TO_TICKS(250), pdMS_TO_TICKS(250), pdMS_TO_TICKS(001), pdMS_TO_TICKS(250));
  vSchedulerPeriodicTaskCreate(vBlink, "Task3", configMINIMAL_STACK_SIZE, &j_led6, 1, &xHandle3, pdMS_TO_TICKS(125), pdMS_TO_TICKS(500), pdMS_TO_TICKS(001), pdMS_TO_TICKS(500));

  vSchedulerStart();
  while(1);
} // main
