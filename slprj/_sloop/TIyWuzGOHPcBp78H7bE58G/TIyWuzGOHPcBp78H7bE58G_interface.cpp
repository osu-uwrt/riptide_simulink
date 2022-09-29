/* Interface file for out-of-process execution of library:
 * TIyWuzGOHPcBp78H7bE58G
 */

#include "xil_interface.h"
#include "xil_data_stream.h"

#include "TIyWuzGOHPcBp78H7bE58G_interface.h"

#include <stdlib.h>

/* Function Init/Term */
void customcode_TIyWuzGOHPcBp78H7bE58G_initializer(void)
{
}

void customcode_TIyWuzGOHPcBp78H7bE58G_terminator(void)
{
}

/* Function isDebug */
boolean_T customcode_TIyWuzGOHPcBp78H7bE58G_isdebug(void)
{
   return true;
}



XIL_INTERFACE_ERROR_CODE xilInitTargetData()
{
    return XIL_INTERFACE_SUCCESS;
}



XIL_INTERFACE_ERROR_CODE xilGetHostToTargetData(uint32_T xilFcnId, XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData **xilIOData)
{
    UNUSED_PARAMETER(xilFcnId);
    UNUSED_PARAMETER(xilCommandType);
    UNUSED_PARAMETER(xilCommandIdx);
    UNUSED_PARAMETER(xilIOData);

    return XIL_INTERFACE_UNKNOWN_TID;
}

XIL_INTERFACE_ERROR_CODE xilOutput(uint32_T xilFcnId, uint32_T xilTID)
{
    UNUSED_PARAMETER(xilTID);

    static uint32_T sizeData = (uint32_T) sizeof(uint32_T);
    static uint32_T sizeScopeID = (uint32_T) sizeof(uint8_T);

    switch (xilFcnId) {
    case 0:
    {
        uint32_T dataWidth_in0 = 0;
        xilReadData((MemUnit_T *) &dataWidth_in0, sizeData);
        uint8_T scopeID_in0 = 0;
        xilReadData((MemUnit_T *) &scopeID_in0, sizeScopeID);
        real_T in0 = 0;
        xilReadData((MemUnit_T *) &in0, (uint32_T) sizeof(real_T));

        uint32_T dataWidth_in1 = 0;
        xilReadData((MemUnit_T *) &dataWidth_in1, sizeData);
        uint8_T scopeID_in1 = 0;
        xilReadData((MemUnit_T *) &scopeID_in1, sizeScopeID);
        real_T in1 = 0;
        xilReadData((MemUnit_T *) &in1, (uint32_T) sizeof(real_T));

        uint32_T dataWidth_in2 = 0;
        xilReadData((MemUnit_T *) &dataWidth_in2, sizeData);
        uint8_T scopeID_in2 = 0;
        xilReadData((MemUnit_T *) &scopeID_in2, sizeScopeID);
        real_T *in2 = (real_T *) calloc((size_t) dataWidth_in2, sizeof(real_T));
        if (scopeID_in2 < 2) {
            xilReadData((MemUnit_T *) in2, dataWidth_in2 * ((uint32_T) sizeof(real_T)));
        }



        add(in0, in1, in2);



        MemUnit_T responseId = XIL_RESPONSE_OUTPUT_DATA;
        if (xilWriteData(&responseId, (uint32_T) sizeof(MemUnit_T)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }

        if (scopeID_in2 > 0) {
            if (xilWriteData((MemUnit_T *) in2, dataWidth_in2 * ((uint32_T) sizeof(real_T))) != XIL_DATA_STREAM_SUCCESS) {
                return XIL_INTERFACE_COMMS_FAILURE;
            }
        }

        free(in2);

        break;
    }

    case 1:
    {


        customcode_TIyWuzGOHPcBp78H7bE58G_initializer();



        MemUnit_T responseId = XIL_RESPONSE_OUTPUT_DATA;
        if (xilWriteData(&responseId, (uint32_T) sizeof(MemUnit_T)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }


        break;
    }

    case 2:
    {


        customcode_TIyWuzGOHPcBp78H7bE58G_terminator();



        MemUnit_T responseId = XIL_RESPONSE_OUTPUT_DATA;
        if (xilWriteData(&responseId, (uint32_T) sizeof(MemUnit_T)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }


        break;
    }

    case 3:
    {
        uint32_T dataWidth_isAllowToDebug = 0;
        xilReadData((MemUnit_T *) &dataWidth_isAllowToDebug, sizeData);
        uint8_T scopeID_isAllowToDebug = 0;
        xilReadData((MemUnit_T *) &scopeID_isAllowToDebug, sizeScopeID);
        boolean_T isAllowToDebug = 0;



        isAllowToDebug = customcode_TIyWuzGOHPcBp78H7bE58G_isdebug();



        MemUnit_T responseId = XIL_RESPONSE_OUTPUT_DATA;
        if (xilWriteData(&responseId, (uint32_T) sizeof(MemUnit_T)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }

        if (xilWriteData((MemUnit_T *) &isAllowToDebug, (uint32_T) sizeof(boolean_T)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }


        break;
    }

    default:
        return XIL_INTERFACE_UNKNOWN_FCNID;
    }

    return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilGetTargetToHostData(uint32_T xilFcnId, XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData **xilIOData, MemUnit_T responseId, uint32_T serverFcnId)
{
    UNUSED_PARAMETER(xilFcnId);
    UNUSED_PARAMETER(xilCommandType);
    UNUSED_PARAMETER(xilCommandIdx);
    UNUSED_PARAMETER(xilIOData);
    UNUSED_PARAMETER(responseId);
    UNUSED_PARAMETER(serverFcnId);

    return XIL_INTERFACE_UNKNOWN_TID;
}

XIL_INTERFACE_ERROR_CODE xilGetTargetToHostPreData(uint32_T xilFcnId, XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData **xilIOData, MemUnit_T responseId, uint32_T serverFcnId)
{
    UNUSED_PARAMETER(xilFcnId);
    UNUSED_PARAMETER(xilCommandType);
    UNUSED_PARAMETER(xilCommandIdx);
    UNUSED_PARAMETER(xilIOData);
    UNUSED_PARAMETER(responseId);
    UNUSED_PARAMETER(serverFcnId);

    return XIL_INTERFACE_UNKNOWN_TID;
}

