/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/
#include <iostream>
#include "HttpClient.h"

using namespace std;

#define POLLING_URL	"https://config-cvr.g.xfinityhome.com/config"
#define DNS_CACHE_TIMEOUT 60
#define MAX_UPLOAD_TIMEOUT 10

int main(int argc, char *argv[])
{
	HttpClient httpClient;
	char* data;
	int curlCode = 0;

	if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL ){
        	/* Open HTTP client */
		httpClient.open(POLLING_URL);

        	/* Perform HTTP GET request */
        	data = httpClient.get(POLLING_URL, &curlCode);
	}
	else{
		char* polling_url = argv[1];
		char* ca_cert_file = argv[2];
		char* cert_file = argv[3];

                /* Open HTTP client */
		httpClient.open(polling_url, DNS_CACHE_TIMEOUT, MAX_UPLOAD_TIMEOUT, ca_cert_file, cert_file);

	        /* Perform HTTP GET request */
        	data = httpClient.get(polling_url, &curlCode);

	}

	if(!curlCode){
		cout<<"\n Recieved data successfully..."<<endl;
		cout<<"\n Data: \n"<<data<<endl;
	}
	else{
		cout<<"\n HTTP GET request failed..."<<endl;
	}

	/* Close HTTP client */
	httpClient.close();

	return 0;
}
