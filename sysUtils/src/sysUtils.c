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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <errno.h>
#include <stdint.h>
#include "sysUtils.h"

#define UTILS_ASSERT_NOT_NULL(P)       	if ((P) == NULL) return 1
#define DATA_LEN                       	256
#define NETWORK_CHECK_URL              	"www.google.com"
#define VERSION_FILE 			"/version.txt"
#define BUFFER_SIZE                     4096

int init_result_string(result_string_t *str)
{
  str->ptr = NULL;
  str->len = 0;
  str->ptr = malloc(str->len+1);

  if(!str->ptr){
    return 1;
  }
  str->ptr[0] = '\0';
  return 0;
}

/**
 * @brief Execute system command and get output
 * @param cmd, system command to be executed
 * @outparam output, output obtained by executing system cmd
 * @return 0 success 1 failure.
 */
int exec_sys_command(char* cmd, result_string_t* output)
{
  char buff[DATA_LEN];
  int len;
  FILE *syscmd = popen(cmd, "re");

  if(!syscmd){
    printf("popen failed with error code %d to execute system command %s", syscmd, cmd);
    return 1;
  }
  memset(buff, 0, DATA_LEN);

  if(output != NULL){
    init_result_string(output);
  }
  while(fgets(buff, sizeof(buff), syscmd) != 0){
    if(output != NULL){
      size_t new_len = strlen(buff);      
      output->ptr = realloc(output->ptr, output->len+new_len+1);
      UTILS_ASSERT_NOT_NULL(output->ptr);
      memcpy(output->ptr + output->len, buff, strlen(buff));
      output->len += new_len;
      memset(buff, 0, DATA_LEN);
    }
  }

  if(output != NULL){
    output->ptr[output->len] = '\0';
  }

  pclose(syscmd);
  return 0;
}

/**
 * @brief Read the file for value of a given token
 * @param pFile, file name to read
 * @param pToken, parameter to be read
 * @param data, value read
 * @return 0 on success,otherwise 1 on failure.
 */
int readValues(FILE *pFile, char *pToken, char *data)
{
  char buff[DATA_LEN];
  char *keyValue;

  if (pFile == NULL)
     return 1;

  /* Search the token in the config file and copy the value */
  fseek(pFile, 0, SEEK_SET);
  while(fgets(buff, DATA_LEN, pFile) != NULL)
  {
    keyValue = strtok(buff, "=" );
    if(!(strcmp(keyValue, pToken)))
    {
      keyValue = strtok(NULL, "\n" );
      if(keyValue != NULL)
      {
        strncpy(data, keyValue, strlen(keyValue));
        data[strlen(keyValue)] = '\0'; // Adding '\0' is required if strncpy is used
      }
      else
        printf("Value for %s not found", pToken);
      break;
    }
  }
  return 0;
}

/**
 * @brief Get value of a parameter from config file
 * @param conf_file, Config file name to get parameter value from
 * @param conf_param, Parameter name to get value of
 * @outparam conf_value, value obtained
 * @return 0 success 1 failure.
 */
int getConfValue(char* conf_file, char* conf_param, char* conf_value)
{
  char confvalue[DATA_LEN];
  FILE *fd = NULL;

  if (0 == access(conf_file, F_OK))
  {
    fd = fopen(conf_file, "r");
    if (fd != NULL)
    {
      int ret = readValues(fd, conf_param, confvalue);
      if (confvalue != NULL)
      {
        strncpy(conf_value, confvalue, strlen(confvalue));
        conf_value[strlen(confvalue)] = '\0';
        printf("\n%s : %s\n", conf_param, conf_value);
      }
      else
      {
        printf("%s not found", conf_param);
        return 1;
      }
    }
    else
    {
      printf("Unable to open conf file %s", conf_file);      
    }
    fclose(fd);
  }
  else
  {
    printf("Conf file %s not found", conf_file);
    return 1;
  }
  return 0;
}

/**
 * @brief Write a parameter and its value in config file
 * @param conf_file, Config file name to write parameter value
 * @param conf_param, Parameter name to be written in config file
 * @param conf_value, value to be set for parameter
 * @return 0 success 1 failure.
 */
