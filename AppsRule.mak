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

include ${PROJ_PRERULE_MAK_FILE}

CFLAGS += -fstack-protector
CFLAGS 	+= -I${RDK_PROJECT_ROOT_PATH}/rdklogger/include
LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/rdklogger/src/.libs/ -lrdkloggers

LIBFLAGS += -L${RDK_PROJECT_ROOT_PATH}/opensource/lib -lssl -lcrypto -ljansson
LIBFLAGS += -Wl,-rpath,$(RDK_PROJECT_ROOT_PATH)/opensource/lib

ifeq ($(USE_OPENCV), yes)
	export PKG_CONFIG_PATH=$(RDK_PROJECT_ROOT_PATH)/opensource/lib/pkgconfig
	CFLAGS	+=  `pkg-config --cflags opencv`
	LIBFLAGS += `pkg-config --libs opencv`
endif

ifeq ($(USE_HTTPCLIENT), yes)
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -llog4c -lz
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/httpclient
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/httpclient -lhttpclient
endif

ifeq ($(USE_RTMESSAGE), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/rtmessage
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/src/rtmessage/ -lrtMessage -lcjson
endif

ifeq ($(USE_PLUGINS), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/plugins
endif

ifeq ($(USE_CONFIGMGR), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/RFCConfig/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/RFCConfig/src/ -lrfcconfig
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/configMgr/config/src/polling
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/configMgr/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/configMgr/config/src -lconfigmanager -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/utility/RFCConfig/src/ -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/configMgr/config/src/polling/soc/
endif

ifeq ($(USE_RFCCONFIG), yes)
	ifneq ($(USE_CONFIGMGR), yes)
		CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/RFCConfig/include
		LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/RFCConfig/src/ -lrfcconfig
	endif
endif

ifeq ($(USE_MISC), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/misc/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/misc/src -lmiscutils
endif

ifeq ($(USE_SYSUTILS), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/sysUtils/include
	LIBFLAGS    += -L$(RDK_PROJECT_ROOT_PATH)/utility/sysUtils/src -lsysutils
endif

ifeq ($(USE_BREAKPAD), yes)
	CFLAGS += -DBREAKPAD
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/breakpadwrap/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/breakpadwrap -lbreakpadwrap
endif

ifeq ($(USE_LIBXML), yes)
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/opensource/include/libxml2/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lxml2 -lz -lcurl
endif

ifeq ($(USE_GST), yes)
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/opensource/include/gstreamer-1.0 -I$(RDK_PROJECT_ROOT_PATH)/opensource/include/glib-2.0 -I$(RDK_PROJECT_ROOT_PATH)/opensource/lib/glib-2.0/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib -lgstapp-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
endif

ifeq ($(USE_STREAMUTILS), yes)
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/utility/streamUtils/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/streamUtils/src -lstreamutils
endif

ifeq ($(USE_SMARTRC), yes)
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba/prebuild/ambarella/library/smartrc/lib/arch_s3l -lsmartrc
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba/prebuild/data_transfer -ldatatx

	CFLAGS += -DS3L
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba/prebuild/ambarella/library/smartrc/include/
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba/packages/data_transfer/
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba/include/arch_s3l
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/src/amba
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/configMgr/include
endif

ifeq ($(USE_LIBSYSWRAPPER), yes)
        CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/libsyswrapper/source/
        LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/libsyswrapper/source/.libs/  -lsecure_wrapper 
endif

ifeq ($(USE_KINESIS_PRODUCER), yes)
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-c-producer/src/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/client/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/client/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/client/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/client/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/state/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/state/include/com/amazonaws/kinesis/video/state
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/state/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/state/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/common/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/common/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/heap/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/heap/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/heap/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/heap/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/trace/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/mkvgen/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/mkvgen/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/mkvgen/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/mkvgen/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/view/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/view/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/view/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/view/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-pic/src/utils/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-producer/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-producer/src/common
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-producer/src/credential-providers
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-producer/opensource/jsoncpp
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-producer/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin/plugin-src/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz -lproducer -lcproducer -llog4cplus
	# LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz -lproducer -llog4cplus
endif
