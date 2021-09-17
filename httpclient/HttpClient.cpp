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
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <curl/curl.h>
#include <string>
#include "rdk_debug.h"
#include "HttpClient.h"

int HttpClient::max_upload_time = MAX_UPLOAD_TIME;

/**
 * @brief This function is the constructor for the class HttpClient.
 * This creates call back data.
 *
 * @return None.
 */
HttpClient::HttpClient()
	: curlSlist(NULL)
	, curlEasyHandle(NULL)
        , m_remainingTime(0)
        , dnsCacheTimeout(0)
        , m_uploadSpeed(0)
        , ca_cert_file(NULL)
        , cert_file(NULL)
	, m_SHAHASHHeader()
        , m_shaHashCode()
	, is_xpki_enabled(false)
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient constructor \n",__FILE__, __LINE__);

	cbData = new callbackData();

	if(NULL != cbData) {
		cbData->data = NULL;
		cbData->pos = 0;
		cbData->allocated = 0;
	}

}

/**
 * @brief This function is the destructor for the class HttpClient.
 * This deletes call back data.
 *
 * @return None.
 */
HttpClient::~HttpClient()
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient destructor \n",__FILE__, __LINE__);

	/* Make sure to close Http Client */
	close();

	if(cbData){
		delete cbData;
	}
}

/**
 * @brief This function is used to return the error string for the given HTTP error code.
 *
 * @param[in] http_code HTTP error code.
 *
 * @return Error string.
 */
const char *HttpClient::url_http_code_str(int http_code)
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): http_code: %d \n",__FILE__, __LINE__, http_code);

	switch (http_code)
	{
		case 100: return "100 Continue"; break;
		case 101: return "101 Switching Protocols"; break;

		case 200: return "200 OK"; break;
		case 201: return "201 Created"; break;
		case 202: return "202 Accepted"; break;
		case 203: return "203 Non-Authoritative Information"; break;
		case 204: return "204 No Content"; break;
		case 205: return "205 Reset Content"; break;
		case 206: return "206 Partial Content"; break;

		case 300: return "300 Multiple Choices"; break;
		case 301: return "301 Moved Permanently"; break;
		case 302: return "302 Moved Temporarily"; break;
		case 303: return "303 See Other"; break;
		case 304: return "304 Not Modified"; break;
		case 305: return "305 Use Proxy"; break;
		case 306: return "306 Unused"; break;
		case 307: return "307 Temporary Redirect"; break;

		case 400: return "400 Bad Request"; break;
		case 401: return "401 Unauthorized"; break;
		case 402: return "402 Payment Required"; break;
		case 403: return "403 Forbidden"; break;
		case 404: return "404 Not Found"; break;
		case 405: return "405 Method Not Allowed"; break;
		case 406: return "406 Not Acceptable"; break;
		case 407: return "407 Proxy Authentication Required"; break;
		case 408: return "408 Request Time-out"; break;
		case 409: return "409 Conflict"; break;
		case 410: return "410 Gone"; break;
		case 411: return "411 Length Required"; break;
		case 412: return "412 Precondition Failed"; break;
		case 413: return "413 Request Entity Too Large"; break;
		case 414: return "414 Request-URI Too Large"; break;
		case 415: return "415 Unsupported Media Type"; break;
		case 416: return "416 Requested Range Not Satisfiable"; break;
		case 417: return "417 Expectation Failed"; break;

		case 500: return "500 Internal Server Error"; break;
		case 501: return "501 Not Implemented"; break;
		case 502: return "502 Bad Gateway"; break;
		case 503: return "503 Service Unavailable"; break;
		case 504: return "504 Gateway Time-out"; break;
		case 505: return "505 HTTP Version not supported"; break;

		default:  return "Unknown"; break;
	}
}

/**
 * @brief This is callback function used by libcurl to wriite data
 *
 * @param[in] ptr    Pointer to received data.
 * @param[in] size   Size of data.
 * @param[in] nmemb  No. of members.
 * @param[in] stream Buffer to which data is written.
 *
 * @return size of data.
 */