int setConfValue(char* conf_file, char* conf_param, char* conf_value)
{
  char buffer[DATA_LEN];
  FILE *fp = NULL;

  if (0 == access(conf_file, F_OK))
  {
    fp = fopen(conf_file,"w");

    if(fp != NULL)
    {
      sprintf(buffer, "%s=%s\n ", conf_param, conf_value);
      fputs((const char *) buffer, fp);
    }
    else
    {
      printf("Unable to open %s file", conf_file);
    }

    fclose(fp);
  }
  else
  {
    printf("Conf file %s not found", conf_file);
    return 1;
  }
  return 0;
}

/** @brief check if any conf value changed in given file
  *  @param  Configuration file that contains required parameter
  *  @param  Attribute name whose value is to be checked
  *  @param  New value to be compared with existing value
  *  @return true if conf value changed
  */
int isConfigValueChanged(char* conf_file, char* attribute,const char* value)
{
  int isChanged = 1;
  char data[DATA_LEN] = {'\0'};
  int  isFound = 1;

  int ret = getConfValue(conf_file, attribute, data);
  if(ret)
  {
	printf("Conf file %s not found", conf_file);
	return isChanged;
  }
  if (data != NULL)
  {
    printf("\nExisting Value : %s", data);
    if(strcmp(data, value) != 0)
    {
      isChanged = 0;
      printf("\nConf Value in %s changed from [%s >>> %s].\n", conf_file, data, value);
    }
  }
  return isChanged;
}

/**
 * @brief Check if system is connected to network
 * return 0 on success 1 on failure
 */
int checknetwork()
{
  struct addrinfo hints;
  struct addrinfo *result;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Stream socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */

  int n = getaddrinfo(NETWORK_CHECK_URL, "80", &hints, &result);
  if (n != 0) 
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(n));
    return 1;
  }
  
  int fd;
  struct addrinfo *a;
  for (a = result; a != NULL; a = a->ai_next) 
  {
    fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
    if (fd == -1) 
    {
      continue;
    }

    if (connect(fd, a->ai_addr, a->ai_addrlen) != -1) 
    { break; }

    close(fd);
    fd = -1;
   }

   if (fd == -1) 
   {
     fprintf(stderr, "connection failed :(\n");
     return 1;
   }

   printf("\n Connection successful \n");
   return 0;
}

/*
 * @brief This function is used to get the camera firmware version.
 * @param[out] firmware name
 * @return Error code.
 */
int getCameraFirmwareVersion(char *fw_version)
{
        size_t max_line_length = FW_NAME_MAX_LENGTH;
        char *file_buffer;
        char *locate_1 = NULL;
        FILE* fp;

        fp = fopen("/version.txt","r");
        if (NULL == fp) {
                printf("File:%s:%d. Error in opening version.txt \n", __FILE__, __LINE__);
                return RDKC_FAILURE;
        }

        file_buffer = (char*)malloc(FW_NAME_MAX_LENGTH + 1);
        if(file_buffer == NULL)
        {
                printf("File:%s:%d. Error in malloc \n", __FILE__, __LINE__);
                fclose(fp);
                return RDKC_FAILURE;
        }

        while(getline(&file_buffer,&max_line_length,fp) != -1)
        {
                /* find the imagename string */
                locate_1 = strstr(file_buffer,"imagename");
                if(locate_1)
                {
                        locate_1 += strlen("imagename:");
                        /* copy the contents till linefeed */
                        while(*locate_1 != '\n')
                                *fw_version++ = *locate_1++;
                        free(file_buffer);
                        file_buffer = NULL;
                        fclose(fp);
                        return RDKC_SUCCESS;
                }
        }

        /* unable to get the image name */
        strcpy(fw_version,"imagename entry not found");
        free(file_buffer);
        file_buffer = NULL;
        fclose(fp);
        return RDKC_SUCCESS;
}

/*
 * @brief API to get the MAC address of the device.
 * @param[out] device_mac MAC of device
 * @return Error code.
 */
