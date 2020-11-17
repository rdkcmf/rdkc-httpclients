##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2019 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

# Add dependent libraries
USE_RTMESSAGE = yes
USE_DATAPROVIDER = no
SUPPORT_MXML = yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

CXX_FLAG = -std=c++11

SRC = sample.cpp
OBJ  = $(SRC:.cpp=.o)

CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/dynamicLogging/util

LIBS += $(LIBFLAGS)
LIBS    += -L$(RDK_PROJECT_ROOT_PATH)/utility/dynamicLogging/util -lrdkdynamiclog

TARGET = enable_dynamic_log

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $(LIBS) $^ -o $(@)

$(OBJ): $(SRC)
	$(CXX) $(CXX_FLAG) -c $^  $(CFLAGS) -o $(@)

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/$(TARGET)


clean:
	$(RM) -rf $(OBJ) *~ $(TARGET)

