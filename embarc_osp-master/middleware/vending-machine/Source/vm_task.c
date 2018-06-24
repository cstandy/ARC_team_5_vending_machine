/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "vm_task.h"

extern SemaphoreHandle_t xSemaphore;

void enQueue(vm_data data)
{
    while(1) {
        if( xQueueSend(xVMQueue, &data, 0) == pdTRUE )
            break;
        vTaskDelay(1);
    }
}


void vm_task(void *p_arg)
{
    EMBARC_PRINTF("entering vm task\r\n");
    vm_data data = {0};

    // xVMQueue, xOledQueue, xCommunicationQueue, xTempQueue, xNumPadQueue, xDCmotorQueue
    // all tasks push the data to xVMQueue

    while (1)
    {
        if (xQueueReceive( xVMQueue, &data, 0 ) == pdTRUE)
        {
            switch (data.target_id)
            {
                case id_main:
                    while (1)
                        if (xQueueSend(xMainQueue, &data, 0) == pdTRUE)
                            break;
                    break;
                case id_wifi:
                    while (1)
                        if (xQueueSend(xCommunicationQueue, &data, 0) == pdTRUE)
                            break;
                    break;
                case id_dcmotor:
                    while (1)
                        if (xQueueSend(xDCmotorQueue, &data, 0) == pdTRUE)
                            break;
                    break;
                case id_oled:
                    while (1)
                        if (xQueueSend(xOledQueue, &data, 0) == pdTRUE)
                            break;
                    break;
            }
        }
    }
}
