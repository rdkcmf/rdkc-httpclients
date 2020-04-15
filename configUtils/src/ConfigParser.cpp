#include "ConfigParser.h"

#include <locale>
#include <fstream>
#include <iostream>
#include <sstream>


ConfigParser::ConfigParser()
{

}


ConfigParser::~ConfigParser()
{
}


bool ConfigParser::loadFromFile(const std::string& filename)
{
    m_data.clear();
    m_filename = filename;
    return read();
}

bool ConfigParser::read()
{
    std::ifstream in(m_filename.c_str());
    if(!in.is_open())
    {
        std::cerr << "Error: Unable to open settings file \"" << m_filename << "\" for reading!" << std::endl;
        return false;
    }

    std::string line;
    while(std::getline(in, line))
    {
        // parse line
        std::pair<std::string, std::string> keyValuePair = parseLine(line);

        if(!keyValuePair.first.empty())
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
std::pair<std::string, std::string> ConfigParser::parseLine(const std::string &line) const
{
    if(line.size() > 0 && line[0] != '#')
    {
        size_t index = 0;
        // trim leading whitespace
        while(std::isspace(line[index], m_locale))
            index++;
        // get the key string
        const size_t beginKeyString = index;
        while(!std::isspace(line[index], m_locale) && line[index] != '=')
            index++;
        const std::string key = line.substr(beginKeyString, index - beginKeyString);

        // skip the assignment
        while(std::isspace(line[index], m_locale) || line[index] == '=')
            index++;

        // get the value string
        const std::string value = line.substr(index, line.size() - index);

        // return the key value pair
        return std::make_pair(key, value);
    }

    // if this line is emtpy or a comment, return an empty pair
    return std::make_pair(std::string(), std::string());
}


void ConfigParser::print() const
{
  for (std::map<std::string,std::string>::const_iterator it=m_data.begin(); it!=m_data.end(); ++it)
    std::cout << it->first << " = " << it->second << '\n';
  #if 0
    for(auto& element: m_data)
        std::cout << element.first << " = " << element.second<< std::endl;
    
    std::cout << std::endl << "Size: " << m_data.size() << std::endl;
  #endif
}

