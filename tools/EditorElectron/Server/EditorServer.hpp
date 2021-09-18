#pragma once

#include "Scripting/Script.hpp"

class Server;

CLASS(EditorServer, Script)
{
	PRI(Server, NONE, Server *)
	PRI(MessageSent, NONE, bool)

public:

	void init() OVR;
	void firstUpdate() OVR;
	void update() OVR;
	void terminate() OVR;
};