#include "Setup.h"

Setup::Setup()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	errorCheck(iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

Setup::Setup(int Flags = AI_PASSIVE, int Family = AF_INET, int SockType = SOCK_STREAM, int Protocol = IPPROTO_TCP)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	errorCheck(iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = Flags;
	hints.ai_family = Family;
	hints.ai_socktype = SockType;
	hints.ai_protocol = Protocol;
}

SOCKET Setup::bindSocket(SOCKET &Socket, PCSTR ListenAddr, PCSTR Port)
{
	iResult = getaddrinfo(ListenAddr, Port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	iResult = bind(Socket, result->ai_addr, (int)result->ai_addrlen);
	errorCheck(iResult);
	freeaddrinfo(result);
	return Socket;
}

int Setup::Listen(SOCKET &Socket)
{
	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Setup::AcceptConnection(SOCKET &ListenSocket, std::vector<SOCKET>& Clients)
{
	do
	{
		SOCKET ClientSocket = INVALID_SOCKET;
		ClientSocket = accept(ListenSocket, hints.ai_addr, (int*)hints.ai_addrlen);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		Clients.push_back(ClientSocket);
		ClientSocket = INVALID_SOCKET;
		printf("Clients: %ld\n", Clients.size());
	} while (Clients.size() < 10);
	return 0;
}

int Setup::errorCheck(int &iResult)
{
	if (iResult != 0)
	{
		printf("Error: %ld\n", WSAGetLastError());
		return 1;
	}
	return 0;
}
