/***************************************************************************************************
 * Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated group companies
 * and/or licensors
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <sys/statvfs.h>

#include "DeviceInfo.h"
#include "Log.h"

#define LWM2M_MAX_ID (65535)

typedef enum {
    MandatoryEnum_OPTIONAL = 0,
    MandatoryEnum_MANDATORY = 1,
} MandatoryEnum;

typedef enum {
    MultipleInstancesEnum_SINGLE = 1,
    MultipleInstancesEnum_MULTIPLE = LWM2M_MAX_ID,
} MultipleInstancesEnum;

DeviceObject g_DeviceObj;

static void RebootExecuteCallback(const AwaExecuteArguments* arguments, void * context) {
    const char* userData = (const char*) context;
    LOG(LOG_WARN, "Request for reboot from context: %s", userData);
    system("reboot");
    exit(0);
}

static void FactoryRestetExecuteCallback(const AwaExecuteArguments* arguments, void * context) {
    const char* userData = (const char*) context;
    LOG(LOG_WARN, "Request for Factory Reset from context: %s", userData);
    LOG(LOG_ERR, "At this moment it just do this same as reset, add implementation you need here!");
    system("reboot");
    exit(0);
}

static void OnTimeChangedCallback(const AwaChangeSet* changeSet, void* context) {
    LOG(LOG_INFO, "Requested time change.\n");
    const AwaTime* tvalue;
    AwaChangeSet_GetValueAsTimePointer(changeSet, CURRENT_TIME, &tvalue);
    LOG(LOG_INFO, "Value of resource %s changed to: %ld\n", CURRENT_TIME, (long int) tvalue);
    time_t newTime = (time_t)tvalue;
    stime(&newTime);
}

static void DefineDeviceObject(AwaClientSession* session) {
    AwaObjectDefinition * objectDefinition = AwaObjectDefinition_New(atoi(DEVICE_OBJECT_ID), "Device", 1, 1);

    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 0, "Manufacturer", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 1, "ModelNumber", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 2, "SerialNumber", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 3, "FirmwareVersion", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsNoType(objectDefinition, 4, "Reboot", MandatoryEnum_MANDATORY,
            AwaResourceOperations_Execute);
    AwaObjectDefinition_AddResourceDefinitionAsNoType(objectDefinition, 5, "FactoryReset", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_Execute);
    AwaObjectDefinition_AddResourceDefinitionAsIntegerArray(objectDefinition, 6, "AvailablePowerSources", 0,
            LWM2M_MAX_ID, AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsIntegerArray(objectDefinition, 7, "PowerSourceVoltage", 0, LWM2M_MAX_ID,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsIntegerArray(objectDefinition, 8, "PowerSourceCurrent", 0, LWM2M_MAX_ID,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsInteger(objectDefinition, 9, "BatteryLevel", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, 0);
    AwaObjectDefinition_AddResourceDefinitionAsInteger(objectDefinition, 10, "MemoryFree", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, 0);
    AwaObjectDefinition_AddResourceDefinitionAsIntegerArray(objectDefinition, 11, "ErrorCode", MandatoryEnum_MANDATORY,
            LWM2M_MAX_ID, AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsNoType(objectDefinition, 12, "ResetErrorCode", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_Execute);
    AwaObjectDefinition_AddResourceDefinitionAsTime(objectDefinition, 13, "CurrentTime", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadWrite, 0);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 14, "UTCOffset", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadWrite, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 15, "Timezone", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadWrite, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 16, "SupportedBindingandModes",
            MandatoryEnum_MANDATORY, AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 17, "DeviceType", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 18, "HardwareVersion", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 19, "SoftwareVersion", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, NULL);
    AwaObjectDefinition_AddResourceDefinitionAsInteger(objectDefinition, 20, "BatteryStatus", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, 0);
    AwaObjectDefinition_AddResourceDefinitionAsInteger(objectDefinition, 21, "MemoryTotal", MandatoryEnum_OPTIONAL,
            AwaResourceOperations_ReadOnly, 0);

    AwaClientDefineOperation* operation = AwaClientDefineOperation_New(session);
    AwaClientDefineOperation_Add(operation, objectDefinition);
    AwaClientDefineOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientDefineOperation_Free(&operation);
}

static void UpdateVolatileResources(AwaClientSetOperation* operation) {
    //Time related
    time_t t = time(NULL);
    struct tm lt = { 0 };
    localtime_r(&t, &lt);
    char str[DEVICE_MAX_BUFF];
    sprintf(str, "%lds", lt.tm_gmtoff);

    AwaClientSetOperation_AddValueAsTime(operation, CURRENT_TIME, time(&t));
    AwaClientSetOperation_AddValueAsCString(operation, TIMEZONE, lt.tm_zone);
    AwaClientSetOperation_AddValueAsCString(operation, UTCOFFSET, str);

    //Error code
    AwaIntegerArray* errorcodeArray = AwaIntegerArray_New();
    AwaIntegerArray_SetValue(errorcodeArray, 0, g_DeviceObj.errorCode);
    AwaClientSetOperation_AddValueAsIntegerArray(operation, ERROR_CODE, errorcodeArray);
    AwaIntegerArray_Free(&errorcodeArray);

    struct statvfs data;
    int ret = statvfs("/dev/root", &data);
    if (ret == 0) {
        uint64_t bytes = data.f_bsize * data.f_bfree;
        AwaClientSetOperation_AddValueAsInteger(operation, MEMORY_FREE, bytes);
    }
}

static void UpdateAllResources(AwaClientSetOperation* operation) {
    AwaClientSetOperation_AddValueAsCString(operation, MANUFACTURER, g_DeviceObj.manufacturer);
    AwaClientSetOperation_AddValueAsCString(operation, DEVICE_TYPE, g_DeviceObj.deviceType);
    AwaClientSetOperation_AddValueAsCString(operation, MODEL_NUMBER, g_DeviceObj.modelNumber);
    AwaClientSetOperation_AddValueAsCString(operation, SERIAL_NUMBER, g_DeviceObj.serialNumber);
    AwaClientSetOperation_AddValueAsCString(operation, HARDWARE_VERSION, g_DeviceObj.hardwareVersion);
    AwaClientSetOperation_AddValueAsCString(operation, FIRMWARE_VERSION, g_DeviceObj.firmwareVersion);
    AwaClientSetOperation_AddValueAsCString(operation, SOFTWARE_VERSION, g_DeviceObj.softwareVersion);
    UpdateVolatileResources(operation);
}

static void SubscribeToChanges(AwaClientSession* session) {
    AwaClientSubscribeOperation* subscribeOperation = AwaClientSubscribeOperation_New(session);

    //Execution callback
    AwaClientExecuteSubscription* subscription = AwaClientExecuteSubscription_New(REBOOT, RebootExecuteCallback, NULL);
    AwaClientSubscribeOperation_AddExecuteSubscription(subscribeOperation, subscription);

    subscription = AwaClientExecuteSubscription_New(FACTORY_RESET, FactoryRestetExecuteCallback, NULL);
    AwaClientSubscribeOperation_AddExecuteSubscription(subscribeOperation, subscription);

    //Changes callback
    AwaClientChangeSubscription* changeSub = AwaClientChangeSubscription_New(CURRENT_TIME, OnTimeChangedCallback, NULL);
    AwaClientSubscribeOperation_AddChangeSubscription(subscribeOperation, changeSub);

    AwaClientSubscribeOperation_Perform(subscribeOperation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSubscribeOperation_Free(&subscribeOperation);
}

static void CreateDeviceInstance(AwaClientSession* session) {
    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);

    AwaClientSetOperation_CreateObjectInstance(operation, OBJECT_INSTANCE(DEVICE_OBJECT_ID, 0));
    AwaClientSetOperation_CreateOptionalResource(operation, MANUFACTURER);
    AwaClientSetOperation_CreateOptionalResource(operation, DEVICE_TYPE);
    AwaClientSetOperation_CreateOptionalResource(operation, MODEL_NUMBER);
    AwaClientSetOperation_CreateOptionalResource(operation, SERIAL_NUMBER);
    AwaClientSetOperation_CreateOptionalResource(operation, HARDWARE_VERSION);
    AwaClientSetOperation_CreateOptionalResource(operation, FIRMWARE_VERSION);
    AwaClientSetOperation_CreateOptionalResource(operation, SOFTWARE_VERSION);
    AwaClientSetOperation_CreateOptionalResource(operation, CURRENT_TIME);
    AwaClientSetOperation_CreateOptionalResource(operation, REBOOT);
    AwaClientSetOperation_CreateOptionalResource(operation, UTCOFFSET);
    AwaClientSetOperation_CreateOptionalResource(operation, TIMEZONE);
    AwaClientSetOperation_CreateOptionalResource(operation, ERROR_CODE);
    AwaClientSetOperation_CreateOptionalResource(operation, SUPPORTED_BINGING_AND_MODES);
    AwaClientSetOperation_CreateOptionalResource(operation, RESET_ERROR_CODE);
    AwaClientSetOperation_CreateOptionalResource(operation, MEMORY_FREE);
    AwaClientSetOperation_CreateOptionalResource(operation, FACTORY_RESET);

    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);
}

void InitDevice(AwaClientSession* session) {
    DefineDeviceObject(session);
    CreateDeviceInstance(session);

    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);
    UpdateAllResources(operation);
    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);

    SubscribeToChanges(session);
}

void UpdateDeviceInfo(AwaClientSession* session) {

    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);
    UpdateVolatileResources(operation);
    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);
}
