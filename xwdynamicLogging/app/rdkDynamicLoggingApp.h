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
#ifndef __RDK_DYNAMIC_LOGGING_APP__
#define __RDK_DYNAMIC_LOGGING_APP__

#include <stdio.h>
#include <stdlib.h>

#include "rtConnection.h"
#include "rtLog.h"
#include "rtMessage.h"
#include "rdk_debug.h"

#define RDKC_SUCCESS 0
#define RDKC_FAILURE -1

#define APP_ENABLE_DYNAMIC_LOG "APP_ENABLE_DYNAMIC_LOG"
#define APP_ENABLE_DYNAMIC_LOG_ADDRESS "tcp://127.0.0.1:10001"

/* Class for enabling dynamic logging */
class EnableLogging {

	public:
		EnableLogging();
		~EnableLogging();
		int initializeMessage();
		int sendMessage(char* module, char* logLevel);

	private:
		rtConnection connection;
		rtMessage req;
		rtMessage res;
		rtError err;
};

#endif //__RDK_DYNAMIC_LOGGING_APP__
