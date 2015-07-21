#include "stdafx.h"
#include <stdint.h>
#include <conio.h>
#include "StopWatch.h"
#include "mmio.h"
#include "FileIoHelperClass.h"

int _tmain(int argc, _TCHAR* argv[])
{
	uint32_t filesz = 4096;

	_ASSERTE(create_very_big_file(L"big.txt", filesz));
	
	FileIoHelper fih;
	LARGE_INTEGER size;
	LARGE_INTEGER offset;
	PUCHAR buf = (PUCHAR)malloc(4096);
	DWORD byte_to_read = 0;
	StopWatch sw2;
	
	size.QuadPart = (LONGLONG)filesz * (LONGLONG)(1024 * 1024);
	offset.QuadPart = (LONGLONG)0;
	sw2.Start();

	fih.FIOpenForRead(L"big.txt");
	fih.FIOCreateFile(L"big2.txt", size);

	while (offset.QuadPart < size.QuadPart){
		if ((size.QuadPart - offset.QuadPart) >(LONGLONG)4096){
			byte_to_read = 4096;

			fih.FIOReadFromFile(offset, byte_to_read, buf);
			fih.FIOWriteToFile(offset, byte_to_read, buf);
		}
		else{
			offset.QuadPart += (LONGLONG)(size.QuadPart - offset.QuadPart);
			byte_to_read = (DWORD)(size.QuadPart - offset.QuadPart);
		}

		offset.QuadPart += (LONGLONG)4096;

	}

	sw2.Stop();
	print("info] time elapsed = %f", sw2.GetDurationSecond());
	
	StopWatch sw;
	sw.Start();
	_ASSERTE(file_copy_using_read_write(L"big.txt", L"big3.txt"));

	sw.Stop();
	print("info] time elapsed = %f", sw.GetDurationSecond());

	return 0;
}