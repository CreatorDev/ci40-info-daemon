/*
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

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <mtd/mtd-user.h>

#include <awa/common.h>
#include <awa/client.h>

#include "Log.h"
#include "DeviceInfo.h"

FILE* g_debugStream = NULL;
int g_debugLevel = LOG_INFO;
sem_t g_debugSemapthore;
bool g_running;

static void ExitApp(int ignore) {
    printf("Exiting...\n");
    g_running = false;
}


static void GetSerialNumber(char* buffer, int bufferSize) {
  FILE *fp;
  char tmpBuf[1035];

  fp = popen("iw dev|grep addr", "r");
  if (fp == NULL) {
    printf("Failed to obtain serial number.\n" );
    exit(1);
  }


  fgets(tmpBuf, sizeof(tmpBuf)-1, fp);
  const char *ptr = strchr(tmpBuf, 'r');
  if (ptr != NULL) {
      int index = ptr - tmpBuf;
      index += 2;   //skip 'r' and space
      int len = strlen(tmpBuf);
      for(;index < len; index++) {
          char c = tmpBuf[index];
          if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
              *buffer = c;
              buffer++;
              bufferSize--;
              if (bufferSize == 1) {
                  break;
              }
          }
      }
      *buffer = 0;
  }
  pclose(fp);
}

static void LoadDeviceData() {

    struct utsname unameData;
    uname(&unameData);
    snprintf(g_DeviceObj.softwareVersion, DEVICE_MAX_BUFF, "%s: %s", unameData.release, unameData.version);
    GetSerialNumber(g_DeviceObj.serialNumber, DEVICE_MAX_BUFF);

    strcpy(g_DeviceObj.manufacturer, "Imagination");
    strcpy(g_DeviceObj.deviceType, "Creator Ci40");
    strcpy(g_DeviceObj.modelNumber, "Ci40");    //TODO: is it ok?
    strcpy(g_DeviceObj.hardwareVersion, "TODO: from where?");
    strcpy(g_DeviceObj.firmwareVersion, "TODO: from where?");
    g_DeviceObj.errorCode = 0;
}

int main(int argc, char **argv) {
    sem_init(&g_debugSemapthore, 0, 1);
    signal(SIGINT, ExitApp);
    g_running = true;

    AwaClientSession * session = AwaClientSession_New();
    AwaClientSession_Connect(session);
    LoadDeviceData();
    InitDevice(session);

    while (g_running) {
        AwaClientSession_Process(session, OPERATION_PERFORM_TIMEOUT);
        AwaClientSession_DispatchCallbacks(session);
        UpdateDeviceInfo(session);
        sleep(1);
    }

    AwaClientSession_Disconnect(session);
    AwaClientSession_Free(&session);
    return 0;
}
