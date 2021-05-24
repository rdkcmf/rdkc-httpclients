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

void FileUtils::filePutContents(const string &name, const string &content, bool append = false)
{
    ofstream outfile;
    if (append)
        outfile.open(name.c_str(), ios_base::app);
    else
        outfile.open(name.c_str());
    outfile << content;
    outfile.close();
}

void FileUtils::createConfig(const string &filename)
{
    char buffer[MAX_BUFFER_SIZE] = {'\0'};
    filePutContents(filename, buffer, false);
    return;
}

int FileUtils::readFileContent(const string &filename, char *fileContent)
{
    ifstream in(filename.c_str());
    string line, strValue;
    int ret = -1;

    if (in)
    {
        in.seekg(0, std::ios::end);
        size_t len = in.tellg();
        if( 0 == len ) {
            return -1;
        }
        in.seekg(0);
        std::string contents(len + 1, '\0');
        in.read(&contents[0], len);
        const char *c = contents.c_str();
        strcpy(fileContent, c);
        ret = 0;
    }
    return ret;
}

void FileUtils::writeContentToFile(const string &filename, const char *fileContent)
{
    fstream ofs;
    ofs.open(filename.c_str(), ios::out | ios::trunc);
    ofs << fileContent << std::endl;
    ofs.close();
}

int FileUtils::replaceValue(const string &filename, const char *name, const char *value)
{
    char fileContent[FILE_SIZE] = {'\0'};
    if (0 != readFileContent(filename, fileContent)) {
        return -1;
    }

    string subject(fileContent);
    char nameRegEx[SIZE] = {'\0'}, replaceNameValue[SIZE] = {'\0'};

    sprintf(nameRegEx, "%s=[^\\n]*[\\n]", name);
    sprintf(replaceNameValue, "%s=%s\n", name, value);

    regex re(nameRegEx);

    string result;
    regex_replace(back_inserter(result), subject.begin(), subject.end(), re, replaceNameValue);
    writeContentToFile(filename, result.c_str());
    return 0;
}

bool FileUtils::exists(const string &filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

bool FileUtils::getConfiguration(const string &filename, const char *name, char *value)
{
    ifstream fin(filename.c_str());
    string line, strValue;
    bool bFound = false;
    char strNameEq[MAX_BUFFER_SIZE] = {'\0'};

    sprintf(strNameEq, "%s=", name);

    while (getline(fin, line))
    {
        if (line.find(strNameEq) != string::npos) // if found in line then
        {
            strValue = line.substr(line.find("=") + 1);

            const char *c = strValue.c_str();
            if (strlen(c) != 0)
            {
                strncpy(value, c, strlen(c));
                value[strlen(c)] = '\0';
                bFound = true;
            }
            else
            {
                strcpy(value, "");
                value[strlen(value)] = '\0';
                bFound = false;
            }
        }
    }
    return bFound;
}

int FileUtils::setConfiguration(const string &filename, const char *name, const char *value)
{
    char prevValue[SIZE] = {'\0'};
    int ret = -1;
    if (!exists(filename))
    {
        createConfig(filename);
    }
    else
    {
        if (true == getConfiguration(filename, name, prevValue))
        {
            if (strcmp(prevValue, value) != 0)
            {
                ret = replaceValue(filename, name, value);
                if (0 != ret ) {
                    return -1;
                }
            }
            return 0;
        }
    }
    char buffer[MAX_BUFFER_SIZE] = {'\0'};
    sprintf(buffer, "%s=%s\n", name, value);

    filePutContents(filename, buffer, true);
    return 0;
}

bool FileUtils::loadFromFile(const std::string &filename)
{
    m_data.clear();
    m_filename = filename;
    return read();
}

bool FileUtils::read()
{
    std::ifstream in(m_filename.c_str());
    if (!in.is_open())
    {
        std::cerr << "Error: Unable to open settings file \"" << m_filename << "\" for reading!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(in, line))
    {
        // parse line
        std::pair<std::string, std::string> keyValuePair = parseLine(line);

        if (!keyValuePair.first.empty())
        {
            // if the line is not empty or a comment save it to the map
            m_data[keyValuePair.first] = keyValuePair.second;
        }
    }

    in.close();
    return true;
}

/**
 * This method parses a line from our format ("key = value") into a std::pair<std::string, std::string>
 * containing th key and the value.
 * If the line is empty or a comment (starts with a '#') an empty pair is returned.
 */
std::pair<std::string, std::string> FileUtils::parseLine(const std::string &line) const
{
    if (line.size() > 0 && line[0] != '#')
    {
        size_t index = 0;
        // trim leading whitespace
        while (std::isspace(line[index], m_locale))
            index++;
        // get the key string
        const size_t beginKeyString = index;
        while (!std::isspace(line[index], m_locale) && line[index] != '=')
            index++;
        const std::string key = line.substr(beginKeyString, index - beginKeyString);

        // skip the assignment
        while (std::isspace(line[index], m_locale) || line[index] == '=')
            index++;

        // get the value string
        const std::string value = line.substr(index, line.size() - index);

        // return the key value pair
        return std::make_pair(key, value);
    }
    // if this line is emtpy or a comment, return an empty pair
    return std::make_pair(std::string(), std::string());
}

void FileUtils::print() const
{
    for (std::map<std::string, std::string>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
#if 0
    for(auto& element: m_data)
        std::cout << element.first << " = " << element.second<< std::endl;
    
    std::cout << std::endl << "Size: " << m_data.size() << std::endl;
#endif
}

int FileUtils::filesize(const string &filename)
{
    int size = 0;
    FILE* fp = NULL;
    fp = fopen(filename.c_str(),"rb");
    if( NULL != fp ) {
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        return size;
    } else {
        return 0;
    }
}
