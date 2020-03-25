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
CFLAGS += -std=c++11
LDFLAGS += -lpthread

TARGET_UTIL_LIB = libmiscutils.so
MISCUTILS_SRCS  = stringUtils.cpp fileUtils.cpp
OBJS_MISCUTILS = $(MISCUTILS_SRCS:.cpp=.o)

RM = rm -f
all: $(TARGET_UTIL_LIB)

$(TARGET_UTIL_LIB): $(OBJS_MISCUTILS)
	$(CC) -shared $(LDFLAGS) $^ -o $@ -lrt
%.o:%.cpp
	$(CC) -c $< $(CFLAGS)  -o $@
uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/$(TARGET_UTIL_LIB)
clean:
	${RM} ${TARGET_UTIL_LIB} ${OBJS_MISCUTILS}

.PHONY: all clean

