#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <timers.h>
#include <queue.h>
#include <stdbool.h>
#include "ram.h"

#define mainQUEUE_RECEIVE_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )
#define mainVALUE_SENT_FROM_TASK            ( 100UL )
#define mainVALUE_SENT_FROM_TIMER            ( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS            pdMS_TO_TICKS( 1000UL )
#define mainQUEUE_LENGTH                    ( 2 )

const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

static QueueHandle_t xQueue = NULL;
static TimerHandle_t xTimer = NULL;

static void prvQueueReceiveTask( void *pvParameters )
{
    printf("Starting Receive Task\r\n");
uint32_t ulReceivedValue;

    /* Prevent the compiler warning about the unused parameter. */
    ( void ) pvParameters;

    bool exitTimer = false;

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h.  It will not use any CPU time while it is in the
        Blocked state. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /* Enter critical section to use printf. Not doing this could potentially cause
        a deadlock if the FreeRTOS simulator switches contexts and another task
        tries to call printf - it should be noted that use of printf within
        the FreeRTOS simulator is unsafe, but used here for simplicity. */
        taskENTER_CRITICAL();
        {
            /* To get here something must have been received from the queue, but
            is it an expected value?  Normally calling printf() from a task is not
            a good idea.  Here there is lots of stack space and only one task is
            using console IO so it is ok.  However, note the comments at the top of
            this file about the risks of making Windows system calls (such as
            console output) from a FreeRTOS task. */
            if( ulReceivedValue == mainVALUE_SENT_FROM_TASK )
            {
                printf( "Message received from task\r\n" );
            }
            else if( ulReceivedValue == mainVALUE_SENT_FROM_TIMER )
            {
                printf( "Message received from software timer\r\n" );
                exitTimer = true;
                
            }
            else
            {
                printf( "Unexpected message\r\n" );
            }

            fflush( stdout );
        }
        taskEXIT_CRITICAL();

        if (exitTimer){
            vTaskEndScheduler();
            return;
        }
    }
}


static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle )
{
const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;

    /* This is the software timer callback function.  The software timer has a
    period of two seconds and is reset each time a key is pressed.  This
    callback function will execute if the timer expires, which will only happen
    if a key is not pressed for two seconds. */

    /* Avoid compiler warnings resulting from the unused parameter. */
    ( void ) xTimerHandle;

    /* Send to the queue - causing the queue receive task to unblock and
    write out a message.  This function is called from the timer/daemon task, so
    must not block.  Hence the block time is set to 0. */
    xQueueSend( xQueue, &ulValueToSend, 0U );
    printf("sending...\r\n");
}



int main( void ){
    printf("Initializing...\r\n");
    prvInitialiseHeap();

    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    xTaskCreate( prvQueueReceiveTask,            /* The function that implements the task. */
                    "Rx",                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
                    NULL,                            /* The parameter passed to the task - not used in this simple case. */
                    mainQUEUE_RECEIVE_TASK_PRIORITY, /* The priority assigned to the task. */
                    NULL );                          /* The task handle is not required, so NULL is passed. */

     /* Create the software timer, but don't start it yet. */
        xTimer = xTimerCreate( "Timer",              /* The text name assigned to the software timer - for debug only as it is not used by the kernel. */
                                xTimerPeriod,        /* The period of the software timer in ticks. */
                                pdTRUE,              /* xAutoReload is set to pdTRUE. */
                                NULL,                /* The timer's ID is not used. */
                                prvQueueSendTimerCallback ); /* The function executed when the timer expires. */

        if( xTimer != NULL )
        {
            xTimerStart( xTimer, 0 );
        }

        printf("Starting Scheduler...\r\n");
        vTaskStartScheduler();
        printf("Before End\r\n");
        // while(1){}

}