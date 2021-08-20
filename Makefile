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

SUBDIRS += httpclient
SUBDIRS += RFCConfig
SUBDIRS += dynamicLogging 
SUBDIRS += sysUtils
ifeq ($(XCAM_MODEL), SCHC2)
SUBDIRS += xwdynamicLogging
SUBDIRS += sshUtility
endif
ifneq ($(XCAM_MODEL), XHB1)
ifneq ($(XCAM_MODEL), XHC3)
SUBDIRS += streamUtils
endif
endif
#SUBDIRS += configUtils
SUBDIRS += misc
SUBDIRS += watchDog

all:
	@for i in `echo $(SUBDIRS)`; do \
		$(MAKE) -C $$i $@ || exit 1; \
	done

install clean uninstall mrproper:
	@for i in `echo $(SUBDIRS)`; do \
		$(MAKE) -C $$i $@; \
	done

