#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include "telemetry_busmessage_sender.h"
#include "watchDogTimer.h"

#include "rdk_debug.h"

#define MAX_TIMEOUT_RETURN 3

using namespace std;

/** @description: usage
 *  @param[in] void
 *  @return: void
 */
void watchDogTimer::usage()
{
	RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","Usage: watchDogTimer [OPTIONS] \n");
	RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG"," [OPTIONS] \n\
			-f device file \n\
			-s set Timeout \n ");
}

/** @description: To convert the watchdog flags to string
 *  @param[in] flags : The watchDog flags
 *  @param[in] logString :logString to be returned
 *  @return: void
 */
void watchDogTimer::flagToString(int flags, char* logString)
{
	if(flags & WDIOF_KEEPALIVEPING)
		sprintf(logString+strlen(logString), "Keep alive ping reply\n");
	if (flags & WDIOF_MAGICCLOSE)
		sprintf(logString+strlen(logString), "Supports magic close char\n");
	if (flags & WDIOF_SETTIMEOUT)
		sprintf(logString+strlen(logString), "Set timeout supported\n");
	if (flags & WDIOF_ALARMONLY)
		sprintf(logString+strlen(logString), "Triggers external alarm\n");
	if (flags & WDIOF_OVERHEAT)
		sprintf(logString+strlen(logString), "Reset due to CPU overheat\n");
	if (flags & WDIOF_FANFAULT)
		sprintf(logString+strlen(logString), "Fan Failed\n");
	if (flags & WDIOF_EXTERN1)
		sprintf(logString+strlen(logString), "External relay 1\n");
	if (flags & WDIOF_EXTERN2)
		sprintf(logString+strlen(logString), "External relay 2\n");
	if (flags & WDIOF_POWERUNDER)
		sprintf(logString+strlen(logString), "Power bad/power fault\n");
	if (flags & WDIOF_CARDRESET)
		sprintf(logString+strlen(logString), "Card previously reset the CPU\n");
	if (flags & WDIOF_POWEROVER)
		sprintf(logString+strlen(logString), "Power over voltage\n");
	if (flags & WDIOF_PRETIMEOUT)
		sprintf(logString+strlen(logString), "Pretimeout (in seconds), get/set\n");
	if (flags & WDIOS_DISABLECARD)
		sprintf(logString+strlen(logString), "Turn off the watchdog timer\n");
	if (flags & WDIOS_ENABLECARD)
		sprintf(logString+strlen(logString), "Turn on the watchdog timer\n");
	if (flags & WDIOS_TEMPPANIC)
		sprintf(logString+strlen(logString), "Kernel panic on temperature trip\n");
}

/** @description: constructor
 *  @param[in] void
 *  @return: void
 */
watchDogTimer::watchDogTimer()
{
	mDeviceDescriptor = 0;
	mTimeout = 15; // default timeout value
	memset(mDeviceName, '\0', sizeof(mDeviceName));
	strncpy(mDeviceName, "/dev/watchdog", sizeof(mDeviceName));
	memset(&ident, 0, sizeof(ident));
}

/** @description: destructor
 *  @param[in] void
 *  @return: void
 */
watchDogTimer::~watchDogTimer()
{
	RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","Quitting the watchDogTimer\n");
}

/** @description: To trigger the timer thread
 *  @param[in] object of the watchDog class
 *  @return: void
 */
void * watchDogTimer::task(void * ctx)
{
	watchDogTimer * timer = (watchDogTimer*)ctx;
	if(false == timer->startTimerThread())
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","Error in thread creation or thread stopped!\n");
	}
}

/** @description: setter to set the device name
 *  @param[in] device Name
 *  @return: success
 */
bool watchDogTimer::setDeviceName(char* fileName)
{
	memset(mDeviceName, '\0', sizeof(mDeviceName));
	strncpy(mDeviceName, fileName, sizeof(mDeviceName));
	return true;
}

/** @description: setter to set timeout
 *  @param[in] timeout value integer
 *  @return: success
 */
bool watchDogTimer::setTimeout(int timeout)
{
	mTimeout = timeout;
	return true;
}

/** @description: function to stop watchdog
 *  @param[in] void
 *  @return: success (true)
 */
bool watchDogTimer::stop()
{
	stopTimerThread();
	close(mDeviceDescriptor);
	return true;
}

/** @description: stop the timer thread
 *  @param[in] void
 *  @return: success
 */
bool watchDogTimer::stopTimerThread()
{
	mTimerThreadStarted = 0;
	return true;
}

/** @description: triggers the timer thread
 *  @param[in] void
 *  @return: success or failure
 */
