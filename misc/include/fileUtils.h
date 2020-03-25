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
#ifndef __FILEUTILS_H_
#define __FILEUTILS_H_

// Creating and writing to a file - C++
#include <sys/stat.h>
#include <fstream>
#include <regex>

using namespace std;

#define MAX_BUFFER_SIZE 1024 // The maximum length of a line in config files should not exceed 1024
#define FILE_SIZE 8192
#define SIZE 512

class FileUtils
{
	void filePutContents(const string& name, const string& content, bool append);
	void createConfig(const string& filename);
	int readFileContent(const string& filename, char* fileContent);
	int replaceValue(const string& filename, const char* name, const char* value);
public:
	FileUtils();
	~FileUtils();
	bool exists (const string& filename);
	bool getConfiguration(const string& filename, const char* name, char* value);
	void setConfiguration(const string& filename, const char* name, const char* value);
	void writeContentToFile(const string& filename, const char* fileContent);
};
#endif
