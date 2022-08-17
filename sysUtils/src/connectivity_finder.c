#include "connectivity_finder.h"
#include "sysUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <stdbool.h>

#define CAPTIVE_PORTAL_URL 	"http://connectivitycheck.comcast.com/generate_204"
#define CAPTIVE_PORTAL_URL_1	"http://captive.sys.comcast.net/generate_204"
#define CAPTIVE_PORTAL_URL_2    "http://clients3.comcast.com/generate_204"

#define GOOGLE_SERVER1		"8.8.8.8"
#define GOOGLE_SERVER2          "8.8.4.4"

static int curl_initialized = 0;
char* gateway = NULL;
static int portal_failure_count = 0;

MODE captivePortal_connectivity_check()
{
    CURL *curl;
    CURLcode res;
    FILE *fp = NULL;
    MODE ret;
    long http_code = 0;
    long timeout = 5L; // 5 seconds
    if (!curl_initialized)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized  = 1;
    }

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, CAPTIVE_PORTAL_URL);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        fp = fopen("/dev/null", "w");
        if(fp != NULL)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        }

        res = curl_easy_perform(curl);
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if(res != CURLE_OK)
        {
            ret = DISCONNECTED_STATE;
        }
        else if(http_code == 200)
        {
            ret = PAUSE_STATE;
        }
        else if(http_code == 204)
        {
            ret = CONNECTED_STATE;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        if(fp != NULL)
        {
            fclose(fp);
        }
    }
    return ret;
}

static bool is_gateway_connected()
{
    char cmdbuf[256];
    if (gateway != NULL)
    {
      snprintf(cmdbuf, sizeof(cmdbuf), "ping -c 3 %s -W 2 > /dev/null", gateway);
      if (system(cmdbuf) == 0)
      {
        return true;
      }
    }
    else
    {
      gateway = getDefaultGateway();
    }
    return false;
}

static bool is_external_network_connected()
{
    char cmdbuf[256];
    snprintf(cmdbuf, sizeof(cmdbuf), "ping -c 3 %s -W 2 > /dev/null", GOOGLE_SERVER1);
    if (system(cmdbuf) == 0)
    {
      return true;
    }
    else
    {
      memset(cmdbuf, 0, 256);
      snprintf(cmdbuf, sizeof(cmdbuf), "ping -c 3 %s -W 2 > /dev/null", GOOGLE_SERVER2);
      if (system(cmdbuf) == 0)
      {
        return true;
      }
    }
    return false;
}

//curl to captive portal
//HTTP-204 -- Connected (NORMAL WORKING STATE)
//HTTP-200 -- Connected (PAUSE STATE)
//curl timeout -- Disconnected (WIFI DISCONNECTED STATE)
MODE stateFinder()
{
    MODE state = captivePortal_connectivity_check();
    if ( state == DISCONNECTED_STATE )
    {
      if (is_external_network_connected())
      {
        state = CONNECTED_STATE;
        portal_failure_count++;
      }
      /*else if (is_gateway_connected() && !is_external_network_connected())
      {
        state = PAUSE_STATE;
        portal_failure_count++;
      }*/
      else
      {
        state = DISCONNECTED_STATE;
      }
      // Print log for captive portal failure only once in approx 5 mins
      if (portal_failure_count >= 150)
      {
        printf("Captive Portal not reachable. Back up logic is in use\n");
        portal_failure_count=0;
      }
    }
    return state;
}