size_t HttpClient::write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Callback function to write data \n",__FILE__, __LINE__);

	HttpClient *hClient = (HttpClient*)stream;
	struct callbackData *result = NULL;
	if(NULL != hClient) {
		result = (struct callbackData *)hClient->cbData;
	}

        if(NULL != result) {
		size_t required = result->pos + (size * nmemb) + 1;     // Include space for the null terminator.

		if((result->pos + (size * nmemb)) >= (result->allocated - 1))
		{
			size_t alloc_amount = MAX(required, (result->allocated << 1));
			RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Reallocating buffer to %d bytes from %d.  Required: %d \n", __FILE__, __LINE__, alloc_amount, result->allocated, required);
			result->data = (char *)realloc(result->data, alloc_amount);
			result->allocated = alloc_amount;
		}

		if(result->data)
		{
			memcpy(result->data + result->pos , ptr, size * nmemb);
			result->allocated = size * nmemb;
			result->pos += size * nmemb;
			result->data[result->pos] = '\0';
		}

		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): response buffer: %s - %d \n",__FILE__, __LINE__, result->data, result->pos);
	}
	return size * nmemb;
}


size_t HttpClient::headerCallback(void *buffer, size_t size, size_t numData, void *stream)
{
	HttpClient *hClient = (HttpClient*)stream;
	const char *headerName = "X-Hash-Code";
	//char *headerName = "Content-Length";
	char headerStr[MAX_HEADER_SIZE];
	// Get the data size in bytes
	int byteSize = size * numData;
	// Ignore large header lines that we don't want to handle
	if (byteSize >= MAX_HEADER_SIZE)
	return byteSize;
	// So copy and null terminate first
	strncpy(headerStr, (const char*)buffer, byteSize);
	headerStr[byteSize] = '\0';
	if (strncmp(headerStr, headerName, strlen(headerName)) == 0)
	{
		if(hClient)
		{
			strcpy(hClient->m_SHAHASHHeader, "");
			RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d):Header found and copying \n",__FILE__, __LINE__);
			strcpy(hClient->m_SHAHASHHeader, headerStr);
		}
	}
	if(hClient)
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d):headerCallback: Received header %s :: SHA_HASH Code [%s]\n",__FILE__, __LINE__,headerStr, hClient->m_SHAHASHHeader);
	return (byteSize);
}
/**
 * @brief This is callback function used by libcurl to wriite data
 *
 * @param[in] ptr    Pointer to received data.
 * @param[in] size   Size of data.
 * @param[in] nmemb  No. of members.
 * @param[in] stream Buffer to which data is written.
 *
 * @return size of data.
 */
size_t HttpClient::write_post_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Callback function to write data \n",__FILE__, __LINE__);

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): response buffer: %s \n",__FILE__, __LINE__, (char *)stream);
        return size * nmemb;
}

/**
 * @brief This function is used to start a libcurl easy session with SSL.
 * This function gets CURL easy handle that we use as input to other functions in the easy interface.
 *
 * @param[in] url URL to be set in the easy interface session.
 * @param[in] dnsCacheTimeout DNS cache timeout
 * @param[in] ca_cert_file CA Info file location
 * @param[in] cert_file SSL Cert file location
 * @param[in] key_file SSL Key file location
 *
 * @return None.
 */
void HttpClient::openWithSSL(const char* url,long dnsCacheTimeout, const char* ca_cert_file, const char* cert_file, const char* key_file)
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient Open \n",__FILE__, __LINE__);

        curlEasyHandle = curl_easy_init();

        curl_easy_setopt(curlEasyHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_DNS_CACHE_TIMEOUT, dnsCacheTimeout);

	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLKEYTYPE, "PEM");
	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERTTYPE, "PEM");

//	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERT, cert_file);
//      curl_easy_setopt(curlEasyHandle, CURLOPT_SSLKEY, key_file);
//      curl_easy_setopt(curlEasyHandle, CURLOPT_CAINFO, ca_cert_file);

	//Using CVR cert for this Server URL, may change later
	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERT,CERT_FILE);
	curl_easy_setopt(curlEasyHandle, CURLOPT_CAINFO, CA_CERT_FILE);

	curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYHOST, 2L);

        if(NULL != cbData) {
                cbData->data = (char *)malloc(BUFFER_SIZE);
                if (NULL != cbData->data) {
                        memset( cbData->data, 0x00, BUFFER_SIZE );
                }
                cbData->allocated = BUFFER_SIZE;
                cbData->pos = 0;
        }
}


