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
USE_SSHUTIL = yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

CFLAGS  += -I./
CFLAGS  += -fPIC -Wall

SRC     += sshUtility.c

OBJ      = $(SRC:.c=.o)

TARGET     =   sshUtility

install: $(TARGET)
all: $(TARGET)
RM = rm

%.o:%.c
	$(CC) -c $< $(CFLAGS)  $(LIBFLAGS) -o $@ $(LIBS)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(@) $^ $(LIBFLAGS)
	$(STRIP) $@

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/bin/$(TARGET)

clean:
	$(RM) -rf $(OBJ) *~ $(TARGET)
