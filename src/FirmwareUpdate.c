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

#include "FirmwareUpdate.h"
#include <string.h>

//info about process: http://www.openmobilealliance.org/release/LightweightM2M/V1_0-20170208-A/OMA-TS-LightweightM2M-V1_0-20170208-A.pdf
//section E.6 LwM2M Object: Firmware Update
//pages 113 to 118

const static char* FIRMWARE_UPDATE_OBJECT_ID = "5";
const static char* PACKAGE = "/5/0/0";
const static char* PACKAGE_URI = "/5/0/1";
const static char* UPDATE = "/5/0/2";
const static char* STATE = "/5/0/3";
const static char* UPDATE_RESULT = "/5/0/5";
const static char* PKG_NAME = "/5/0/6";
const static char* FIRMWARE_UPDATE_PROTOCOL_SUPPORT = "/5/0/8";
const static char* FIRMWARE_UPDATE_DELIVERY_METHOD = "/5/0/9";
const static int OPERATION_PERFORM_TIMEOUT = 30;

typedef enum {
    FirmwareUpdateProtocolSupport_COAP = 0,
    FirmwareUpdateProtocolSupport_COAPS = 1,
    FirmwareUpdateProtocolSupport_HTTP = 2,
    FirmwareUpdateProtocolSupport_HTTPS = 3
} FirmwareUpdateProtocolSupport;

typedef enum {
    FirmwareUpdateDeliveryMethod_PULL_ONLY = 0,
    FirmwareUpdateDeliveryMethod_PUSH_ONLY = 1,
    FirmwareUpdateDeliveryMethod_BOTH = 2
} FirmwareUpdateDeliveryMethod;

typedef enum {
    FirmwareUpdateResult_INIT = 0,
    FirmwareUpdateResult_UPDATE_SUCCESS = 1,
    FirmwareUpdateResult_NOT_ENOUGH_FLASH_MEM = 2,
    FirmwareUpdateResult_OUT_OF_RAM = 3,
    FirmwareUpdateResult_CONNECTION_LOST_WHILE_DOWNLOADING = 4,
    FirmwareUpdateResult_INTEGRITY_CHECK_FAILURE = 5,
    FirmwareUpdateResult_UNSUPPORTED_PACKAGE_TYPE = 6,
    FirmwareUpdateResult_INVALID_URI = 7,
    FirmwareUpdateResult_FIRMWARE_UPDATE_FAILED = 8,
    FirmwareUpdateResult_UNSUPPORTED_PROTOCOL = 9
} FirmwareUpdateResult;

typedef enum {
    FirmwareUpdateState_IDLE = 0,
    FirmwareUpdateState_DOWNLOADING = 1,
    FirmwareUpdateState_UPDATING = 2
} FirmwareUpdateState;

typedef struct{
    char packageUri[255];
    FirmwareUpdateState state;
    FirmwareUpdateResult updateResult;
    FirmwareUpdateProtocolSupport protocolSupport;
    FirmwareUpdateDeliveryMethod deliveryMethod;
} FirmwareUpdateObject;

static FirmwareUpdateObject firmwareObject;

void firmwareUpdate_Init(AwaClientSession* session) {
    memset(&firmwareObject.packageUri, 0, sizeof(firmwareObject.packageUri));
    firmwareObject.state = FirmwareUpdateState_IDLE;
    firmwareObject.updateResult = FirmwareUpdateResult_INIT;
    firmwareObject.protocolSupport = FirmwareUpdateProtocolSupport_HTTPS;
    firmwareObject.deliveryMethod = FirmwareUpdateDeliveryMethod_PULL_ONLY;

    //constant values in IPSO
    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);
    AwaClientSetOperation_AddValueAsInteger(operation, FIRMWARE_UPDATE_PROTOCOL_SUPPORT, firmwareObject.protocolSupport);
    AwaClientSetOperation_AddValueAsInteger(operation, FIRMWARE_UPDATE_DELIVERY_METHOD, firmwareObject.deliveryMethod);
    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);

    //TODO: build subscriptions
}

void firmwareUpdate_Shutdown(AwaClientSession* session) {
    //todo: cleanup subscriptions
}

void firmwareUpdate_OnPackageUrlChange() {

}

void firmwareUpdate_OnUpdateExecuted() {

}

void firmwareUpdate_UpdateInfo(AwaClientSession* session) {
    AwaClientSetOperation* operation = AwaClientSetOperation_New(session);

    AwaClientSetOperation_AddValueAsInteger(operation, STATE, firmwareObject.state);
    AwaClientSetOperation_AddValueAsInteger(operation, UPDATE_RESULT, firmwareObject.updateResult);

    AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    AwaClientSetOperation_Free(&operation);
}