/**
 * @brief This function is used to start a libcurl easy session.
 * This function gets CURL easy handle that we use as input to other functions in the easy interface.
 *
 * @param[in] url URL to be set in the easy interface session.
 * @param[in] dnsCacheTimeout DNS cache timeout
 * @param[in] ca_cert_file CA Info file location
 * @param[in] cert_file SSL Cert file location
 *
 * @return None.
 */
void HttpClient::open(const char* url, long dnsCacheTimeout1, int upload_time, const char* ca_cert_file1, const char* cert_file1)
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient Open \n",__FILE__, __LINE__);

	max_upload_time = upload_time;
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): max_upload_time: %d\n",__FILE__, __LINE__, max_upload_time);


	curlEasyHandle = curl_easy_init();
        dnsCacheTimeout = dnsCacheTimeout1;
        if ( getenv("XPKI") != NULL) {
	    RDK_LOG(RDK_LOG_INFO,"LOG.RDK.HTTPCLIENT","%s(%d): xpki is enabled\n",__FILE__, __LINE__);
            is_xpki_enabled = true;
	}
        if (is_xpki_enabled) {
	   RDK_LOG(RDK_LOG_INFO,"LOG.RDK.HTTPCLIENT","%s(%d): using xpki cert\n",__FILE__, __LINE__);
           ca_cert_file = ca_cert_file1;
           cert_file = XPKI_CERT_FILE; 
        }
        else {
	    RDK_LOG(RDK_LOG_INFO,"LOG.RDK.HTTPCLIENT","%s(%d): using normal cert\n",__FILE__, __LINE__);
            ca_cert_file = ca_cert_file1;
            cert_file = cert_file1;
        }
        if(NULL != cbData) {
        	cbData->data = (char *)malloc(BUFFER_SIZE);
                if (NULL != cbData->data) {
       			memset( cbData->data, 0x00, BUFFER_SIZE );
		}
        	cbData->allocated = BUFFER_SIZE;
        	cbData->pos = 0;
	}
}

/**
 * @brief This function is used to get data from the server.
 *
 * @param[in] url URL of the GET server.
 * @param[out] curlCode Return value for the curl command.
 *
 * @return Call back data.
 */
char *HttpClient::getResponse(const char *url, int *curlCode, int connecttimeout)
{
	if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return NULL;
	}

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Requesting/GET URL at %s\n",__FILE__, __LINE__, url);

        if (NULL != cbData) {
		cbData->allocated = BUFFER_SIZE;
		cbData->pos = 0;
	}

        if (!(cbData->data || curlEasyHandle))
        {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate HTTP structures\n", __FILE__, __LINE__);
                return NULL;
        }
	curlEasyHandle_reset();
	curlEasyHandle_initialize(url);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
	//No header in response
	curl_easy_setopt(curlEasyHandle, CURLOPT_HEADER, 0L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_response);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curlEasyHandle, CURLOPT_HEADERFUNCTION, headerCallback);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEHEADER, this);
	curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
	curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, connecttimeout );
	CURLcode curl_code = curl_easy_perform(curlEasyHandle);
	
	if (curl_code)
	{
		RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
		if(curlCode){
			*curlCode = (int)curl_code;
		}
		return NULL;
	}
	else
	{
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
		if(curlCode)
			*curlCode = 0;
		long http_response;
		curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
		if (http_response != 200 && http_response != 0)
		{
			if(curlCode)
				*curlCode = http_response;
			RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): Received HTTP response code: %ld: %s\n", __FILE__, __LINE__, http_response, url_http_code_str(http_response));
			memset(cbData->data, 0, cbData->allocated);
		}
	}
	return cbData->data;
}
/**
 * @brief This function is used to get data from the server.
 *
 * @param[in] url URL of the GET server.
 * @param[out] curlCode Return value for the curl command.
 *
 * @return Call back data.
 */
