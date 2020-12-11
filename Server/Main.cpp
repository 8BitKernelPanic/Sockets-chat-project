#include "Setup.h"

int main()
{
	// Create vector of clients and listen socket
	std::vector<SOCKET> Clients = {};
	SOCKET Socket = INVALID_SOCKET;
	Setup setup;
	setup.bindSocket(Socket, NULL, "80");
	setup.Listen(Socket);

	// Start a seperate thread for accepting connections so that we can go on and process existing clients.
	std::thread cThread(&Setup::AcceptConnection, &setup, std::ref(Socket), std::ref(Clients));

	//Write the program loop. But first design it. Then refactor the code and make it look pretty and be better.

	cThread.join();
	return 0;
}