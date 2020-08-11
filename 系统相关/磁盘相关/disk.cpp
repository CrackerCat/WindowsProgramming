#include "disk.h"

void GetDiskDriverVolumnName()
{
	DWORD dwSize = GetLogicalDriveStrings(0, nullptr);


	if (dwSize != 0)
	{		
		LPTSTR lp = new TCHAR [dwSize * sizeof(TCHAR)];

		GetLogicalDriveStrings(dwSize * sizeof(TCHAR), lp);

		LPTSTR lpTemp = lp;

		while (*lp != 0)
		{
			UINT nRes = GetDriveType(lp);

			if (nRes == DRIVE_FIXED)
			{
				LPTSTR lpNameBuf = new TCHAR[12];
				DWORD dwNameSize = 12;
				DWORD dwSerialNumber;
				DWORD dwMaxLen;
				DWORD dwFileFlag;
				LPTSTR lpSysNameBuf = new TCHAR[10];
				DWORD dwSysNameSize = 10;
				GetVolumeInformation(lp, lpNameBuf, dwNameSize, &dwSerialNumber, &dwMaxLen, &dwFileFlag, lpSysNameBuf, dwSysNameSize);
				wcout << lp <<"  "<< lpNameBuf<< " "<< hex << dwSerialNumber  <<endl;

				/*
				* hex ʮ������
				* oct �˽���
				* dec ʮ���� 
				*/

				delete[] lpSysNameBuf;
				delete[] lpNameBuf;
			}

			lp = wcschr(lp, 0) + 1;
		}

		lp = lpTemp;
		delete[] lp;
	}

}

void FormatDriverByDriverName(LPTSTR driver)
{
	UINT driveID = driver[0] - 'A';
	//����δ���ã����������
	//SHFormatDrive(GetConsoleWindow(),driveID, SHFMT_ID_DEFAULT,0);
}

void GetDiskDriverSpaceByDriverName(LPTSTR driver)
{
	ULARGE_INTEGER nFreeBytesAvailableToCaller;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER nTotalNumberOfFreeBytes;
	auto result = GetDiskFreeSpaceEx(driver, &nFreeBytesAvailableToCaller, &nTotalNumberOfBytes, &nTotalNumberOfFreeBytes);

	if (result != 0)
	{
		std::wstringstream wss;
		wss << "Drive:" << driver << " total size: "
			<< nTotalNumberOfBytes.QuadPart << " bytes "
		    << "available size: "
			<< nTotalNumberOfFreeBytes.QuadPart << " bytes ";
		wcout << wss.str() << endl;		
	}
}
