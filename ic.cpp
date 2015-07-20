#include "stdafx.h"
#include <stdint.h>
#include <conio.h>
#include "StopWatch.h"
#include "mmio.h"
#include "FileIoHelperClass.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	_ASSERTE(create_very_big_file(L"big.txt", 4000));

	FileIoHelper fih;
	LARGE_INTEGER size;
	LARGE_INTEGER offset;
	PUCHAR buf = (PUCHAR)malloc(4096);
	StopWatch sw2;

	size.QuadPart = (LONGLONG)4096;
	offset.QuadPart = (LONGLONG)0;
	sw2.Start();

	fih.FIOpenForRead(L"big.txt");
	fih.FIOReadFromFile(offset, 4096, buf);
	fih.FIOCreateFile(L"big.txt2", size);

	fih.FIOWriteToFile(offset, 4096, buf);

	sw2.Stop();
	print("info] time elapsed = %f", sw2.GetDurationSecond());

	StopWatch sw;
	sw.Start();
	_ASSERTE(file_copy_using_read_write(L"big.txt", L"big3.txt"));

	sw.Stop();
	print("info] time elapsed = %f", sw.GetDurationSecond());
	

	return 0;
}