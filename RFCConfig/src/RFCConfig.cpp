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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include "RFCConfig.h"

RFCConfig::RFCConfig()
{
        /* Constructor */
}

RFCConfig::~RFCConfig()
{
        /* Destructor */
}

/** @description: Parse the RFC file
 *  @param: file - RFC file name
 *  @param: field_name - parameter name
 *  @param: value - parameter value
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfig::Parse(const char* file, const char* field_name, char* value) {
    char ipName[MAX_SIZE] = {0};

    std::ifstream fin(file);
    std::string line;
    std::istringstream sin;

    while (std::getline(fin, line)) {
        sin.str(line.substr(line.find("=")+1));
        if (line.find(field_name) != std::string::npos) {
                sin >> ipName;
                strcpy(value, ipName);
                sin.clear();
                return RDKC_SUCCESS;
        }
        sin.clear();
    }
    return RDKC_FAILURE;
}

/** @description: Serch RFC configuration file in /opt/RFC path
 *  @param: file - RFC file name
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfig::Search(const char* file)
{
    char dirPath[MAX_SIZE] = {0};

    DIR    *RFCDir = NULL;
    struct dirent *RFCConfigFile = NULL;
    char   filename[MAX_SIZE];
    char   *period = NULL;

    strcpy(dirPath,RFC_PATH);
    /*open the directory where the user config file exists*/
    if ((RFCDir = opendir (dirPath)) != NULL) {
        while ((RFCConfigFile = readdir (RFCDir)) != NULL) {

            strcpy(filename,RFCConfigFile->d_name);

            /*Ignore invalid files*/
            if((period=strchr(filename,DELIMITTER))==NULL)
                continue;

            if( strcmp( filename,file) == 0) {
                if(NULL != RFCDir)
                {
                    closedir(RFCDir);
                    RFCDir=NULL;
                }
                return RDKC_SUCCESS;
            }
        }
    }
    else {

        RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.RFCCONFIG","%s(%d):Unable to open directory.\n",__FUNCTION__, __LINE__);
        return RDKC_FAILURE;
    }
    RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.RFCCONFIG","%s(%d): File not available \n",__FUNCTION__, __LINE__);
    if(NULL != RFCDir)
    {
        closedir(RFCDir);
        RFCDir=NULL;
    }
    return RDKC_FAILURE;
}

/** @description: Read .ini file
 *  @param: file - RFC file name
 *  @param: name - parameter name
 *  @param: value - parameter value
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfig::ReadConfig(char* file, char* name, char* value)
{
    char fullFileName[MAX_SIZE] = {0};

    if ( RDKC_FAILURE == Search(file) ) {
        return RDKC_FAILURE;
    }
    sprintf(fullFileName,"%s/%s",RFC_PATH,file);
    if( RDKC_FAILURE == Parse(fullFileName,name,value) ) {
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}

/** @description: Get parameter value
 *  @param: file - RFC file name
 *  @param: name - parameter name
 *  @param: value - parameter value
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfig::GetValueFromRFCFile( char* file, char* name, char* value)
{
    if( RDKC_FAILURE == ReadConfig(file,name,value) ) {
        RDK_LOG( RDK_LOG_DEBUG,"LOG.RDK.RFCCONFIG","%s(%d): Unable to read from config file. \n", __FUNCTION__, __LINE__);
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}

/** @description: Check if RFC files are available or not
 *  @param: file - RFC file name
 *  @return: RDKC_SUCCESS on success, RDKC_FAILURE on failure
 */
int RFCConfig::IsRFCFileAvailable( char* file )
{
    if( RDKC_FAILURE == Search(file) ) {
        return RDKC_FAILURE;
    }
    return RDKC_SUCCESS;
}


