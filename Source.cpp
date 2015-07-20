#include <stdio.h>
#include <stdarg.h>
#include <Strsafe.h>
#include <stdlib.h>
#include <memory>
#include "windows.h"
#include <locale.h>
#include "WinNT.h"

typedef unsigned int uint32_t;

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}


/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	wchar_t *buf2 = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CzreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	// 파일에 데이터 쓰기
	DWORD bytes_written = 0;
	wchar_t string_buf[1024];
	char utf8[500] = { 0, };
	
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세1")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}
	
	//유니코드 --> utf-8 로 변환

	int nLen = WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), utf8, nLen, NULL, NULL);

	//_wsetlocale(LC_ALL, L"korean");
	//wprintf(L"%s\n", string_buf);
	//wprintf(L"한글ABC");
	if (!WriteFile(file_handle, utf8, strlen(utf8), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}
	
	// 영어로 쓰기
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"All work and no play makes jack a dull boy.1")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	//유니코드 --> utf8로 변환
	char utfw[500] = { 0, };

	nLen = WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, string_buf, lstrlenW(string_buf), utfw, nLen, NULL, NULL);

	if (!WriteFile(file_handle, utfw, strlen(utfw), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}
	

	
	
	char string_bufa[1024];
	wchar_t string_uni[500] = { 0, };

	if (!SUCCEEDED(StringCbPrintfA(
		string_bufa,
		sizeof(string_bufa),
		"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	//멀티 --> 유니코드 변환

	char utfa[500] = { 0, };

	int nLen2 = MultiByteToWideChar(CP_ACP, 0, string_bufa, strlen(string_bufa), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, string_bufa, strlen(string_bufa), string_uni, nLen2);

	//유니 --> utf8

	nLen2 = WideCharToMultiByte(CP_UTF8, 0, string_uni, lstrlenW(string_uni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, string_uni, lstrlenW(string_uni), utfa, nLen2, NULL, NULL);

	if (!WriteFile(file_handle,utfa, strlen(utfa), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}
	
	// 영어로 쓰기
	if (!SUCCEEDED(StringCbPrintfA(
		string_bufa,
		sizeof(string_bufa),
		"All work and no play makes jack a dull boy.")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	nLen2 = MultiByteToWideChar(CP_ACP, 0, string_bufa, strlen(string_bufa), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, string_bufa, strlen(string_bufa), string_uni, nLen2);

	//유니 --> utf8

	char utf8a[500] = { 0, };

	nLen2 = WideCharToMultiByte(CP_UTF8, 0, string_uni, lstrlenW(string_uni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, string_uni, lstrlenW(string_uni), utf8a, nLen2, NULL, NULL);

	if (!WriteFile(file_handle, utf8a, strlen(utf8a), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 파일 닫기
	CloseHandle(file_handle);

	uint32_t buflen2 = 0;
	buflen2 = GetCurrentDirectoryW(buflen2, buf2);
	if (0 == buflen2)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf2 = (PWSTR)malloc(sizeof(WCHAR)* buflen2);
	if (0 == GetCurrentDirectoryW(buflen2, buf2))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf2);
		return false;
	}

	wchar_t file_name2[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name2,
		sizeof(file_name2),
		L"%ws\\bob2.txt",
		buf2)))
	{
		print("err, can not create file name");
		free(buf2);
		return false;
	}
	free(buf2); buf2 = NULL;

	CopyFileW(file_name, file_name2, TRUE);

	char bufr[500] = { 0, };
	wchar_t tow[500] = { 0, };
	DWORD readn;

	struct sTest
	{
		int nCount;
		char CharName[200];

		sTest()
		{
			nCount = 0;
			memset(CharName, 0, sizeof(CharName));
		}
	};

	// 파일 생성
	HANDLE file_handle2 = CreateFileW(
		file_name2,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file_handle2 == INVALID_HANDLE_VALUE)
	{
		print("err, CzreateFile(path=%ws), gle=0x%08x", file_name2, GetLastError());
		return false;
	}

	ReadFile(file_handle2,bufr,499,&readn,NULL);

	int nLen3 = MultiByteToWideChar(CP_UTF8, 0, bufr, strlen(bufr), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, bufr, strlen(bufr), tow, nLen3);

	setlocale(LC_ALL, "Korean");
	wprintf(L"%s\n", tow);

	//memory mapped io.

	TCHAR *PtrInFile;
	wchar_t tow2[500] = { 0, };

	HANDLE Hmap = CreateFileMapping(file_handle2, NULL, PAGE_READONLY, 0, 0, NULL);

	if (Hmap == NULL)
	{
		MessageBoxA(0, "CreateFileMapping Error", "", 0);
		exit(0);
	}

	PtrInFile = (TCHAR*)MapViewOfFile(Hmap,FILE_MAP_READ,0,0,0);
	if (PtrInFile == NULL)
	{
		MessageBoxA(0, "MapViewOfFile Error", "", 0);
		exit(0);
	}
	char *ex = (char*)PtrInFile;

	nLen3 = MultiByteToWideChar(CP_UTF8, 0, ex, strlen(ex), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, ex, strlen(ex), tow2, nLen3);

	setlocale(LC_ALL, "Korean");
	wprintf(L"%s\n", tow2);

	UnmapViewOfFile(PtrInFile);
	CloseHandle(Hmap);
	CloseHandle(file_handle2);

	//파일삭제
	DeleteFileW(file_name2);
	DeleteFileW(file_name);

	//에러
	TCHAR errMes[1024];
	DWORD err;

	err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, errMes, 1024, NULL);

	wprintf(L"Error Code = %d, Message = %s", err, errMes);

	
	return true;
}

void main()
{
	char c;

	create_bob_txt();

	//scanf("%c", &c);
	return;
}