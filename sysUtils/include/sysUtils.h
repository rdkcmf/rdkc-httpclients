/**
##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2019 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
**/
#ifndef __SYS_UTILS_H_
#define __SYS_UTILS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* string data type */
struct result_string
{
  char*  ptr;
  size_t len;
};

#define RDKC_FAILURE    	-1
#define RDKC_SUCCESS    	0
#define FW_NAME_MAX_LENGTH      256
#define CAM_MAC_MAX_LENGTH	512
#define VER_NUM_MAX_LENGTH	128

typedef struct result_string  result_string_t;

int getCameraFirmwareVersion(char *fw_version);

int init_result_string(result_string_t *str);

int exec_sys_command(char* cmd, result_string_t* output);

int readValues(FILE *pFile, char *pToken, char *data);

int getConfValue(char* conf_file, char* conf_param, char* conf_value);

int setConfValue(char* conf_file, char* conf_param, char* conf_value);

int isConfigValueChanged(char* conf_file, char* attribute,const char* value);

int checknetwork();

int getDeviceMacValue(char *device_mac);

int getCameraVersionNum(char* version_num);

int getDefaultGatewayAndIface(in_addr_t * addr, char *interface);

char* getDefaultGateway();
#endif
