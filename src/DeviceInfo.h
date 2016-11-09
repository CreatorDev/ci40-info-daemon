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

#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__

#include <awa/common.h>
#include <awa/client.h>

#define DEVICE_OBJECT_ID            "3"
#define MANUFACTURER                "/3/0/0"
#define MODEL_NUMBER                "/3/0/1"
#define SERIAL_NUMBER               "/3/0/2"
#define FIRMWARE_VERSION            "/3/0/3"
#define REBOOT                      "/3/0/4"
#define FACTORY_RESET               "/3/0/5"
#define AVAILABLE_POWER_SOURCES     "/3/0/6"
#define POWER_SOURCE_VOLTAGE        "/3/0/7"
#define POWER_SOURCE_CURRENT        "/3/0/8"
#define BATTERY_LEVEL               "/3/0/9"
#define MEMORY_FREE                 "/3/0/10"
#define ERROR_CODE                  "/3/0/11"
#define RESET_ERROR_CODE            "/3/0/12"
#define CURRENT_TIME                "/3/0/13"
#define UTCOFFSET                   "/3/0/14"
#define TIMEZONE                    "/3/0/15"
#define SUPPORTED_BINGING_AND_MODES "/3/0/16"
#define DEVICE_TYPE                 "/3/0/17"
#define HARDWARE_VERSION            "/3/0/18"
#define SOFTWARE_VERSION            "/3/0/19"

#define OBJECT_INSTANCE(obj, inst) "/" obj "/" #inst

typedef enum{
  INSTANCE_0 = 0,
  INSTANCE_1,
  INSTANCE_2,
  INSTANCE_3,
} InstanceId;

#define DEVICE_MAX_BUFF             100
#define OPERATION_PERFORM_TIMEOUT   30

typedef enum {
    PowerSource_DC_POWER,
    PowerSource_INTERNAL_BATTERY,
    PowerSource_EXTERNAL_BATTERY,
    PowerSource_POWER_OVER_ETHERNET,
    PowerSource_USB,
    PowerSource_AC_MAINS_POWER,
    PowerSource_SOLAR
} PowerSource;

#define MAX_POWER_SOURCES 4

typedef struct{
    char manufacturer[DEVICE_MAX_BUFF];
    char deviceType[DEVICE_MAX_BUFF];
    char modelNumber[DEVICE_MAX_BUFF];
    char serialNumber[DEVICE_MAX_BUFF];
    char hardwareVersion[DEVICE_MAX_BUFF];
    char firmwareVersion[DEVICE_MAX_BUFF];
    char softwareVersion[DEVICE_MAX_BUFF];
    int errorCode;
    int availPowerSources;
    int powerSourceVoltage[MAX_POWER_SOURCES];
    int powerSourceCurrent[MAX_POWER_SOURCES];
    int batteryLevel;
    PowerSource powerSources[MAX_POWER_SOURCES];
} DeviceObject;

extern DeviceObject g_DeviceObj;


void InitDevice(AwaClientSession* session);
void UpdateDeviceInfo(AwaClientSession* session);

#endif /* __DEVICE_INFO_H__ */
