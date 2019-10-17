#ifndef __DMCLIUTILS_H_
#define __DMCLIUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#define SYSTEM_COMMAND_SHELL_NOT_FOUND 127
#define SYSTEM_COMMAND_ERROR -1
#define RDKC_FAILURE    -1
#define RDKC_SUCCESS    0
#define SIZE 512

class dmcliUtils
{
public:
	dmcliUtils();
	~dmcliUtils();
	int dmcliSet(const std::string &paramName, const std::string &paramValue);
};
#endif
