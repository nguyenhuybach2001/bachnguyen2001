#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8000);

	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		printf("Ket noi khong thanh cong - %d\n", ret);
		return 1;
	}

	//in thông tin tên máy chu
	struct Computer
	{
		char name[MAX_COMPUTERNAME_LENGTH + 1];
	};
	DWORD cchComputerName = MAX_COMPUTERNAME_LENGTH + 1;
	Computer computer = { 0 };

	if (!GetComputerNameA(&computer.name[0], &cchComputerName))
	{
		printf("Failed to get computer name, Error: %u\r\n", GetLastError());
	}
	else
	{
		printf("Computer Name: %s\r\n", computer.name);
	}

	//tra ve danh sach ky tu cua o dia
	char driveletter;
	WCHAR rootpath[] = L"?:\\";
	DWORD validdrives = GetLogicalDrives();
	for (UINT i = 0; i < 26; ++i)
	{
		if (validdrives & (1 << i))
		{
			driveletter = 'A' + i;
			rootpath[0] = driveletter;
			UINT drivetype = GetDriveTypeW(rootpath);
			printf("Drive %c (%ls) is type %u\n", driveletter, rootpath, drivetype);
			if (drivetype == DRIVE_CDROM)
			{
				printf("CD-Rom drive letter %c\n", driveletter);
			}
		}
	}

	//tra ve kich thuoc cua o dia

	BOOLEAN bSuccess;

	DWORD dwSectorsPerCluster;
	DWORD dwBytesPerSector;
	DWORD dwFreeClusters;
	DWORD dwTotalClusters;

	//get disk space for current drive
	bSuccess = GetDiskFreeSpace(
		NULL, //current drive
		&dwSectorsPerCluster, //sectors per cluster
		&dwBytesPerSector, //bytes per sector
		&dwFreeClusters, //free clusters
		&dwTotalClusters //total clusters
	);

	if (!bSuccess) {
		printf("Could not get drive information.\n");
		exit(EXIT_FAILURE);
	}

	printf("Bytes per sector: %d\n", dwBytesPerSector);
	printf("Sectors per cluster: %d\n", dwSectorsPerCluster);
	printf("Free clusters: %d\n", dwFreeClusters);
	printf("Total clusters: %d\n", dwTotalClusters);

	unsigned int uiKBPerCluster = dwBytesPerSector * dwSectorsPerCluster / 1024;

	printf("You have %.2f MB of free storage\n", (double)uiKBPerCluster * dwFreeClusters / 1024);
	printf("You have %.2f MB of total storage\n", (double)uiKBPerCluster * dwTotalClusters / 1024);

	return 0;

	closesocket(client);
	WSACleanup();

}