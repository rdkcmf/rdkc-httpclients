/* vi: set sw=4 ts=4: */
/*
 * crypt() for uClibc
 * Copyright (C) 2000-2006 by Erik Andersen <andersen@uclibc.org>
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <unistd.h>
#include <crypt.h>
#include <stdio.h>
#include <ctype.h>
#ifdef __cplusplus
extern "C"{
#endif
#include "sysUtils.h"
#ifdef __cplusplus
}
#endif
#define MACKEY_STRING_LEN	32
#define MAC_STRING_LEN	12
#define STRING_LEN	520

char *crypt(const char *key, const char *salt)
{
	const unsigned char *ukey = (const unsigned char *)key;
	const unsigned char *usalt = (const unsigned char *)salt;
	return __md5_crypt(ukey, usalt);
}

int main()
{
	FILE *fp = NULL;
	if ((fp = fopen("/tmp/passwd", "w")) == NULL) {
		printf("Couldn't open file \n");
		return 1;
	}

	char passwd_str[STRING_LEN];
	char *encrypted_pwd = NULL;
	char mac_key[MACKEY_STRING_LEN] = {0};
	char mac[MAC_STRING_LEN] = {0};
	result_string_t device_mac = {0};
	int ret = -1;
	ret = exec_sys_command("mfrApi_test 3 9 | sed -e 's/://g'", &device_mac);

	if(ret == 0) {
		//Convert mac to lower case
		for(int i = 0; device_mac.ptr[i] && isxdigit(device_mac.ptr[i]); i++) {
			mac[i] = tolower(device_mac.ptr[i]);
		}

		if(MAC_STRING_LEN != strlen(mac)) {
			fclose(fp);
			printf("Invalid value\n");
			return 1;
		}

		sprintf(mac_key, "%s%s", mac, "encrypt");
		// printf("Crypt input string: %s\n", mac_key);
		encrypted_pwd = crypt(mac_key, "rdkcencr");
		if(NULL == encrypted_pwd) {
			fclose(fp);
			printf("Encryption failed\n");
			return 1;
		}
		printf("Encryption successful\n");

		/*write the value to the file*/
		memset(passwd_str, 0, 0x201);
		snprintf(passwd_str, 0x201, "%s%s%s", "root:", encrypted_pwd, ":0:0:root:/root:/bin/sh\n");
		fprintf(fp, "%s", passwd_str);
	}
	fclose(fp);
	return 0;
}
