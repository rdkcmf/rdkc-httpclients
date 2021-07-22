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
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <locale>
#include <iostream>

typedef unsigned char u8;		/**< UNSIGNED  8-bit data type */
typedef unsigned short u16;		/**< UNSIGNED 16-bit data type */
typedef unsigned int u32;		/**< UNSIGNED 32-bit data type */
typedef unsigned long long u64; /**< UNSIGNED 64-bit data type */
typedef signed char s8;			/**<   SIGNED  8-bit data type */
typedef signed short s16;		/**<   SIGNED 16-bit data type */
typedef signed int s32;			/**<   SIGNED 32-bit data type */
typedef signed long long s64;

using namespace std;

#define MAX_BUFFER_SIZE 1024 // The maximum length of a line in config files should not exceed 1024
#define FILE_SIZE 8192
#define SIZE 512

class FileUtils
{
	void filePutContents(const string &name, const string &content, bool append);
	void createConfig(const string &filename);
	int readFileContent(const string &filename, char *fileContent);
	int replaceValue(const string &filename, const char *name, const char *value);
	/**
    *	returns the string in the type of T
    **/
	template <typename T>
	T convertStrToType(const std::string &input) const;
	/**
    *return string of type T
    **/
	template <typename T>
	std::string convertToString(const T input) const;

	bool read();
	std::pair<std::string, std::string> parseLine(const std::string &line) const;
	std::string m_filename;
	std::map<std::string, std::string> m_data;
	const std::locale m_locale;

  public:
	FileUtils();
	~FileUtils();
	bool exists(const string &filename);
	bool getConfiguration(const string &filename, const char *name, char *value);
	int setConfiguration(const string &filename, const char *name, const char *value);
	void writeContentToFile(const string &filename, const char *fileContent);

	bool loadFromFile(const std::string &filename);
	template <typename T>
	void get(const std::string &key, T &value) const;
	void print() const;
        int  filesize(const string &filename);
};

template <typename T>
inline std::string FileUtils::convertToString(T input) const
{
	throw "Unsupported type supplied, either change types, or write a correct conversion function for the template type.";
}

template <>
inline std::string FileUtils::convertToString<std::string>(std::string value) const
{
	return value;
}

template <>
inline std::string FileUtils::convertToString<const char *>(const char *value) const
{
	return std::string(value);
}

template <>
inline std::string FileUtils::convertToString<bool>(bool value) const
{
	return (value) ? "TRUE" : "FALSE";
}

template <>
inline std::string FileUtils::convertToString<char>(char value) const
{
	std::string tmp = "";
	tmp = value;
	return tmp;
}

template <>
inline std::string FileUtils::convertToString<int>(int value) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template <>
inline std::string FileUtils::convertToString<float>(float value) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template <>
inline std::string FileUtils::convertToString<short>(short value) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template <>
inline std::string FileUtils::convertToString<double>(double value) const
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template <typename T>
inline T FileUtils::convertStrToType(const std::string &input) const
{
	throw "Unconvertable type encountered, please use a different type, or define the handle case in SettingsParser.hpp";
}

template <>
inline int FileUtils::convertStrToType<int>(const std::string &input) const
{
	int value;
	std::stringstream ss(input);
	ss >> value;
	return value;
}

template <>
inline unsigned int FileUtils::convertStrToType<unsigned int>(const std::string &input) const
{
	unsigned int value;
	std::stringstream ss(input);
	ss >> value;
	return value;
}

template <>
inline unsigned short FileUtils::convertStrToType<unsigned short>(const std::string &input) const
{
	unsigned short value;
	std::stringstream ss(input);
	ss >> value;
	return value;
}

template <>
inline unsigned char FileUtils::convertStrToType<unsigned char>(const std::string &input) const
{
	unsigned char value;
	std::stringstream ss(input);
	ss >> value;
	return value;
}

template <>
inline double FileUtils::convertStrToType<double>(const std::string &input) const
{
	double value;
	std::stringstream ss(input);
	ss >> value;

	return value;
}

template <>
inline float FileUtils::convertStrToType<float>(const std::string &input) const
{
	float value;
	std::stringstream ss(input);
	ss >> value;

	return value;
}

template <>
inline short FileUtils::convertStrToType<short>(const std::string &input) const
{
	short value;
	std::stringstream ss(input);
	ss >> value;

	return value;
}

template <>
inline bool FileUtils::convertStrToType<bool>(const std::string &input) const
{
	return input == "TRUE" ? true : false;
}

template <>
inline char FileUtils::convertStrToType<char>(const std::string &input) const
{
	return input[0];
}

template <>
inline std::string FileUtils::convertStrToType<std::string>(const std::string &input) const
{
	return input;
}

template <typename T>
inline void FileUtils::get(const std::string &key, T &value) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = m_data.find(key);

	if (it != m_data.end())
	{
		value = convertStrToType<T>(it->second);
	}
}
#endif
