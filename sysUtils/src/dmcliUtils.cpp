#include "dmcliUtils.h"

dmcliUtils::dmcliUtils()
{
    /* Constructor */
}

dmcliUtils::~dmcliUtils()
{
    /* Destructor */
}

/**
 * @brief Execute dmcli set command
 * @param paramName, tr181 parameter name
 * @param paramValue, value to be set for the parameter
 * @return 0 success 1 failure.
 */
int dmcliUtils::dmcliSet(const std::string &paramName, const std::string &paramValue) 
{
        char dmcliSetQuery[SIZE] = {'\0'};
        memset(dmcliSetQuery, 0 , sizeof(dmcliSetQuery));
        snprintf(dmcliSetQuery, sizeof(dmcliSetQuery), "dmcli -s %s=%s &", paramName.c_str(), paramValue.c_str());

        int ret = system(dmcliSetQuery);

        if ((WEXITSTATUS(ret) == SYSTEM_COMMAND_SHELL_NOT_FOUND) || (WEXITSTATUS(ret) == SYSTEM_COMMAND_ERROR))
        {
                return RDKC_FAILURE;
        }

        return RDKC_SUCCESS;
}
