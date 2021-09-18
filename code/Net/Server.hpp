#pragma once

#include "Core/Core.hpp"
#include "Core/Serialization.hpp"

#include <netinet/in.h>

enum class ServerStatus
{
	UNINITIALIZED,
	LISTENING,
	CONNECTED
};

#define DEFAULT_SOCKET_READ_SIZE 1024

CLASS(Server, ObjectBase)
{
	PRI(Status, GET, ServerStatus);

private:
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	mutable SStr buffer;

	JSON readSimpleJSON(u32 size = DEFAULT_SOCKET_READ_SIZE) CNS;

public:
	void init(u16 port);

	bool checkConnectionRequest();

	void writeData(CNS SStr &data) CNS;
	SStr readData(u32 size = DEFAULT_SOCKET_READ_SIZE) CNS;

	JSON readJSON() CNS;
	void writeJSON(JSON & json) CNS;
};