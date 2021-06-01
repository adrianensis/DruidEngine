#pragma once

#include "Core/Core.hpp"
#include "Core/Serialization.hpp"

#include <netinet/in.h>
#include <functional>
#include <string>

enum class ServerStatus {
	UNINITIALIZED,
	LISTENING,
	CONNECTED
};

#define DEFAULT_SOCKET_READ_SIZE 1024

CLASS(Server, ObjectBase){

	PRI(Status, GET, ServerStatus);

private:

	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	mutable std::string buffer;

	JSON readSimpleJSON(u32 size = DEFAULT_SOCKET_READ_SIZE) const;

public:
	void init(u16 port);

	bool checkConnectionRequest();

	void writeData(const std::string& data) const;
	std::string readData(u32 size = DEFAULT_SOCKET_READ_SIZE) const;

	JSON readJSON() const;
	void writeJSON(JSON& json) const;
};