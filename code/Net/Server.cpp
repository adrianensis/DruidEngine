#include "Net/Server.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void Server::init(u16 port)
{
	TRACE()

	mStatus = ServerStatus::UNINITIALIZED;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	int flags = fcntl(server_fd, F_GETFL);

	if (flags < 0)
	{
		perror("could not get flags on TCP listening socket");
	}

	if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("could not set TCP listening socket to be non-blocking");
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 1) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	mStatus = ServerStatus::LISTENING;

	/*if ((new_socket)<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}*/
	/*valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");*/
}

bool Server::checkConnectionRequest()
{
	new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t *)&addrlen);

	bool result = false;
	if (new_socket < 0)
	{
		if (errno == EWOULDBLOCK)
		{
			//perror("No pending connections.\n");
		}
		else
		{
			perror("error when accepting connection");
		}

		result = false;
	}
	else
	{
		perror("NEW Connection accepted.\n");
		mStatus = ServerStatus::CONNECTED;
		result = true;
	}

	return result;
}

void Server::writeData(const SStr &data) const
{
	int sent = send(new_socket, data.c_str(), data.size(), 0);

	if (sent < 0)
	{
		perror("Error sending.\n");
	}
}

SStr Server::readData(u32 size /* = DEFAULT_SOCKET_READ_SIZE*/) const
{
	buffer.clear();
	buffer.resize(size);
	int readed = read(new_socket, &buffer[0], size);

	if (readed < 0)
	{
		perror("Error sending.\n");
	}

	return buffer;
}

JSON Server::readJSON() const
{
	JSON json = readSimpleJSON();

	if (json.contains("__size"))
	{
		json = readSimpleJSON(json["__size"]);
	}

	return json;
}

JSON Server::readSimpleJSON(u32 size /* = DEFAULT_SOCKET_READ_SIZE*/) const
{
	JSON json;
	SStr data = readData(size);

	if (!data.empty())
	{
		ECHO(data);

		json = JSON::parse(data);

		if (!json.empty())
		{
			ECHO(json.dump());
		}
	}

	return json;
}

void Server::writeJSON(JSON &json) const
{
}
