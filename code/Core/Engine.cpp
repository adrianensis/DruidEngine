#include "Core/ClassManager.hpp"
#include "Core/Memory.hpp"
#include "Core/Engine.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/Singleton.hpp"
#include "Graphics/Graphics.hpp"
#include "Scripting/ScriptEngine.hpp"

#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"
#include "UI/UI.hpp"
#include "Command/CommandLine.hpp"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

#include "generated-code/code.generated.class.hpp"

void Engine::init()
{
	REGISTER_CLASSES_BY_NAME();

	mFPS = 60;

	Memory::init();

	ClassManager::getInstance().init();

	Profiler::getInstance().init();

	EngineConfig::getInstance().init();
	RenderContext::init();
	Input::getInstance().init();
	TimerManager::getInstance().init();
	EventsManager::getInstance().init();
	MaterialManager::getInstance().init();
	UIManager::getInstance().init();
	ScenesManager::getInstance().init();

	CommandLine::getInstance().init();
}

void Engine::initSubsystems()
{
	f32 sceneSize = ScenesManager::getInstance().getCurrentScene()->getSize();
	RenderEngine::getInstance().init(sceneSize);
	ScriptEngine::getInstance().init();
}

void Engine::terminateSubSystems()
{
	ScriptEngine::getInstance().terminate();
	RenderEngine::getInstance().terminate();

	TimerManager::getInstance().terminate();
}

void Engine::run()
{
	Time::getInstance().init();

	f32 inverseFPS = 1.0f / mFPS;
	f32 inverseFPSMillis = inverseFPS * 1000.0f;

	f32 diff = 0;

	while (!RenderContext::isClosed())
	{
		Time::getInstance().startFrame();

		if (ScenesManager::getInstance().getSceneHasChanged())
		{
			terminateSubSystems();
			ScenesManager::getInstance().loadCurrentScene();
			initSubsystems();
		}

		Input::getInstance().pollEvents();

		CommandLine::getInstance().update();

		ScenesManager::getInstance().update();
		TimerManager::getInstance().update();
		ScriptEngine::getInstance().update();
		RenderEngine::getInstance().update();

		f32 dtMillis = Time::getInstance().getElapsedTimeMillis();
		
		if (inverseFPSMillis > dtMillis)
		{
			diff = inverseFPSMillis - dtMillis;
			auto diff_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double, std::milli>(diff));
			std::this_thread::sleep_for(std::chrono::milliseconds(diff_duration.count()));
		}
		
		std::cout << std::round(1.0f/Time::getInstance().getDeltaTimeSeconds()) << std::endl;

		Profiler::getInstance().update(Time::getInstance().getElapsedTimeMillis());

		Time::getInstance().endFrame();
	}
}

void Engine::terminate()
{
	TRACE();
	
	CommandLine::getInstance().terminate();
	CommandLine::deleteInstance();

	ScenesManager::deleteInstance();

	terminateSubSystems();

	UIManager::getInstance().terminate();
	UIManager::deleteInstance();

	MaterialManager::deleteInstance();

	ScriptEngine::deleteInstance();
	RenderEngine::deleteInstance();
	EventsManager::getInstance().terminate();
	EventsManager::deleteInstance();
	TimerManager::deleteInstance();
	Time::deleteInstance();
	Input::deleteInstance();
	RenderContext::terminate();

	EngineConfig::deleteInstance();

	SubsystemsManager::deleteInstance();

	Profiler::getInstance().terminate();

	ClassManager::deleteInstance();

	Memory::terminate();
}