int getDeviceMacValue(char *device_mac)
{
        int fd;
        struct ifreq ifr;
        unsigned char *mac;
        char deviceMACValue[CAM_MAC_MAX_LENGTH] = {'\0'};

	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;

	/* Get all Network Interfaces*/
	if (getifaddrs(&ifAddrStruct) == -1) {
		printf("Network interface error: 0x%x", errno);
		return RDKC_FAILURE;
	}

	printf("Available Interfaces are \n");
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {

		printf("%s \n", ifa->ifa_name);
	}

        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd >=0)
        {
                ifr.ifr_addr.sa_family = AF_INET;

		/* Iterate through each interfaces */
		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {

			if (!ifa->ifa_name) {
				continue;
			}

			if(0 == strcmp(ifa->ifa_name, "lo")) {
				printf("Skipping Loopback interface\n");
				continue;
			}

			if(0 == strcmp(ifa->ifa_name, "sit0")) {
				printf("Skipping tunnel interface\n");
                                continue;
                        }

			if(0 == strcmp(ifa->ifa_name, "macvlan0")) {
				printf("Skipping macvlan0 interface\n");
                                continue;
                        }

	                strncpy(ifr.ifr_name , ifa->ifa_name , IFNAMSIZ-1);

			/* get the hw address */
			if(ioctl(fd, SIOCGIFHWADDR, &ifr)>=0)
			{
				if (!ifr.ifr_hwaddr.sa_data) {
					continue;
				}

				close(fd);

				mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
				sprintf(deviceMACValue, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

				if(device_mac)
				{
					strcpy(device_mac,deviceMACValue);
					printf("Successfully retrieved the MAC as %s\n", deviceMACValue);
					return RDKC_SUCCESS;
				}
			}
			else
			{
				// No MAC available
				if(device_mac)
                                {
					strcpy(device_mac,"XX:XX:XX:XX:XX:XX");
				}

				close(fd);
				printf("Unable to get MAC as ioctl is failed\n");
				return RDKC_FAILURE;
			}
		}

		// No MAC available
		if(device_mac)
		{
			strcpy(device_mac,"XX:XX:XX:XX:XX:XX");
		}

		close(fd);
		printf("Unable to get MAC even after going through all the interfaces\n");
		return RDKC_FAILURE;
	}
	else
	{
		// No MAC available
		if(device_mac)
		{
			strcpy(device_mac,"XX:XX:XX:XX:XX:XX");
		}

                close(fd);
                printf("Unable to get MAC as opening the socket is failed\n");
                return RDKC_FAILURE;
        }
}


/**
 * @brief API to get the camera version number.
 * @param[out] version number
 * @return Error code.
 */
int getCameraVersionNum(char* version_num)
{
        size_t max_line_length = VER_NUM_MAX_LENGTH;
        char *file_buffer;
        char *locate_1 = NULL;
        FILE* fp;

        fp = fopen(VERSION_FILE,"r");
        if (NULL ==fp)
        {
               printf("%s(%d): Error in opening /version.txt file\n", __FILE__, __LINE__);
                return RDKC_FAILURE;
        }

        file_buffer = (char*)malloc(VER_NUM_MAX_LENGTH + 1);
        if (NULL == file_buffer)
        {
               printf("%s(%d): dynamic allocation failed\n", __FILE__, __LINE__);
                fclose(fp);
                return RDKC_FAILURE;
        }

        while (getline(&file_buffer,&max_line_length,fp) != -1)
        {
                /* find the imagename string */
                locate_1 = strstr(file_buffer,"VERSION");
                if(locate_1)
                {
                        locate_1 += strlen("VERSION=");
                        /* copy the contents till linefeed */
                        while(*locate_1 != '\n') {
                                *version_num++ = *locate_1++;
			}
                        *version_num = '\0'; //terminate the string using null character

                        free(file_buffer);
                        fclose(fp);
                        return RDKC_SUCCESS;
                }
        }

	// Veersion is not available
        strcpy(version_num,"x.x.x.x");
        free(file_buffer);
        fclose(fp);

        return RDKC_FAILURE;
}
/*
 * @description: This function is used to get interface and default gateway.
 *
 * @param[out]: default gateway address
 *
 * @param[out]: interface default gateway is linked to
 *
 * @return: Error code.
 */

