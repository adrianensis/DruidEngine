#pragma once

#include "Scripting/Script.hpp"

class Server;

CLASS(EditorServer, Script)
{
	PRI(Server, NONE, Server *)
	PRI(MessageSent, NONE, bool)

public:

	void init() override;
	void firstUpdate() override;
	void update() override;
	void terminate() override;
};