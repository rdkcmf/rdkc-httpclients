#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include "watchDogTimer.h"

#include "rdk_debug.h"
#ifdef BREAKPAD
#include "breakpadwrap.h"
#endif

watchDogTimer watchDog;

static void terminate(int sig)
{
	watchDog.stop();
	RDK_LOG( RDK_LOG_INFO,"LOG.RDK.WATCHDOG", "Stopping the watchdog Timer\n");
    return;
}

int main(int argc, char *argv[])
{
    rdk_logger_init("/etc/debug.ini");
	int opt = 0, timeout = 0;
	signal(SIGINT, terminate);

#ifdef BREAKPAD
	sleep(1);
	BreakPadWrapExceptionHandler eh;
	eh = newBreakPadWrapExceptionHandler();
#endif

	if(argc <= 1)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.WATCHDOG","arguments missing\n");
		watchDog.usage();
		return -1;
	}
    while((opt = getopt(argc, argv, ":f:s:")) != -1)
    {
        switch(opt)
        {
			case 'f':
				if(optarg)
				{
					RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","device file Name: %s\n", optarg);
					watchDog.setDeviceName(optarg);
				}
				break;
			case 's':
				if(optarg)
				{
					timeout = atoi(optarg);
					watchDog.setTimeout(timeout);
				}
                break;
            case ':':
                RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","Options are not supplied with values. Using the default values\n");
                break;
            case '?':
                RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG","unknown option: %c\n", optopt);
                break;
			default:
				RDK_LOG(RDK_LOG_INFO,"LOG.RDK.WATCHDOG", "Using default values\n");
				break;
        }
    }
	watchDog.start();
	return 0;
}
