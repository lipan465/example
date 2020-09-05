#include "sm4.h"
#include <string>
#include <string.h>
#include <stdio.h>
unsigned char IniKey[] = "avscRDCF4es@1234";

std::string EncryptString(unsigned char key[] ,const std::string& input) {
#if 0
	sm4_context ctx;
	int length = input.size();
	unsigned char iv[16] = {0};
	unsigned char* encryptOut = new unsigned char[length];

	sm4_setkey_enc(&ctx, GlobalConfig::getInstance()->getSecretKey());
	sm4_crypt_cbc(&ctx, SM4_ENCRYPT, input.size(), iv, (unsigned char*)input.c_str(), encryptOut);

	std::string outStr;
	outStr.reserve(length * 2);

	for(int i=0;i<length;i++) {
		char tmp[3] = {0};
		sprintf_s(tmp, "%02X", encryptOut[i]);
		outStr += tmp;
	}
	delete[] encryptOut;
	
	return outStr;
#else
	sm4_context ctx;
	int length = 0;
	unsigned char iv[16] = {0};
	int pad = 0;
#if 0 // ZERO PAD
	length = (input.size() + 15)/16*16;
	pad = 0;
#else // PKCS5
	length = (input.size()/16 + 1 )*16;
	if(input.size()%16 != 0) pad = length - input.size();
	else pad = 16;
#endif
	memset(iv, '0', sizeof(iv));
	unsigned char* encryptIn = new unsigned char[length];
	unsigned char* encryptOut = new unsigned char[length];

	memset(encryptIn, pad, length);
	memset(encryptOut, 0, length);

	memcpy(encryptIn, input.c_str(), input.size());

	//sm4_setkey_enc(&ctx, GlobalConfig::getInstance()->getSecretKey());
	sm4_setkey_enc(&ctx, key);
	sm4_crypt_cbc(&ctx, SM4_ENCRYPT, length, iv, encryptIn, encryptOut);

	std::string tmpStr;
	//Util::base64Encode(encryptOut, length, tmpStr);
	tmpStr.assign((char*)encryptOut, length);
	delete[] encryptIn;
	delete[] encryptOut;

	return tmpStr;
#endif
}

std::string DecryptString(unsigned char key[] ,const std::string& input)
{
#if 0
	std::string tmpStr;
	sm4_context ctx;
	int length = input.size()/2;
	unsigned char iv[16] = {0};
	unsigned char* decryptIn = new unsigned char[length];
	unsigned char* decryptOut = new unsigned char[length];

	char *strEnd;
	for(int i=0;i<length;i++) {
		string subs = input.substr(i*2, 2);
		decryptIn[i] = strtol(subs.c_str(),&strEnd,16);
	}

	sm4_setkey_dec(&ctx, GlobalConfig::getInstance()->getSecretKey());
	sm4_crypt_cbc(&ctx, SM4_DECRYPT, length, iv, (unsigned char*)decryptIn, decryptOut);

	
	tmpStr.assign((char*)decryptOut, length);

	delete[] decryptIn;
	delete[] decryptOut;
	return tmpStr;
#else
	//std::string tmpStr;
	sm4_context ctx;
	unsigned char iv[16] = {0};

	memset(iv, '0', sizeof(iv));

	//Util::base64Decode(input, tmpStr);

	int length = input.size();
	unsigned char* decryptIn = new unsigned char[length];
	unsigned char* decryptOut = new unsigned char[length];
	int pad = 0;
	
	memset(decryptIn, 0, length);
	memset(decryptOut, 0, length);

	memcpy(decryptIn, input.c_str(), input.size());

	//sm4_setkey_dec(&ctx, GlobalConfig::getInstance()->getSecretKey());
	sm4_setkey_dec(&ctx, key);
	sm4_crypt_cbc(&ctx, SM4_DECRYPT, length, iv, decryptIn, decryptOut);

	pad = decryptOut[length - 1];
	if(pad < 0 || pad > 16) return "";

	std::string tmpStr;
	tmpStr.assign((char*)decryptOut, length - pad);

	delete[] decryptIn;
	delete[] decryptOut;
	return tmpStr;
#endif
}

	unsigned long GetTickCount()
    {
#ifndef _MSC_VER
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else
		return ::GetTickCount();
#endif
    }


int main() 
{
    char* buf = new char[1024*1024];

    unsigned long start = GetTickCount();
    std::string str = EncryptString(IniKey, std::string(buf, 1024*1024));
    printf("%d %d\n",str.size(), GetTickCount() - start);
    return 0;
}