int getDefaultGatewayAndIface(in_addr_t * addr, char *interface)
{
  long destination, gateway, flag;
  char iface[IF_NAMESIZE];
  char buf[BUFFER_SIZE];
  FILE * file;

  memset(iface, 0, sizeof(iface));
  memset(buf, 0, sizeof(buf));

  file = fopen("/proc/net/route", "r");
  if (!file)
  {
    return -1;
  }

  while (fgets(buf, sizeof(buf), file))
  {
    if (sscanf(buf, "%s %lx %lx %lx", iface, &destination, &gateway, &flag) == 4)
    {
      /* 
        Looking for destination 0.0.0.0 ie default gateway IP
        and as per net/route.h, the Flag value is,
		0001 -> U
		0010 -> G
	UG to determine Gateway  */
      if ((destination == 0) && (flag == 3))
      { /* default */
        *addr = gateway;
        strcpy(interface, iface);
        fclose(file);
        return 0;
      }
    }
   }
   /* default route not found */
   if (file)
   {
     fclose(file);
   }
   return -1;
}

/*
 * @description: This function will return address of default gateway
 *
 * @return : Address of default gateway
 *
 */
char* getDefaultGateway()
{
  in_addr_t addr = 0;
  char iface[IF_NAMESIZE];

  memset(iface, 0, sizeof(iface));

  int ret = getDefaultGatewayAndIface(&addr, iface);
  if (!ret)
    return inet_ntoa(*(struct in_addr *) &addr);
  else
    return NULL;
}

/**
* Returns the current time encoded as a time_t object
**
*/
time_t getCurrentTime(time_t *arg)
{
	if(arg != NULL)
	{
		time_t result = time(arg);
	
		if(result != (time_t)(-1))
		{
			//printf("The current time is %s(%jd seconds since the Epoch)\n",asctime(gmtime(&result)), (intmax_t)result);
			return  (intmax_t)result;
		}
		return (time_t)(-1);
	}
	struct timespec curr_time;
	
	if(clock_gettime(CLOCK_MONOTONIC, &curr_time) == -1)
	{
		perror("Error in getting monotonic linear time");
		return (time_t)(-1);
	}
	return curr_time.tv_sec;
}

unsigned int compareTimestamp(unsigned int new_ts, unsigned int old_ts)
{
	if(new_ts <  old_ts)
		return (0xffffffff - old_ts + new_ts);
	else
		return (new_ts - old_ts);
}

int getProcessId(char* processName)
{
    FILE *inFp = NULL;
    char command[BUFFER_SIZE] = {'\0'};
    int pid = 0;

    if (processName == NULL) {

        return pid;
    }

    sprintf(command, "pidof %s", processName);
    if(!(inFp = popen(command, "r"))){
        return pid;
    }
    fscanf(inFp,"%d",&pid);
    pclose(inFp);
    return pid;
}

/**
* @description: Set/Get entire content of a file.
* @param[in]  : filename - File name with path to get/set value
* @param[in]  : action - Action to perform. Should be "get" or "set"
* @param      : value - Value to get/set. This is a [out] param in case of "get" and [in] param in case of set
* @return     : 0 in case of success, 1 if failure
*/
int getSetFileContent(char* filename, char* action, char* value)
{
     FILE *fp = NULL;
     if (strcmp(action, "get") == 0)
     {
         int contentsize;
         fp = fopen(filename, "rb");
         if(NULL == fp)
         {
           printf("Error in opening file");
           return 1;
         }
         else
         {
           fseek(fp, 0L, SEEK_END);
           contentsize = ftell(fp);
           fseek(fp, 0, SEEK_SET);
           printf("File content - read content size - %d\n", contentsize);
           fread(value, contentsize, 1 ,fp);
           if(NULL != fp)
           {
             fclose(fp);
             fp = NULL;
           }
           return 0;
        }
     }
     else if (strcmp(action, "set") == 0)
     {
         if(NULL != value)
         {
           printf("Value to set (%s)\n", value);
           fp = fopen(filename ,"wb");
           if(fp)
           {
             printf("Saving value to location : %s\n", filename);
             fwrite(value, sizeof(char), strlen(value), fp);
             if (NULL != fp)
             {
               fclose(fp);
               fp = NULL;
             }
             return 0;
           }
           else{
             printf("Failed to open file. Skipping save\n");
             return 1;
           }
         }
         else
         {
           printf("Value to save is NULL...\n");
           return 1;
         }
     }
}
