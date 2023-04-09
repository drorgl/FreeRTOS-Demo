#include "ram.h"

#include "FreeRTOS.h"



#define mainREGION_1_SIZE    8201
#define mainREGION_2_SIZE    40905
#define mainREGION_3_SIZE    50007

void  prvInitialiseHeap( void )
{
/* The Windows demo could create one large heap region, in which case it would
be appropriate to use heap_4.  However, purely for demonstration purposes,
heap_5 is used instead, so start by defining some heap regions.  No
initialisation is required when any other heap implementation is used.  See
http://www.freertos.org/a00111.html for more information.

The xHeapRegions structure requires the regions to be defined in start address
order, so this just creates one big array, then populates the structure with
offsets into the array - with gaps in between and messy alignment just for test
purposes. */
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
HeapStats_t xHeapStats;
const HeapRegion_t xHeapRegions[] =
{
    /* Start address with dummy offsets                       Size */
    { ucHeap + 1,                                             mainREGION_1_SIZE },
    { ucHeap + 15 + mainREGION_1_SIZE,                        mainREGION_2_SIZE },
    { ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,    mainREGION_3_SIZE },
    { NULL, 0 }
};

    /* Sanity check that the sizes and offsets defined actually fit into the
    array. */
    configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

    /* Prevent compiler warnings when configASSERT() is not defined. */
    ( void ) ulAdditionalOffset;

    /* The heap has not been initialised yet so expect stats to all be zero. */
    vPortGetHeapStats( &xHeapStats );

    vPortDefineHeapRegions( xHeapRegions );

    /* Sanity check vTaskGetHeapStats(). */
    // prvExerciseHeapStats();
}