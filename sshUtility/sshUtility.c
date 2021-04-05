/*
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
*/
#include "rtConnection.h"
#include "rtLog.h"
#include "rtMessage.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rdk_debug.h"

void usage()
{
	RDK_LOG( RDK_LOG_INFO,"LOG.RDK.RTMESSAGE", "sshUtility <<commandToExecute>>\n");
}

int main(int argc, char* argv[])
{
	rdk_logger_init("/etc/debug.ini");
	if(argc <2)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.RTMESSAGE","Insufficient arguments\n");
		usage();
		return -1;
	}

	rtError err;
	rtLog_SetLevel(RT_LOG_DEBUG);

	rtConnection con;
	rtConnection_Create(&con, "SSHAPP", "tcp://127.0.0.1:10001");
	//con->client =1;

	rtMessage res=NULL;
	rtMessage req=NULL;
	rtMessage_Create(&req);
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.RTMESSAGE","command to execute - %s\n",  argv[1]);
	rtMessage_SetString(req, "command", argv[1]);

	err = rtConnection_SendRequest(con, req, "RDK.MODEL.SSHUTILITY", &res, 2000);
	 RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.RTMESSAGE","SendRequest:%s\n", rtStrError(err));

	if (err == RT_OK)
	{
		char* p = NULL;
		char* output = NULL;
		uint32_t len = 0;

		rtMessage_ToString(res, &p, &len);
		RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.RTMESSAGE","\tres:%.*s\n", len, p);
		rtMessage_GetString(res, "output", &output);
		printf("output:%s\n", output);

		free(p);
	}
	else
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.RTMESSAGE","Command execution failure\n");

	rtMessage_Release(req);
	if(res != NULL)
		rtMessage_Release(res);

	sleep(1);

	rtConnection_Destroy(con);
	return 0;
}
