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
#ifndef __RFCCONFIGPARSE_H_
#define __RFCCONFIGPARSE_H_

#include <stdio.h>
#include "rdk_debug.h"
#include "RFCCommon.h"

class RFCConfig
{
public:
	RFCConfig();
	~RFCConfig();
	int GetValueFromRFCFile(char* file, char* name, char* value);
	int IsRFCFileAvailable(char* file);

private:
	int Parse(const char* file, const char* field_name, char* value);
	int Search(const char* file);
	int ReadConfig(char* file, char* name, char* value);
};
#endif
