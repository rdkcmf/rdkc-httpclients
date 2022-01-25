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
#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>


#define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#define MAX_HEADER_SIZE  1025
#define BUFFER_SIZE     4096
#define MAX_UPLOAD_TIME 8
#define DEFAULT_DNS_CACHE_TIMEOUT   60
#define DEFAULT_CURL_TIMEOUT 10

#define CA_CERT_FILE    "/etc/ssl/certs/CAcerts.pem"
#define CERT_FILE       "/etc/ssl/certs/client.pem"

#define XPKI_KEY_FILE   "/opt/usr_config/xpki_key.pem"
#define XPKI_CERT_FILE  "/opt/usr_config/xpki_cert.pem"

#define STATIC_XPKI_KEY_FILE   "/opt/usr_config/static_xpki_key.pem"
#define STATIC_XPKI_CERT_FILE  "/opt/usr_config/static_xpki_cert.pem"

enum HTTP_RESPONSE_STATUS
{
	RDKC_HTTP_RESPONSE_OK = 200,
	RDKC_HTTP_RESPONSE_REDIRECT_START = 300,
	RDKC_HTTP_RESPONSE_FORBIDDEN = 403
};

/**
 * @struct callbackData
 * @brief This structure contains information for the call back data.
 */
struct callbackData
{
	char *data;
	int pos;
	int allocated;
};

/**
 * @class HttpClient
 * @brief Defines a class to handle the http communication at client side.
 */
class HttpClient
{
public:
	HttpClient();
	~HttpClient();

	void open(const char* url, long dnsCacheTimeout = DEFAULT_DNS_CACHE_TIMEOUT, int upload_time = MAX_UPLOAD_TIME, const char* ca_cert_file = CA_CERT_FILE, const char* cert_file = CERT_FILE);
	void openWithSSL(const char* url, long dnsCacheTimeout, const char* ca_cert_file, const char* cert_file , const char* key_file);
	
	void addHeader(const char * headerStr, const char * headerVal);
	void resetHeaderList();
	char *get(const char *url, int *curlCode);
	char *getResponse(const char *url, int *curlCode, int connecttimeout = DEFAULT_CURL_TIMEOUT);
	int post(const char *url, const char *data, long *response_code);
	int post_binary(const char *url, const char *data, long *response_code,long file_len);
        int post_binary(const char *url, const char *data, long *response_code,long file_len,int startTime);
	char *getHashCode();
	curl_off_t getUploadSpeed();
	char *post_getdata(const char *url, const char *data, long int *response_code, int connecttimeout = DEFAULT_CURL_TIMEOUT);
	void close();

private:
	CURL *curlEasyHandle;
	struct curl_slist *curlSlist;
	struct callbackData *cbData;
	time_t m_startTime;
	int m_remainingTime;
        long dnsCacheTimeout;
	const char* ca_cert_file;
        const char* cert_file;
	char  m_SHAHASHHeader[MAX_HEADER_SIZE];
	char  m_shaHashCode[MAX_HEADER_SIZE];
	static int max_upload_time;
	curl_off_t m_uploadSpeed;
	static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t write_post_response(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t headerCallback(void *buffer, size_t size, size_t numData, void *stream);
	const char *url_http_code_str(int http_code);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static int ProgressCallback_thumbnail(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	void curlEasyHandle_initialize(const char *url);
	void curlEasyHandle_initialize_mutualtls(const char* url);
	void curlEasyHandle_reset();
	bool is_xpki_enabled;
	bool is_static_xpki_enabled;
};

#endif
