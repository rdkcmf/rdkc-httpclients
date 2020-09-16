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
#include "rdk_dynamic_logging_util.h"

rtConnection RdkDynamicLogging::connection;
rtError RdkDynamicLogging::err;

/** @description: Constructor
 *  @param[in] void
 *  @return: void
 */
RdkDynamicLogging::RdkDynamicLogging()
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","(%s):%d rdkDynamicLogging Constructor called.\n", __FUNCTION__, __LINE__);
}

/** @description: Destructor
 *  @param[in] void
 *  @return:void
 */
RdkDynamicLogging::~RdkDynamicLogging()
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","(%s):%d rdkDynamicLogging Destructor called.\n", __FUNCTION__, __LINE__);
}

/** @description: Init dynamic logging
 *  @param[in] appName : application name
 *  @return: RDKC_SUCCESS on success, otherwise RDKC_FAILURE
 */
int RdkDynamicLogging::initialize(std::string appName)
{
	std::string thread_name = appName + "dynamicLogThread";
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","(%s):%d rdkDynamicLogging thread name %s.\n", __FUNCTION__, __LINE__, thread_name.c_str());
	
	rtLog_SetLevel(RT_LOG_INFO);
        rtLog_SetOption(rdkLog);

        rtConnection_Create(&connection, appName.c_str(), DYNAMIC_LOG_UTIL_ADDRESS);
        rtConnection_AddListener(connection, DYNAMIC_LOG_REQ_RES_TOPIC, dynLogOnMessage, connection);

	if( RDKC_SUCCESS != pthread_create(&rdkDynamicLogThread, NULL, &RdkDynamicLogging::rtMessageDynamicLogReceive, this))
        {
          RDK_LOG( RDK_LOG_ERROR,"LOG.RDK.DYNAMICLOG","%s(%d): can't create thread. \n", __FILE__, __LINE__ );
          return RDKC_FAILURE;
        }

	pthread_setname_np(rdkDynamicLogThread, thread_name.c_str());

	return RDKC_SUCCESS;
}

/** @description:Dispatch the message received
 *  @param[in] args : void pointer
 *  @return: void pointer
 */
void* RdkDynamicLogging::rtMessageDynamicLogReceive(void *args)
{
	while (1) {
    		rtError err = rtConnection_Dispatch(connection);
    		if (err != RT_OK) {
      			RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.DYNAMICLOG","%s(%d): Dispatch Error: %s", __FILE__, __LINE__, rtStrError(err));
		}
  	}
}

/** @description:Callback function for the message received
 *  @param[in] hdr : pointer to rtMessage Header
 *  @param[in] buff : buffer for data received via rt message
 *  @param[in] n : number of bytes received
 *  @return: void pointer
 */
void RdkDynamicLogging::dynLogOnMessage(rtMessageHeader const* hdr, uint8_t const* buff, uint32_t n, void* closure)
{
        char const*  module = NULL;
        char const*  logLevel = NULL;

        rtConnection con = (rtConnection) closure;

        rtMessage req;
        rtMessage_FromBytes(&req, buff, n);

	//Handle the rtmessage request
        if (rtMessageHeader_IsRequest(hdr))
        {
                char* buff = NULL;
                uint32_t buff_length = 0;

                rtMessage_ToString(req, &buff, &buff_length);
                rtLog_Info("Req : %.*s", buff_length, buff);
                free(buff);

                rtMessage_GetString(req, "module", &module);
                rtMessage_GetString(req, "logLevel", &logLevel);

                RDK_LOG(RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","(%s):%d Module name: %s\n", __FUNCTION__, __LINE__, module);
                RDK_LOG(RDK_LOG_INFO,"LOG.RDK.DYNAMICLOG","(%s):%d log level: %s\n", __FUNCTION__, __LINE__, logLevel);

                RDK_LOG_ControlCB(module, NULL, logLevel, 1);

                // create response
                rtMessage res;
                rtMessage_Create(&res);
                rtMessage_SetString(res, "reply", "Success");
                rtConnection_SendResponse(con, hdr, res, 1000);
                rtMessage_Release(res);
        }
        rtMessage_Release(req);

}



