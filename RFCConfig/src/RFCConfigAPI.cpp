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
#include "RFCConfigAPI.h"
#include "RFCConfig.h"

/*
#ifdef __cplusplus
extern "C" {
#endif
*/
RFCConfig *rfc = NULL;

/** @description: Initialise RFC Config
 *  @param: void
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfigInit()
{
        if(rfc) {
                return RDKC_SUCCESS;
        } else {
                rfc = new RFCConfig();
                if(!rfc) {
                        return RDKC_FAILURE;
                }
                return RDKC_SUCCESS;
        }

}

/** @description: Get parameter value
 *  @param: file - RFC file name
 *  @param: name - parameter name
 *  @param: value - parameter value
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int GetValueFromRFCFile(char* file, char* name, char* value)
{
        int ret = RDKC_FAILURE;
        if( NULL != rfc ) {
                ret = (rfc -> GetValueFromRFCFile(file,name, value));
        }
        return ret;
}
/** @description: Get parameter value
 *  @param: file - RFC file name
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int IsRFCFileAvailable( char* file )
{
        int ret = RDKC_FAILURE;
        if( NULL != rfc ) {
                ret = (rfc -> IsRFCFileAvailable( file ));
        }
        return ret;
}

/** @description: Get parameter value
 *  @param: void
 *  @return: void
 */
void RFCRelease()
{
        if( NULL != rfc ) {
                delete rfc;
                rfc = NULL;
        }
}
/*
#ifdef __cplusplus
}
#endif
*/

