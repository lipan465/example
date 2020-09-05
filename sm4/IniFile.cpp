#include "stdafx.h"
#include "IniFile.h"
#include "utilitylib.h"
#include "sm4/sm4.h"

#include <cstdint>
#include <fstream>
#include <sstream>

unsigned char IniKey[] = "avscRDCF4es@1234";

std::string CIniFile::EncryptString(unsigned char key[] ,const std::string input)
{
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

std::string CIniFile::DecryptString(unsigned char key[] ,const std::string input)
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

CIniFile::CIniFile(LPCTSTR filePath, bool isEncrypt)
{
	_isEncrypt = isEncrypt;
	if(_isEncrypt) {
		TCHAR tmpDir[MAX_PATH] = {0};
		TCHAR tmpFile[MAX_PATH] = {0};
		GetTempPath(MAX_PATH, tmpDir);
		GetTempFileName(tmpDir, _T("tmp"), 0, tmpFile);
		_IniFilePath = tmpFile;
		_OrgFilePath = filePath;

		std::stringstream buffer;  
		std::ifstream ifs(YaoUtil::StrUtil::WStrToStr(filePath), std::ios::in | std::ios::binary);
		if (ifs && ifs.is_open()) {
			buffer << ifs.rdbuf();  
		}

		std::string encStr = DecryptString(IniKey, buffer.str());

		std::ofstream ofs(YaoUtil::StrUtil::WStrToStr(_IniFilePath), std::ios::out | std::ios::binary);
		if (ofs && ofs.is_open()) {
			ofs.write(encStr.c_str(), encStr.size());
		}
	}
	else {
		_OrgFilePath = _IniFilePath = filePath;
	}
}

void CIniFile::SaveFile(LPCTSTR filePath, bool isEncrypt)
{
	if(isEncrypt) {
		std::stringstream buffer;  
		std::ifstream ifs(YaoUtil::StrUtil::WStrToStr(_IniFilePath), std::ios::in | std::ios::binary);
		if (ifs && ifs.is_open()) {
			buffer << ifs.rdbuf();  
		}

		std::string encStr = EncryptString(IniKey, buffer.str());

		std::ofstream ofs(YaoUtil::StrUtil::WStrToStr(filePath), std::ios::out | std::ios::binary);
		if (ofs && ofs.is_open()) {
			ofs.write(encStr.c_str(), encStr.size());
		}
	}
	else {
		::CopyFile(_IniFilePath.c_str(), filePath, FALSE);
	}
}

CIniFile::~CIniFile(void)
{
	if(_isEncrypt) {
		SaveFile(_OrgFilePath.c_str(), true);
		::DeleteFile(_IniFilePath.c_str());
	}
}

void CIniFile::ReadIniFileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::wstring& value)
{
	WCHAR buf[1024] = {0};
	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf, 1024, _IniFilePath.c_str());
	value = buf;
}

void CIniFile::ReadIniFileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::string& value)
{
	WCHAR buf[1024] = {0};
	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf, 1024, _IniFilePath.c_str());
	value = YaoUtil::StrUtil::WStrToStr(buf);
}

void CIniFile::WriteIniFileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR Value)
{
	WritePrivateProfileString(lpAppName,lpKeyName,Value, _IniFilePath.c_str());
}

void CIniFile::WriteIniFileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int Value)
{
	TCHAR buf[100] = {0};
	_stprintf_s(buf,_T("%d"),Value);
	WriteIniFileString(lpAppName,lpKeyName,buf);
}

void CIniFile::ReadIniFileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault, int &nReturnValue)
{
	nReturnValue = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, _IniFilePath.c_str());
}

void CIniFile::WriteIniFileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int64_t Value)
{
	TCHAR buf[100] = {0};
	_stprintf_s(buf,_T("%ld"),Value);
	WriteIniFileString(lpAppName,lpKeyName,buf);
}

void CIniFile::ReadIniFile(LPCTSTR lpAppName, LPCTSTR lpKeyName, float fDefault, float &fReturnValue)
{
	TCHAR buf[100] = {0};
	TCHAR lpDefault[100] = {0};
	_stprintf_s(lpDefault,_T("%f"),fDefault);
	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf, 100, _IniFilePath.c_str());
	fReturnValue = (float)_wtof(buf);
}

void CIniFile::ReadIniFileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int64_t nDefault, int64_t &nReturnValue)
{
	TCHAR buf[100] = {0};
	TCHAR lpDefault[100] = {0};
	_stprintf_s(lpDefault,_T("%ld"),nDefault);
	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf, 100, _IniFilePath.c_str());
	nReturnValue = _wtoi64(buf);
}
