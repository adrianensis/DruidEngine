#pragma once

#include "Core/ObjectBase.hpp"

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

public:
	void init(u16 port);

	bool checkConnectionRequest();

	void writeData(const std::string& data) const;
	std::string readData(u32 size = DEFAULT_SOCKET_READ_SIZE) const;

	void onData(std::function<void(std::ifstream &file)> callback);
};