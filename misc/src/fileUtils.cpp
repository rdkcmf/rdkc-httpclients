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
#include "fileUtils.h"

FileUtils::FileUtils()
{
    /* Constructor */
}

FileUtils::~FileUtils()
{
    /* Destructor */
}

void FileUtils::filePutContents(const string& name, const string& content, bool append = false) 
{
    ofstream outfile;
    if (append)
        outfile.open(name.c_str(), ios_base::app);
    else
        outfile.open(name.c_str());
    outfile << content;
    outfile.close();
}

void FileUtils::createConfig(const string& filename)
{
    char buffer[MAX_BUFFER_SIZE] = {'\0'};

    filePutContents(filename, buffer, false);
    return;
}

int FileUtils::readFileContent(const string& filename, char* fileContent)
{
    ifstream in(filename.c_str());
    string line, strValue;
    int ret = -1;

    if(in)
    {
        in.seekg(0, std::ios::end);
        size_t len = in.tellg();
        in.seekg(0);
        std::string contents(len + 1, '\0');
        in.read(&contents[0], len);
        const char* c = contents.c_str();
        strcpy(fileContent, c);
        ret = 0;
    }
    return ret;
}

void FileUtils::writeContentToFile(const string& filename, const char* fileContent)
{
    fstream ofs;
    ofs.open(filename.c_str(), ios::out | ios::trunc);
    ofs<<fileContent;
    ofs.close(); 
}

int FileUtils::replaceValue(const string& filename, const char* name, const char* value)
{
    char fileContent[FILE_SIZE] = {'\0'};
    if(0 != readFileContent(filename, fileContent))
        return -1;

    string subject(fileContent);
    char nameRegEx[SIZE] = {'\0'}, replaceNameValue[SIZE] = {'\0'};

    sprintf(nameRegEx, "%s=[^\\n]*[\\n]", name);
    sprintf(replaceNameValue, "%s=%s\n", name, value);

    regex re(nameRegEx);

    string result;
    regex_replace (back_inserter(result), subject.begin(), subject.end(), re, replaceNameValue);

    writeContentToFile(filename, result.c_str());
}

bool FileUtils::exists (const string& filename)
{
    struct stat buffer;   
    return (stat (filename.c_str(), &buffer) == 0); 
}

bool FileUtils::getConfiguration(const string& filename, const char* name, char* value)
{
    ifstream fin(filename.c_str());
    string line, strValue;
    bool bFound = false;
    char strNameEq[MAX_BUFFER_SIZE] = {'\0'};

    sprintf(strNameEq, "%s=", name);

    while( getline( fin, line ) ) 
    {
        if( line.find( strNameEq ) != string::npos ) // if found in line then
        {
            strValue = line.substr(line.find("=") + 1) ;            
            
            const char* c = strValue.c_str();
            if ( strlen(c) != 0 ) {
                strncpy(value, c,strlen(c));
                value[strlen(c)] = '\0';
                bFound = true;
            } else {
                strcpy(value, "");
                value[strlen(value)] = '\0';
                bFound = false;
            }
        }
    }
    return bFound;
}

void FileUtils::setConfiguration(const string& filename, const char* name, const char* value)
{
    char prevValue[SIZE] = {'\0'};
    if(!exists(filename))
    {
        createConfig(filename);
    }
    else
    {
        if(true == getConfiguration(filename, name, prevValue))
        {
            if(strcmp(prevValue, value) != 0)
            {
                replaceValue(filename, name, value);
            }
            return;
        }
    }

    if(strcmp(prevValue, "") == 0)
    {
        replaceValue(filename, name, value);
        return;
    }else {
        char buffer[MAX_BUFFER_SIZE] = {'\0'};
        sprintf(buffer, "%s=%s\n",name,value);

        filePutContents(filename, buffer, true);
        return;
   }
    char buffer[MAX_BUFFER_SIZE] = {'\0'};
    sprintf(buffer, "%s=%s\n",name,value);

    filePutContents(filename, buffer, true);
    return;
}
