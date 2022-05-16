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

CFLAGS += -fstack-protector -Wno-error
CFLAGS 	+= -I${RDK_PROJECT_ROOT_PATH}/rdklogger/include
CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}/opensource/include
LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/rdklogger/src/.libs/ -lrdkloggers
ifeq ($(XCAM_MODEL), XHC3)
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -llog4c
endif

LIBFLAGS += -Wl,-rpath,$(RDK_PROJECT_ROOT_PATH)/opensource/lib
LIBFLAGS += -L${RDK_PROJECT_ROOT_PATH}/opensource/lib -lssl -lcrypto -ljansson

ifeq ($(USE_OPENCV), yes)
	export PKG_CONFIG_PATH=$(RDK_PROJECT_ROOT_PATH)/opensource/lib/pkgconfig
	CFLAGS	+=  `pkg-config --cflags opencv`
	LIBFLAGS += `pkg-config --libs opencv`
endif

ifeq ($(USE_WATCHDOG), yes)
	LIBFLAGS = -lpthread -L$(RDK_PROJECT_ROOT_PATH)/rdklogger/src/.libs/ -lrdkloggers
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -llog4c
endif

ifeq ($(USE_SSHUTIL), yes)
    CFLAGS 	+= -I${RDK_PROJECT_ROOT_PATH}/rdklogger/include
    CFLAGS 	+= -I${RDK_PROJECT_ROOT_PATH}/opensource/include
    CFLAGS     += -I${RDK_PROJECT_ROOT_PATH}/opensource/src/rtmessage
    CFLAGS 	+= -I${RDK_PROJECT_ROOT_PATH}/libexchanger/src
    LIBFLAGS = -L$(RDK_PROJECT_ROOT_PATH)/opensource/src/rtmessage/ -lrtMessage -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/libexchanger/Release/src -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/libexchanger/password/src
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/libexchanger/Release/src -lspake2plus
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lssl -lcrypto
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcjson
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/rdklogger/src/.libs/ -lrdkloggers
    LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -llog4c -lz
endif

ifeq ($(USE_CJSON), yes)
	CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}opensource/include/cjson/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcjson
endif

ifeq ($(USE_HTTPCLIENT), yes)
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -llog4c -lz
	LIBFLAGS += -L${RDK_PROJECT_ROOT_PATH}/opensource/lib -lssl -lcrypto -ljansson
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/httpclient
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/utility/httpclient -lhttpclient
endif

ifeq ($(USE_RBUS), yes)
	USE_RTMESSAGE = yes
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/rbus/include
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/rbuscore/rbus-core/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lmsgpackc
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/rbuscore/build/rbus-core/lib -lrbus-core
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/rbus/build/src -lrbus
endif

ifeq ($(USE_RTMESSAGE), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/rtmessage
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/src/rtmessage/ -lrtMessage -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/libexchanger/Release/src
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcjson
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
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz
	LIBFLAGS    += -L$(RDK_PROJECT_ROOT_PATH)/utility/sysUtils/src -lsysutils
	LDFLAGS = $(LIBFLAGS)
endif

ifeq ($(USE_CONFIGUTILS), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/utility/misc/include
	LIBFLAGS    += -L$(RDK_PROJECT_ROOT_PATH)/utility/misc/src/ -lmiscutils 
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

ifeq ($(USE_WIFIHAL), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/wifi-hal-generic/include
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/wifi-hal-generic/src/.libs/ -lwifihal
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/ -lwpa_client
endif

ifeq ($(USE_BLE), yes)
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/ -lblelnf
endif

ifeq ($(USE_CONSUMER), yes)
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/camera-hal/streamer/xBroker/
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/camera-hal/streamer/xBroker/xConsumer
	CFLAGS  += -I$(RDK_PROJECT_ROOT_PATH)/camera-hal/streamer
	LIBFLAGS   += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/vendor/img/fs/shadow_root/usr/lib -lstreamerconsumer
endif

ifeq ($(USE_PRODUCER), yes)
	CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}/camera-hal/streamer/
	CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}/camera-hal/streamer/xBroker/
	CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}/camera-hal/streamer/xBroker/xProducer/include
	CFLAGS  += -I${RDK_PROJECT_ROOT_PATH}/camera-hal/streamer/xaudio
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/camera-hal/streamer/xBroker/xProducer/lib -lstreamerproducer
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

ifeq ($(USE_TELEMETRY_2), yes)
	USE_RTMESSAGE = yes
	CFLAGS += -I$(RDK_PROJECT_ROOT_PATH)/telemetry/include/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/ -ltelemetry_msgsender -lt2utils -lmsgpackc -Wl,-rpath-link=${RDK_PROJECT_ROOT_PATH}/rbuscore/build/rbus-core/lib/ -Wl,-rpath-link=${RDK_PROJECT_ROOT_PATH}/rbus/build/src/ -Wl,-rpath-link=$(RDK_PROJECT_ROOT_PATH)/sdk/fsroot/ramdisk/usr/lib/
endif

ifeq ($(USE_KINESIS_PRODUCER), yes)
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src/common
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-producer-c/src/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/client/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/client/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/client/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/client/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/state/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/state/include/com/amazonaws/kinesis/video/state
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/state/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/state/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/common/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/common/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/heap/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/heap/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/heap/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/heap/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/trace/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/mkvgen/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/mkvgen/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/mkvgen/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/mkvgen/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/view/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/view/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/view/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/view/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-pic/src/utils/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-producer-c/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-producer-c/src/Common
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src/credential-providers
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/amazon-kinesis-video-streams-producer-c/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin/plugin-src/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz -lKinesisVideoProducer -lcproducer -llog4cplus
	# LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz -lproducer -llog4cplus
endif

ifeq ($(USE_KINESIS_PRODUCER_311), yes)
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src/common
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/src/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/client/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/client/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/client/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/client/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/state/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/state/include/com/amazonaws/kinesis/video/state
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/state/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/state/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/common/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/common/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/heap/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/heap/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/heap/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/heap/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/trace/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/mkvgen/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/mkvgen/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/mkvgen/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/mkvgen/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/view/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/view/include/com/amazonaws/kinesis/video/client
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/view/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/view/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/dependency/libkvspic/kvspic-src/src/utils/include
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/src
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/src/Common
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/src/credential-providers
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/dependency/libkvscproducer/kvscproducer-src/tst
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin
	CFLAGS  +=  -I$(RDK_PROJECT_ROOT_PATH)/opensource/src/amazon-kinesis-video-streams-producer-sdk-cpp/kinesis-video-gstreamer-plugin/plugin-src/
	LIBFLAGS += -L$(RDK_PROJECT_ROOT_PATH)/opensource/lib -lcurl -lz -lKinesisVideoProducer -lcproducer -llog4cplus
endif
