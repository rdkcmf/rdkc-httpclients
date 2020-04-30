#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

class watchDogTimer
{
    public:
        watchDogTimer();
        ~watchDogTimer();
        bool start();
        bool stop();
        bool setDeviceName(char* fileName);
        bool setTimeout(int timeout);
        int getBootStatus();
        void flagToString(int flags, char* logString);
        void usage();

    private:

        int ioctlCall(int flag);
        bool startTimerThread();
        bool stopTimerThread();
        static void * task(void * ctx);
        pthread_t mTimerThreadID;
        int mDeviceDescriptor;
        int mTimeout;
        int mTimerThreadStarted;
        char mDeviceName[64];
        struct watchdog_info ident;
};

#endif