char *HttpClient::get(const char *url, int *curlCode)
{
        if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return NULL;
        }

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Requesting/GET URL at %s\n",__FILE__, __LINE__, url);

        if (NULL != cbData) {
                cbData->allocated = BUFFER_SIZE;
                cbData->pos = 0;
        }

        if (!(cbData->data || curlEasyHandle))
        {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate HTTP structures\n", __FILE__, __LINE__);
                return NULL;
        }
        curlEasyHandle_reset();
        curlEasyHandle_initialize(url);
        //curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_response);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
        curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, 5L );

        CURLcode curl_code = curl_easy_perform(curlEasyHandle);
        if (curl_code)
        {
                RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
                if(curlCode){
                        *curlCode = (int)curl_code;
                }
                return NULL;
        }
        else
        {
                RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
                if(curlCode)
                        *curlCode = 0;
                long http_response;
                curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
                if (http_response != 200 && http_response != 0)
                {
                        if(curlCode)
                                *curlCode = http_response;
                        RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): Received HTTP response code: %ld: %s\n", __FILE__, __LINE__, http_response, url_http_code_str(http_response));
                        memset(cbData->data, 0, cbData->allocated);
                }
        }

        return cbData->data;
}

/**
 * @brief This function is used to post data to server.
 *
 * @param[in] url URL of the POST server.
 * @param[in] data Data to be sent.
 * @param[out] response_code Return value for the curl command.
 *
 * @return Curl code for the post.
 */
int HttpClient::post(const char *url, const char *data, long *response_code)
{
        if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return -1;
        }

        if (NULL == data) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid Data \n", __FILE__, __LINE__);
                return -1;
        }

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Sending/POST URL to data = [%s] : %d\n", __FILE__, __LINE__, data, strlen(data));
	char *responseData = (char*)data;

	if (!curlEasyHandle)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate CURL structure\n", __FILE__, __LINE__);
		return -1;
	}
	curlEasyHandle_reset();
	curlEasyHandle_initialize_mutualtls(url);
	curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_POST, 1L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDSIZE, 0L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, 10L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_response);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, this);

	struct curl_slist *list_item;
	list_item = curlSlist;

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Listing the headers after set and url_post\n",__FILE__, __LINE__);
	while (NULL != list_item)
	{
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT", "%s\n", list_item->data);
		list_item = list_item->next;
	}

	/* Now run off and do what you've been told! */
	CURLcode curl_code = curl_easy_perform(curlEasyHandle);
	if (curl_code)
	{
		RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned code: %d (%s) on URL: \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
	}
	else
	{
		long http_response = 0;
		curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success:%d (%s) on URL: \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
		if(response_code) {
			*response_code = http_response;
		}
	}

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT"," Exiting %s\n", __FUNCTION__);
	return curl_code;
}

int HttpClient::ProgressCallback_thumbnail(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)

{
        time_t curtime = 0;
        //struct timeval tv;
	struct timespec ts;

        //gettimeofday(&tv, NULL);
	clock_gettime(CLOCK_REALTIME, &ts);
        //curtime=tv.tv_sec;
        curtime=ts.tv_sec;
        HttpClient *hClient = (HttpClient *)clientp;
        //icurrent_time = sc_linear_time(NULL);
        max_upload_time = 20;
        if ( curtime - hClient->m_startTime >= max_upload_time)
        {
          RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","Exceed the max upload time. currentTime=%d,startTime=%d\n",curtime ,hClient->m_startTime);
          return 1; //Returning a non-zero value from this callback will cause libcurl to abort the transfer and return
        }
        return 0;

}

int HttpClient::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)

{
	time_t curtime = 0;
	//struct timeval tv;
	struct timespec ts;

	//gettimeofday(&tv, NULL);
	clock_gettime(CLOCK_REALTIME, &ts);
	//curtime=tv.tv_sec;
	curtime=ts.tv_sec;
	HttpClient *hClient = (HttpClient *)clientp;
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","Inside HttpClient::ProgressCallback time diff =%d  and m_remainingTime %d\n",curtime - hClient->m_startTime,hClient->m_remainingTime);

	//icurrent_time = sc_linear_time(NULL);
	if ( curtime - hClient->m_startTime >= hClient->m_remainingTime)
        {
          RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","Exceed the max upload time. currentTime=%d,startTime=%d\n",curtime ,hClient->m_startTime);
	  return 1; //Returning a non-zero value from this callback will cause libcurl to abort the transfer and return
        }
  	return 0;

}

