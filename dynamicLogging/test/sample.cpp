/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/
#include  "rdk_dynamic_logging_util.h"

int main(int argc, char **argv)
{
	rdk_logger_init("/etc/debug.ini");

	RdkDynamicLogging rdkDynamicLoggingInst;
	rdkDynamicLoggingInst.initialize("SAMPLE_APP");

	while (1) {
		//Logging Module can be arbitrary
		RDK_LOG(RDK_LOG_INFO,"LOG.RDK.OVERLAY","(%s):%d Testing dynamic log, log level set to info.\n", __FUNCTION__, __LINE__);
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.OVERLAY","(%s):%d Testing dynamic log, log level set to debug.\n", __FUNCTION__, __LINE__);
		sleep(1);
	}
}
