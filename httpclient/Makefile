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
USE_HTTPCLIENT = yes
SUPPORT_MXML = yes

include ${RDK_PROJECT_ROOT_PATH}/utility/AppsRule.mak

LDFLAGS += "-ldl"

CFLAGS  += "-lrt"
CFLAGS  += -fPIC -Wall

LIBS += $(LIBFLAGS)

SRC          += HttpClient.cpp
SRC_TEST     += HttpClientTest.cpp

OBJ      = $(SRC:.cpp=.o)
OBJ_TEST = $(SRC_TEST:.cpp=.o)

INSTPROGS +=   libhttpclient.so
TARGET     =   http_client_test

install: $(INSTPROGS)
all: $(INSTPROGS) $(TARGET)
RM = rm

%.o:%.cpp
	$(CXX) -c $< $(CFLAGS)  -o $@

$(INSTPROGS): $(OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -shared -o $(@)

$(TARGET): $(OBJ) $(OBJ_TEST)
	$(CXX) $(CFLAGS) -o $(@) $^ $(LIBS) $(LDFLAGS) -lhttpclient

uninstall:
	$(RM) -f $(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/$(INSTPROGS)


clean:
	$(RM) -rf $(OBJ) $(OBJ_TEST) *~ $(INSTPROGS) $(TARGET)
