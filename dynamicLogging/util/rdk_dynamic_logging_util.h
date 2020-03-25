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
#ifndef __RDK_DYNAMIC_LOGGING_UTIL__
#define __RDK_DYNAMIC_LOGGING_UTIL__

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "rdk_debug.h"
#include "rtConnection.h"
#include "rtLog.h"
#include "rtMessage.h"


#define RDKC_SUCCESS			0
#define RDKC_FAILURE			-1
#define DYNAMIC_LOG_UTIL_ADDRESS 	"tcp://127.0.0.1:10001"
#define DYNAMIC_LOG_REQ_RES_TOPIC	"RDKC.ENABLE_DYNAMIC_LOG"	

/* Utility for RDK Dynamic logging */
class RdkDynamicLogging {
	public:
		RdkDynamicLogging();
		~RdkDynamicLogging();

		int initialize(std::string app);

	private:
		static void* rtMessageDynamicLogReceive(void *args);
		static void dynLogOnMessage(rtMessageHeader const* hdr, uint8_t const* buff, uint32_t n, void* closure);

		pthread_t rdkDynamicLogThread;
		static rtConnection connection;
		static rtError err;

};

#endif //__RDK_DYNAMIC_LOGGING_UTIL__