/**
 * @brief This function is used to post binary data to the RWS POST server.
 *
 * @param[in] url URL of the POST server.
 * @param[in] data Data to be sent.
 * @param[out] response_code Return value for the curl command.
 * @param[in] file_len Length of binary data.
 *
 * @return Curl code for the post.
 */
char* HttpClient::post_getdata(const char *url, const char *data, long *response_code, int connecttimeout)
{	
        if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return NULL;
        }

        if (NULL == data) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid Data \n", __FILE__, __LINE__);
                return NULL;
        }

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Sending/POST URL to data = [%s] : %d\n", __FILE__, __LINE__, data, strlen(data));
        char *responseData = (char*)data;

        if (!curlEasyHandle)
        {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate CURL structure\n", __FILE__, __LINE__);
                return NULL;
        }
        curlEasyHandle_reset();
        curlEasyHandle_initialize(url);
        //curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, responseData);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_response);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, connecttimeout);
	curl_easy_setopt(curlEasyHandle, CURLOPT_HEADER, 0);
        struct curl_slist *list_item;
        list_item = curlSlist;

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Listing the headers after set and url_post\n",__FILE__, __LINE__);
        while (NULL != list_item)
        {
                RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT", "%s\n", list_item->data);
                list_item = list_item->next;
        }

	CURLcode curl_code = curl_easy_perform(curlEasyHandle);
	
	if (curl_code)
	{
		RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
		if(curl_code) {
			*response_code = (int)curl_code;
		}
		return NULL;
	}
	else
	{
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success returned code: %d (%s) on URL: %s\n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code), url);
		
		long http_response;
		curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
		if(response_code) {
			*response_code = http_response;
		}

		if (http_response != 200 && http_response != 0)
		{
			RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): Received HTTP response code: %ld: %s\n", __FILE__, __LINE__, http_response, url_http_code_str(http_response));
			memset(cbData->data, 0, cbData->allocated);
		}
	}
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT"," Exiting %s\n", __FUNCTION__);

	return cbData->data;
}

/**
 * @brief This function is used to post binary data to the server.
 *
 * @param[in] url URL of the POST server.
 * @param[in] data Data to be sent.
 * @param[out] response_code Return value for the curl command.
 * @param[in] file_len Length of binary data.
 *
 * @return Curl code for the post.
 */
