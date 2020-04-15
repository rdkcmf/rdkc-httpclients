#include <string>
#include <map>
#include <vector>
#include <sstream>
typedef unsigned char           u8;     /**< UNSIGNED  8-bit data type */
typedef unsigned short          u16;    /**< UNSIGNED 16-bit data type */
typedef unsigned int            u32;    /**< UNSIGNED 32-bit data type */
typedef unsigned long long      u64;    /**< UNSIGNED 64-bit data type */
typedef signed char             s8;     /**<   SIGNED  8-bit data type */
typedef signed short            s16;    /**<   SIGNED 16-bit data type */
typedef signed int              s32;    /**<   SIGNED 32-bit data type */
typedef signed long long        s64;
class ConfigParser 
{
public:
    ConfigParser();
    ~ConfigParser();

    bool loadFromFile(const std::string& filename);
    
    template<typename T>
    void get(const std::string& key, T & value) const;
    void print() const;
private:
    /**
    *	returns the string in the type of T
    **/
    template<typename T>
    T convertStrToType(const std::string &input) const;
    /**
    *return string of type T
    **/
    template<typename T>
    std::string convertToString(const T input) const;
    
    bool read();
    std::pair<std::string, std::string> parseLine(const std::string &line) const;
    std::string m_filename;
    std::map<std::string, std::string> m_data;
    const std::locale m_locale;
};

template<typename T>
inline std::string ConfigParser::convertToString(T input) const {
    throw "Unsupported type supplied, either change types, or write a correct conversion function for the template type.";
}

template<>
inline std::string ConfigParser::convertToString<std::string>(std::string value) const {
    return value;
}

template<>
inline std::string ConfigParser::convertToString<const char*>(const char* value) const {
    return std::string(value);
}

template<>
inline std::string ConfigParser::convertToString<bool>(bool value) const {
    return (value) ? "TRUE" : "FALSE";
}

template<>
inline std::string ConfigParser::convertToString<char>(char value) const {
    std::string tmp = "";
    tmp = value;
    return tmp;
}

template<>
inline std::string ConfigParser::convertToString<int>(int value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string ConfigParser::convertToString<float>(float value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string ConfigParser::convertToString<short>(short value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string ConfigParser::convertToString<double>(double value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
inline T ConfigParser::convertStrToType(const std::string &input) const {
    throw "Unconvertable type encountered, please use a different type, or define the handle case in SettingsParser.hpp";
}

template<>
inline int ConfigParser::convertStrToType<int>(const std::string &input) const {
    int value;
    std::stringstream ss(input);
    ss >> value;
    return value;
}

template<>
inline unsigned int ConfigParser::convertStrToType<unsigned int>(const std::string &input) const {
    unsigned int value;
    std::stringstream ss(input);
    ss >> value;
    return value;
}

template<>
inline unsigned short ConfigParser::convertStrToType<unsigned short>(const std::string &input) const {
    unsigned short value;
    std::stringstream ss(input);
    ss >> value;
    return value;
}

template<>
inline unsigned char  ConfigParser::convertStrToType<unsigned char>(const std::string &input) const {
    unsigned char value;
    std::stringstream ss(input);
    ss >> value;
    return value;
}


template<>
inline double ConfigParser::convertStrToType<double>(const std::string &input) const {
    double value;
    std::stringstream ss(input);
    ss >> value;
    
    return value;
}

template<>
inline float ConfigParser::convertStrToType<float>(const std::string &input) const {
    float value;
    std::stringstream ss(input);
    ss >> value;
    
    return value;
}

template<>
inline short ConfigParser::convertStrToType<short>(const std::string &input) const {
    short value;
    std::stringstream ss(input);
    ss >> value;
    
    return value;
}

template<>
inline bool ConfigParser::convertStrToType<bool>(const std::string &input) const {
    return input == "TRUE" ? true : false;
}

template<>
inline char ConfigParser::convertStrToType<char>(const std::string &input) const {
    return input[0];
}

template<>
inline std::string ConfigParser::convertStrToType<std::string>(const std::string &input) const {
    return input;
}

template<typename T>
inline void ConfigParser::get(const std::string& key, T &value) const {
    std::map<std::string, std::string>::const_iterator it;
    it = m_data.find(key);
 
    if (it != m_data.end()){
        value = convertStrToType<T>(it->second);

    }
}
