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
include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

INCPATH += ../include

CFLAGS += -I${INCPATH}
CFLAGS += -fPIC -Wall
CFLAGS += -g -std=c++11
LDFLAGS = -lpthread

RELEASE_TARGET = libmiscutils.so
DEBUG_TARGET = libmiscutils_debug.so
MISCUTILS_SRCS  = stringUtils.cpp fileUtils.cpp
OBJS_MISCUTILS = $(MISCUTILS_SRCS:.cpp=.o)

RM = rm -f
all: $(RELEASE_TARGET) $(DEBUG_TARGET) install

$(RELEASE_TARGET): $(OBJS_MISCUTILS)
	$(CC) -shared $(LDFLAGS) $^ -o $@ -lrt
	$(STRIP) $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS_MISCUTILS)
	$(CC) -shared $(LDFLAGS) $^ -o $@ -lrt

%.o:%.cpp
	$(CC) -c $< $(CFLAGS)  -o $@

install:
ifneq ($(XCAM_MODEL), XHB1)
	$(PLATFORM_BREAKPAD_BINARY) $(DEBUG_TARGET) > $(RELEASE_TARGET).sym
	mv -f $(RELEASE_TARGET).sym  $(PLATFORM_SYMBOL_PATH)
	echo "xStreamer debug symbol created for $(RELEASE_TARGET)"
endif

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/$(RELEASE_TARGET)
clean:
	${RM} ${RELEASE_TARGET} $(DEBUG_TARGET) ${OBJS_MISCUTILS}

.PHONY: all clean

