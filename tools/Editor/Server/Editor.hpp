#pragma once

#include "Scripting/Script.hpp"

class Server;

CLASS(Editor, Script) {

	PRI(Server, NONE, Server*)
	PRI(MessageSent, NONE, bool)

public:

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};