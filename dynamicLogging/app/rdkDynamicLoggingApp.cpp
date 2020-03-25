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
#include "rdkDynamicLoggingApp.h"

/** @description: Constructor
 *  @param[in] void
 *  @return: void
 */
EnableLogging::EnableLogging()
{
	RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","%s(%d): EnableLogging constructor called.\n",__FUNCTION__, __LINE__);	
}

/** @description: Destructor
 *  @param[in] void
 *  @return: void
 */
EnableLogging::~EnableLogging()
{
	RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","%s(%d): EnableLogging destructor called.\n",__FUNCTION__, __LINE__);	
}

/** @description: Initializes rtmessage connection
 *  @param[in] : void
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE otherwise
 */
int EnableLogging::initializeMessage()
{
	rtError ret = RT_OK;

        rtLog_SetLevel(RT_LOG_INFO);
        rtLog_SetOption(rdkLog);
        ret = rtConnection_Create(&connection, APP_ENABLE_DYNAMIC_LOG, APP_ENABLE_DYNAMIC_LOG_ADDRESS);

	if(ret != RT_OK) {
		RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): Failed to initialize rtMessage.\n",__FUNCTION__, __LINE__);
		return RDKC_FAILURE;
	}

	RDK_LOG( RDK_LOG_ERROR,"LOG.RDK.DYNAMICLOG","%s(%d): Initialize rtMessage successfully.\n",__FUNCTION__, __LINE__);	
	return RDKC_SUCCESS;
}

/** @description: Send rtmessage
 *  @param[in] module : char pointer to module name
 *  @param[in] logLevel : char logLevel for the module
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE otherwise 
 */
int EnableLogging::sendMessage(char* module, char* logLevel)
{
	rtMessage_Create(&req);
        rtMessage_SetString(req,"module", module);
        rtMessage_SetString(req, "logLevel", logLevel);

	err = rtConnection_SendRequest(connection, req, "RDKC.ENABLE_DYNAMIC_LOG", &res, 2000);
	RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): Send Request:%s\n",__FUNCTION__, __LINE__,rtStrError(err));	

	//Handles the response.
        if (err == RT_OK)
        {
	    char* p = NULL;
	    uint32_t len = 0;
            rtMessage_ToString(res, &p, &len);
            RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d):\tres:%.*s\n", __FUNCTION__, __LINE__,len, p);
            free(p);
        }
	rtMessage_Release(req);
	rtMessage_Release(res);

	return RDKC_SUCCESS;
}

/** @description: help routine
 *  @param[in] : void
 *  @return: void
 */
void help()
{
	RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): USAGE: enableLogging <MODULE> <LOG_LEVEL>\n",  __FUNCTION__, __LINE__ );
	RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): Refer /etc/debug.ini for <MODULE> and <LOG_LEVEL>.\n" ,  __FUNCTION__, __LINE__);
}

/** @description: main
 *  @param[in] argc : no. of command line args
 *  @param[in] argv : pointer to array of command line args
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE otherwise
 */
int main (int argc, char **argv)
{
	char* module = NULL;
	char* loglevel = NULL;
	int ret = RDKC_SUCCESS;

	EnableLogging rdkDynamicLogInst;

	rdk_logger_init ("/etc/debug.ini");

	if(argc < 3) {
		RDK_LOG( RDK_LOG_ERROR,"LOG.RDK.DYNAMICLOG","%s(%d):Insufficient arguments passed\n",  __FUNCTION__, __LINE__);
		help();
		return RDKC_FAILURE;
	}

	if(argv[1]) {
		module = argv[1];
		RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): module name:%s\n", __FUNCTION__, __LINE__, module);
	}

	if(argv[2]) {
		loglevel = argv[2];
		RDK_LOG( RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","%s(%d): loglevel name:%s\n", __FUNCTION__, __LINE__, loglevel);
	}

	ret = rdkDynamicLogInst.initializeMessage();
	if(RDKC_SUCCESS != ret) {
		return RDKC_FAILURE;
	}

	rdkDynamicLogInst.sendMessage(module, loglevel);

	return RDKC_SUCCESS;
}
