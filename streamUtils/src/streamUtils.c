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
#include "sysUtils.h"
#include "streamUtils.h"
#include "rdkc_stream_api.h"

#define DATA_LEN                   256
#define MAX_ENCODE_STREAM_NUM      4

#define HIGH_RESOLUTION_HEIGHT   1080
#define HIGH_RESOLUTION_WIDTH    1920
#define MEDIUM_RESOLUTION_HEIGHT 720
#define MEDIUM_RESOLUTION_WIDTH  1280
#define LOW_RESOLUTION_HEIGHT    360
#define LOW_RESOLUTION_WIDTH     640

#define HIGH_RESOLUTION_CONFVALUE   "1080p"
#define MEDIUM_RESOLUTION_CONFVALUE "720p"
#define LOW_RESOLUTION_CONFVALUE    "360p"

int getStreamId(char* conf_file, char* param_name, char* resoln_level)
{
  char level[DATA_LEN];
  int stream_id;
  
  int ret = getConfValue(conf_file, param_name, level);
  if (level != NULL)
  {
    strncpy(resoln_level, level, strlen(level));
    resoln_level[strlen(level)] = '\0';
  }
  printf("\nResolution level %s", resoln_level);
    
  for (stream_id=0; stream_id < MAX_ENCODE_STREAM_NUM; stream_id++)
  {
    printf("\n Loop stream id : %d", stream_id);
    StreamConfig config;
    memset(&config, 0, sizeof(StreamConfig));
    rdkc_stream_get_config(stream_id, &config);
    int stream_height = config.height;
    int stream_width = config.width;
    printf("\n Loop stream_height : %d stream_width : %d", stream_height, stream_width);
    if (strcasecmp(resoln_level, LOW_RESOLUTION_CONFVALUE) == 0)
    {
      if (stream_height == LOW_RESOLUTION_HEIGHT && stream_width == LOW_RESOLUTION_WIDTH)
        return stream_id;      
    }
    else if (strcasecmp(resoln_level, MEDIUM_RESOLUTION_CONFVALUE) == 0)
    {
      if (stream_height == MEDIUM_RESOLUTION_HEIGHT && stream_width == MEDIUM_RESOLUTION_WIDTH)
        return stream_id;
    }
    else if (strcasecmp(resoln_level, HIGH_RESOLUTION_CONFVALUE) == 0)
    {
      if (stream_height == HIGH_RESOLUTION_HEIGHT && stream_width == HIGH_RESOLUTION_WIDTH)
        return stream_id;
    }  
  }
  return 1;
}
