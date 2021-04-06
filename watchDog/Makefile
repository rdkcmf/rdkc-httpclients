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

CURRENTPATH = $(PWD)
USE_WATCHDOG = yes
USE_BREAKPAD = yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

CFLAGS  += -I./
CFLAGS  += -g -fPIC -Wall -fpermissive

SRC     += watchDogTimer.cpp Main.cpp

OBJ      = $(SRC:.cpp=.o)

RELEASE_TARGET     =   watchDogTimer
DEBUG_TARGET     =   watchDogTimer_debug

install: $(RELEASE_TARGET)
	$(PLATFORM_BREAKPAD_BINARY) $(DEBUG_TARGET) > $(RELEASE_TARGET).sym
	sed -i "1s/$(DEBUG_TARGET)/$(RELEASE_TARGET)/" $(RELEASE_TARGET).sym
	mv -f $(RELEASE_TARGET).sym  $(PLATFORM_SYMBOL_PATH)
	echo "Debug symbol created for $(RELEASE_TARGET)"

all: $(RELEASE_TARGET) $(DEBUG_TARGET) install
RM = rm

%.o:%.cpp
	$(CXX) -c $< $(CFLAGS)  -o $@ $(LIBS)

$(RELEASE_TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $(@) $^ $(LIBFLAGS)
	$(STRIP) $@

$(DEBUG_TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $(@) $^ $(LIBFLAGS)

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/bin/$(RELEASE_TARGET)

clean:
	$(RM) -rf $(OBJ) *~ $(RELEASE_TARGET) $(DEBUG_TARGET)
