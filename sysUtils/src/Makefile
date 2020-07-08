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

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak
#LDFLAGS = $(LIBFLAGS)

INCPATH += ../include

CFLAGS += -I${INCPATH}
CFLAGS += -fPIC -Wall
LDFLAGS := -lpthread
LDFLAGS += -L$(PROJ_ROOT)/share/libsccomm/bin -lsccomm
LDFLAGS += -L${PROJ_ROOT}/vendor/img/fs/shadow_root/usr/lib -lmxml

TARGET_UTIL_LIB = libsysutils.so

SYSUTIL_SRCS  = sysUtils.c
SYSUTIL_CPP_SRCS = dmcliUtils.cpp

OBJS_SYSUTIL = $(SYSUTIL_SRCS:.c=.o)
OBJS_CPP_SYSUTIL = $(SYSUTIL_CPP_SRCS:.cpp=.o)

RM = rm -f
all: $(TARGET_UTIL_LIB)

$(TARGET_UTIL_LIB): $(OBJS_SYSUTIL) $(OBJS_CPP_SYSUTIL)
	$(CXX) -shared $(LDFLAGS) $^ -o $@ -lrt
%.o:%.c
	$(CC) -c $< $(CFLAGS)  -o $@
%.o:%.cpp
	$(CXX) -c $< $(CFLAGS)  -o $@
uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/$(TARGET_UTIL_LIB)
clean:
	${RM} ${TARGET_UTIL_LIB} ${OBJS_SYSUTIL} ${OBJS_CPP_SYSUTIL}

.PHONY: all clean

