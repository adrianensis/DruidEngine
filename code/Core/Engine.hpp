#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class RenderEngine;
class PhysicsEngine;
class ScriptEngine;

class Engine: public ObjectBase, public Singleton<Engine> {

private:

	 f32 mFPS;
	 RenderEngine* mRenderEngine;
	 PhysicsEngine* mPhysicsEngine;
	 ScriptEngine* mScriptEngine;

public:

	GENERATE_METADATA(Engine);

	Engine();
	virtual ~Engine() override;

	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};
}