int HttpClient::post_binary(const char *url, const char *data, long *response_code,long file_len)
{
        //struct timeval tv;
	struct timespec ts;
	char arr[256];

        if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return -1;
        }

        if (NULL == data) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid Data \n", __FILE__, __LINE__);
                return -1;
        }

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Sending/POST URL to data = [%x] : %ld \n", __FILE__, __LINE__, data, file_len);
        char *responseData = (char*)data;

        if (!curlEasyHandle)
        {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate CURL structure\n", __FILE__, __LINE__);
                return -1;
        }
	curlEasyHandle_reset();
	curlEasyHandle_initialize_mutualtls(url);
        //curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_NOPROGRESS, 0);

        curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, responseData);
        curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDSIZE, file_len);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_post_response);
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, arr);
        curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, 20);
        curl_easy_setopt(curlEasyHandle, CURLOPT_PROGRESSDATA, this);
        curl_easy_setopt(curlEasyHandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback_thumbnail);

        struct curl_slist *list_item;
        list_item = curlSlist;

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Listing the headers after set and url_post\n",__FILE__, __LINE__);
        while (NULL != list_item)
        {
                RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT", "%s\n", list_item->data);
                list_item = list_item->next;
        }

        //start_time = sc_linear_time(NULL);
        //gettimeofday(&tv, NULL);
	clock_gettime(CLOCK_REALTIME, &ts);
        //m_startTime = tv.tv_sec;
        m_startTime = ts.tv_sec;
        /* Now run off and do what you've been told! */
        CURLcode curl_code = curl_easy_perform(curlEasyHandle);
        if (curl_code)
        {
                RDK_LOG(RDK_LOG_WARN,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned error code: %d (%s) \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
        }
        else
        {
                long http_response = 0;
		curl_off_t val;
		CURLcode res;
                curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
                RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success:%d (%s) \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
                if(response_code) {
                        *response_code = http_response;
                }
		/* check for bytes downloaded */ 
    		res = curl_easy_getinfo(curlEasyHandle, CURLINFO_SIZE_UPLOAD_T, &val);
    		if((CURLE_OK == res))
      			RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d):Data uploaded: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", __FILE__, __LINE__,val);
 
    		/* check for total download time */ 
		double total;
    		res = curl_easy_getinfo(curlEasyHandle, CURLINFO_TOTAL_TIME, &total);
    		if((CURLE_OK == res))
      			RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","Total upload time: %.lf sec.\n",total);
 
		 curl_off_t speed;
    		/* check for average download speed */ 
    		res = curl_easy_getinfo(curlEasyHandle, CURLINFO_SPEED_UPLOAD_T, &speed);
    		if((CURLE_OK == res))
		{
      			RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","Average upload speed: %" CURL_FORMAT_CURL_OFF_T " kbyte/sec.\n", speed / 1024);
			m_uploadSpeed = speed;
		}
        }

        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT"," Exiting %s\n", __FUNCTION__);
        return curl_code;
}

/**
 * @brief This function is used to post binary data to the server.
 *
 * @param[in] url URL of the POST server.
 * @param[in] data Data to be sent.
 * @param[out] response_code Return value for the curl command.
 * @param[in] file_len Length of binary data.
 *
 * @return Curl code for the post.
 */
int HttpClient::post_binary(const char *url, const char *data, long *response_code,long file_len,int remainingTime)
{

	char arr[256];

        if (NULL == url) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid URL \n", __FILE__, __LINE__);
                return -1;
        }

        if (NULL == data) {
                RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Invalid Data \n", __FILE__, __LINE__);
                return -1;
        }

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Sending/POST URL to data = [%x] : %ld \n", __FILE__, __LINE__, data, file_len);
	char *responseData = (char*)data;

	if (!curlEasyHandle)
	{
		RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.HTTPCLIENT","%s(%d): Could not allocate CURL structure\n", __FILE__, __LINE__);
		return -1;
	}


	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): remainingTime=%d \n", __FILE__, __LINE__,remainingTime);

	curlEasyHandle_reset();
	curlEasyHandle_initialize(url);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_NOPROGRESS, 0);

	curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, responseData);
        curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDSIZE, file_len);
	curl_easy_setopt(curlEasyHandle, CURLOPT_HTTPHEADER, curlSlist);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_post_response);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, arr);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_TIMEOUT, remainingTime);
	curl_easy_setopt(curlEasyHandle, CURLOPT_CONNECTTIMEOUT, remainingTime);
	curl_easy_setopt(curlEasyHandle, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt(curlEasyHandle, CURLOPT_PROGRESSFUNCTION, ProgressCallback);

	struct curl_slist *list_item;
	list_item = curlSlist;

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): Listing the headers after set and url_post\n",__FILE__, __LINE__);
	while (NULL != list_item)
	{
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT", "%s\n", list_item->data);
		list_item = list_item->next;
	}
	//struct timeval tv;
	struct timespec ts;
        //gettimeofday(&tv, NULL);
	clock_gettime(CLOCK_REALTIME, &ts);
	//m_startTime = tv.tv_sec;
	m_startTime = ts.tv_sec;
        m_remainingTime = remainingTime;
	/* Now run off and do what you've been told! */
	CURLcode curl_code = curl_easy_perform(curlEasyHandle);
	if (curl_code)
	{
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform returned code: %d (%s) on URL: \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
	}
	else
	{
		long http_response = 0;
		curl_easy_getinfo(curlEasyHandle, CURLINFO_RESPONSE_CODE, &http_response);
		RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): CURL perform success:%d (%s) on URL: \n", __FILE__, __LINE__, curl_code, curl_easy_strerror(curl_code));
		if(response_code) {
			*response_code = http_response;
		}
	}

	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT"," Exiting %s\n", __FUNCTION__);
	return curl_code;
}

