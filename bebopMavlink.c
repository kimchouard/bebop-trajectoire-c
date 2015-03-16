
#include <stdlib.h>
#include <stdio.h>

#include <libARSAL/ARSAL.h>
#include <libARSAL/ARSAL_Print.h>
#include <libARMAVLINK/libARMavlink.h>

#include "bebopMavlink.h"

#define TAG "BebopDroneMavLink"

int testMavlink ()
{
	int failed = 0;
    const char *const mavlinkFilePath = "test.mv";
	eARMAVLINK_ERROR filegenError = ARMAVLINK_OK;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- Begin Mavlink Tests --");

	ARMAVLINK_FileGenerator_t* fileGen = ARMAVLINK_FileGenerator_New(&filegenError);

	if (filegenError != ARMAVLINK_OK)
	{
		failed = 1;
	}

	if (!failed)
	{
		ARMAVLINK_FileGenerator_CreateMavlinkFile(fileGen, mavlinkFilePath);
	}

	if (!failed)
	{
    	ARMAVLINK_FileGenerator_Delete(&fileGen);
	}


    return failed;
}