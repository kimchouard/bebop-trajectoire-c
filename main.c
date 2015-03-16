
/**
 * @file main.c
 * @brief This file is the core of our project. It contains a demo of libraries we implemented.
 * @date 11/03/2015
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include "bebopDroneNetwork.h"
 #include "main.h"

/*****************************************
 *
 *             define :
 *
 *****************************************/

#define TAG "BebopDroneSimulation"

/*****************************************
 *
 *             implementation :
 *
 *****************************************/


int main (int argc, char *argv[])
{

    ARSAL_PRINT (ARSAL_PRINT_INFO, TAG, "-- Starting --");


	pid_t child = 0;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- Bebop Drone Receive Video Stream --");
    
    // fork the process to launch ffplay
    if ((child = fork()) == 0)
    {
        execlp("mplayer", "mplayer", "video_fifo", NULL);
        ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Missing mplayer, you will not see the video. Please install mplayer.");
        return -1;
    }

    //init network
    BD_MANAGER_t *deviceManager = (BD_MANAGER_t *)initNetwork();

    // Run for 2 minutes
    sleep(20);

/**
    //stop network
    stopNetwork(deviceManager, child);
**/

    if (deviceManager != NULL)
    {
        deviceManager->run = 0; // break threads loops

        
        if (deviceManager->readerThreads != NULL)
        {
            // Stop reader Threads
            int readerThreadIndex = 0;
            for (readerThreadIndex = 0 ; readerThreadIndex < numOfCommandBufferIds ; readerThreadIndex++)
            {
                if (deviceManager->readerThreads[readerThreadIndex] != NULL)
                {
                    ARSAL_Thread_Join(deviceManager->readerThreads[readerThreadIndex], NULL);
                    ARSAL_Thread_Destroy(&(deviceManager->readerThreads[readerThreadIndex]));
                    deviceManager->readerThreads[readerThreadIndex] = NULL;
                }
            }
            
            // free reader thread array
            free (deviceManager->readerThreads);
            deviceManager->readerThreads = NULL;
        }
        
        if (deviceManager->readerThreadsData != NULL)
        {
            // free reader thread data array
            free (deviceManager->readerThreadsData);
            deviceManager->readerThreadsData = NULL;
        }

        stopVideo (deviceManager);
        stopNetwork (deviceManager);
        fclose (deviceManager->video_out);
        free (deviceManager);
    }

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- END --");

    if (child > 0)
    {
        kill(child, SIGKILL);
    }

    return 0;
}