/**
 * @brief This function is used to add a new header
 * This function appends the header string to list of headers used by curl.
 *
 * @param[in] headerStr Header string.
 * @param[in] headerVal Header value.
 *
 * @return None.
 */
void HttpClient::addHeader(const char *headerStr, const char * headerVal)
{
	RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient AddHeader \n",__FILE__, __LINE__);

	char headerString[MAX_HEADER_SIZE];
	memset(headerString, 0, sizeof(headerString));

	snprintf(headerString, sizeof(headerString), "%s: %s", headerStr, headerVal);
	headerString[MAX_HEADER_SIZE-1] = 0;
	curlSlist = curl_slist_append(curlSlist, headerString);
}

/**
 * @brief This function is used to get the SHA-HASH  header for the response.
 *
 * @return hashCode.
 */
char *HttpClient::getHashCode()
{
	sscanf(m_SHAHASHHeader, "X-Hash-Code: %s", m_shaHashCode);
	strcpy(m_SHAHASHHeader, "");
	return m_shaHashCode;
}

curl_off_t HttpClient::getUploadSpeed()
{
	return m_uploadSpeed;
}
/**
 * @brief This function is used to reset the header list.
 *
 * @param  None.
 *
 * @return None.
 */
void HttpClient::resetHeaderList()
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient ResetHeaderList \n",__FILE__, __LINE__);

        if(curlSlist) {
                curl_slist_free_all(curlSlist);
                curlSlist = NULL;
        }

}

void HttpClient::curlEasyHandle_initialize(const char* url) /*DELIA-19201*/
{
        curl_easy_setopt(curlEasyHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_DNS_CACHE_TIMEOUT, dnsCacheTimeout);
        curl_easy_setopt(curlEasyHandle, CURLOPT_CAINFO, ca_cert_file);
        curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERT, cert_file);
        if ( is_xpki_enabled ) {
                curl_easy_setopt(curlEasyHandle, CURLOPT_SSLKEYTYPE, "PEM");
                curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERTTYPE, "PEM");
                curl_easy_setopt(curlEasyHandle, CURLOPT_SSLKEY, XPKI_KEY_FILE);
                curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYPEER, 1L);
                curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYHOST, 2L);
        }
        curl_easy_setopt(curlEasyHandle, CURLOPT_FOLLOWLOCATION, 1L);
	//curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 0L);
}

void HttpClient::curlEasyHandle_initialize_mutualtls(const char* url) /*DELIA-19201*/
{
        curl_easy_setopt(curlEasyHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlEasyHandle, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curlEasyHandle, CURLOPT_DNS_CACHE_TIMEOUT, dnsCacheTimeout);
        curl_easy_setopt(curlEasyHandle, CURLOPT_CAINFO, ca_cert_file);
        curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERT, cert_file);
        curl_easy_setopt(curlEasyHandle, CURLOPT_FOLLOWLOCATION, 1L);

	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLKEYTYPE, "PEM");
	curl_easy_setopt(curlEasyHandle, CURLOPT_SSLCERTTYPE, "PEM");

	curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curlEasyHandle, CURLOPT_SSL_VERIFYHOST, 2L);
}

void HttpClient::curlEasyHandle_reset() /*DELIA-19201*/
{
	curl_easy_reset(curlEasyHandle);
}
/**
 * @brief This function is used to clean up curl interface.
 * It frees call back data, curl list and clean up curl easy handle.
 *
 * @return None.
 */
void HttpClient::close()
{
        RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.HTTPCLIENT","%s(%d): HttpClient Close \n",__FILE__, __LINE__);

	if(cbData->data) {
		free(cbData->data);
		cbData->data = NULL;
	}
	if(curlSlist) {
		curl_slist_free_all(curlSlist);
		curlSlist = NULL;
	}
	if(curlEasyHandle) {
		curl_easy_cleanup(curlEasyHandle);
		curlEasyHandle = NULL;
	}
}