bool watchDogTimer::startTimerThread()
{
	if (mTimerThreadStarted == 0)
	{
		mTimerThreadStarted = 1;
	}
	else
	{
		char threadName[128] = {0};
		memset(threadName, '\0', sizeof(threadName));
		if (pthread_getname_np(mTimerThreadID, threadName, sizeof(threadName)) != 0)
		{
			memset(threadName, '\0', sizeof(threadName));
			strncpy(threadName, "watchDogTimerThread", sizeof(threadName) - 1);
		}
		RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","%s is already running!", threadName);
		return false;
	}
	while(mTimerThreadStarted)
	{
		int flags;
		flags = ioctlCall(WDIOC_KEEPALIVE);
		if(-1 == flags)
		{
			RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","Keep Alive failed! exiting the thread\n");
			return false;
		}
		sleep(2);
	}
	return true;
}

/** @description: API to get the last reboot reason
 *  @param[in] void
 *  @return: flag value which is returned from ioctl
 */
int watchDogTimer::getBootStatus()
{
	int flags = 0;
	flags = ioctlCall(WDIOC_GETBOOTSTATUS);
	if(-1 == flags)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","Error in getting the bootStatus\n");
		return -1;
	}
	return flags;
}

/** @description: generic api to make ioctl calls
 *  @param[in] flag value
 *  @return: ioctl return value
 */
int watchDogTimer::ioctlCall(int flags)
{
	int retFlag = 0;
	if(0 != ioctl(mDeviceDescriptor, flags, &retFlag))
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","Error in ioctl call\n");
		return -1;
	}
	return retFlag;
}

/** @description: function to start the watchDog execution
 *  @param[in] void
 *  @return: success or failure
 */
bool watchDogTimer::start()
{
	int flags = 0, getTimeout = 0, retry = 0;
	char logString[512] = {0};
	mDeviceDescriptor = open(mDeviceName, O_WRONLY);

	//error checking if the open call failed.
	if(-1 == mDeviceDescriptor)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","Failed to open the device\n");
		return false;
	}

	flags = getBootStatus();
	if(-1 == flags)
	{
		return false;
	}
	RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","Flag set by last reboot(WDIOC_GETBOOTSTATUS) is 0x%x.\n", flags);
	//Adding the reboot reason found to the log.
	if(flags != 0)
	{
		memset(logString, '\0', sizeof(logString));
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		sprintf(logString, "%d-%02d-%02dT%02d:%02d:%02dGMT PreviousRebootReason: watchDog-", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		flagToString(flags, logString+strlen(logString));
		RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","%s\n", logString);
                t2_event_d("SYS_REBOOT_WDKeepAlive", 1);
                t2_event_d("SYS_REBOOT_WDCloseChar", 1);
                t2_event_d("SYS_REBOOT_WDTO", 1);
                t2_event_d("SYS_REBOOT_WDExtrnlAlarm", 1);
                t2_event_d("SYS_REBOOT_WDCPUOverHeat", 1);
                t2_event_d("SYS_REBOOT_WDFanFail", 1);
                t2_event_d("SYS_REBOOT_WDExtrnlRelay", 1);
                t2_event_d("SYS_REBOOT_WDPwrFault", 1);
                t2_event_d("SYS_REBOOT_WDCpuReset", 1);
                t2_event_d("SYS_REBOOT_WDOverVoltage", 1);
                t2_event_d("SYS_REBOOT_WDPreTO", 1);
                t2_event_d("SYS_REBOOT_WDTimerOff", 1);

		FILE* fp = fopen("/opt/logs/rebootInfo.log", "w");
		fprintf(fp, "%s", logString);
		fclose(fp);
		fp = NULL;
	}

	while(retry < MAX_TIMEOUT_RETURN)
	{
		RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","Setting watchdog timer to %d.\n", mTimeout);
		if(0 != ioctl(mDeviceDescriptor, WDIOC_SETTIMEOUT, &mTimeout))
		{
			RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","unable to Set the timeout value\n");
			close(mDeviceDescriptor);
			return false;
		}
		getTimeout = ioctlCall(WDIOC_GETTIMEOUT);
		if(-1 == getTimeout)
		{
			RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","fetching the timeout value failed\n");
		}
		if(mTimeout == getTimeout)
			break;
		if (retry+1  == MAX_TIMEOUT_RETURN)
		{
			RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","timeout setting is failing continuosly\n");
			return false;
		}
		retry++;
		sleep(1);
	}

	if (mTimerThreadStarted == 0)
	{
		int rc = pthread_create(&mTimerThreadID, NULL, &watchDogTimer::task, this);
		if (!rc && mTimerThreadID)
		{
			pthread_setname_np(mTimerThreadID, "watchDogTimerThread");
		}
		RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG"," Task Thread created = %lu \n", mTimerThreadID);
	}
	pthread_join(mTimerThreadID, NULL);
	return true;
}
