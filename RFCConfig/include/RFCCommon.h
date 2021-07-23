/**
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
**/
#ifndef __RFCCOMMON_H_
#define __RFCCOMMON_H_

#include "RFCConfigAPI.h"

#define RFCFILE "rfcVariable.ini"

/* ---------------- GENERIC -------------------- */
#define RDKC_SUCCESS 0
#define RDKC_FAILURE -1
#define MAX_SIZE 100
#define DELIMITTER '.'
#define RDKC_TRUE "true"
#define RDKC_FALSE "false"
#define RFC_PATH "/opt/RFC"

/* ------------------- IV ---------------------- */
#define IVENGINE "IVEngine"
#define GMM "GMM"
#define ALG_IV 6
#define ALG_GMM 1
#define TYPE_OF_FRAME "FRAME_TYPE"
#define FRAME_TYPE_YUV "yuv"
#define FRAME_TYPE_ME1 "me1"
#define FT_YUV 1
#define FT_ME1 2
#define RFC_IV ".RFC_RDKC_IV.ini"
#define RFC_GMM ".RFC_RDKC_XCVGMM.ini"

/* ------------------- CVR --------------------- */
//#define RFC_CVR_AUDIO ".RFC_RDKC_CVR_AUDIO.ini"
#define CVR_AUDIO "CVR_AUDIO"
#define KVS_ABSTIMESTAMP "KVSCLIP_ABSTIMESTAMP"
#define KVS_LIVEMODE "KVSCLIP_LIVEMODE"

/* ------------------- EVO --------------------- */
#define LIVE_AUDIO "LIVE_AUDIO"

/* ------------------- THUMBNAIL UPLOAD --------------------- */
#define RFC_TN_UPLOAD                   ".RFC_RDKC_TN_UPLOAD.ini"
#define TN_UPLOAD_URL                   "TN_UPLOAD_URL"
#define TN_UPLOAD_PASSIVE_INTERVAL      "TN_UPLOAD_PASSIVE_INTERVAL"

/* ----------------------------  OVERLAY  ------------------------ */
#define RFC_OVERLAY             ".RFC_RDKC_OVERLAY.ini"
#define OVERLAY_FEATURE         "RDKC_OVERLAY"

/* ----------------------------  OD_FRAMES  ------------------------ */
#define OD_FRAMES_UPLOAD	"RDKC_OD_FRAMES_UPLOAD"

/* ----------------------------  SMART THUMBNAIL  ------------------------ */
#define SMART_TN_UPLOAD		"SMART_TN_UPLOAD"

/* ----------------------------  DELIVERY DETECTION  --------------------- */
#define DELIVERY_DETECTION_RFC  "RDKC_DELIVERY_DETECTION"

/* ----------------------------  JSON Provisioning  ------------------------ */
#define RFC_JSON_FLAG                	   "RFC_ENABLE_RDKC_PROVISIONING_JSON"

/* ----------------------------  Audio Analytic  ------------------------ */
#define RDKC_AA_ENGINE          "RDKC_AA_ENGINE"
#define RDKC_AA_UPLOAD_URL      "RDKC_AA_UPLOAD_URL"
#define RDKC_AA_PCM_UPLOAD      "RDKC_AA_PCM_UPLOAD"
#define RDKC_AA_CLIP_DURATION   "RDKC_AA_CLIP_DURATION"

/* ---------------------------- WEBPA RFC------------------------ */
#define RFC_RDKC_CVR_AUDIO "ENABLE_RDKC_CVR_AUDIO"
#define RFC_RDKC_XCVGMM "ENABLE_RDKC_XCVGMM"
#define RFC_RDKC_TN_UPLOAD "ENABLE_RDKC_TN_UPLOAD"
#define RFC_RDKC_SMART_TN_UPLOAD "ENABLE_RDKC_SMART_TN_UPLOAD"
#define RFC_RDKC_CVR_LITE "ENABLE_RDKC_CVR_LITE"
#define RFC_RDKC_CVR_KVS "ENABLE_RDKC_CVR_KVS"
#define RFC_RDKC_LIVE_AUDIO "ENABLE_RDKC_LIVE_AUDIO"
#define RFC_RDKC_LNF "ENABLE_RDKC_LNF"
#define RFC_RDKC_OVERLAY "ENABLE_RDKC_OVERLAY"
#define RFC_RDKC_EVO "ENABLE_RDKC_EVO"
#define RFC_RDKC_PROVISIONING_JSON "ENABLE_RDKC_PROVISIONING_JSON"
#define RFC_RDKC_AA "ENABLE_RDKC_AA"
#define RFC_RDKC_ADVANCED_GOP "ENABLE_RDKC_ADVANCED_GOP"
#define RFC_RDKC_RESOLUTIONCHANGE "ENABLE_RDKC_RESOLUTIONCHANGE"
#define RFC_RDKC_HDR "ENABLE_RDKC_HDR"
#define RFC_XCAM2_SMARTRC "ENABLE_XCAM2_SMARTRC"
#define RFC_XCAM2_THERMAL_MONITOR "ENABLE_XCAM2_THERMAL_MONITOR"
#define RFC_RDKC_CONFIGURE_DAYNIGHT "ENABLE_XCAM2_THERMAL_MONITOR"

#endif
