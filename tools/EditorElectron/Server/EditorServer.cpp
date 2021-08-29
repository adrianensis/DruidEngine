#include "Server/EditorServer.hpp"

#include "Core/Time/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"

#include "Net/Server.hpp"

#include "Core/Serialization.hpp"

void EditorServer::init()
{
	mServer = new Server();
}

void EditorServer::firstUpdate()
{
	mServer->init(8080);

	mMessageSent = false;
}

void EditorServer::update()
{
	if (mServer->getStatus() == ServerStatus::LISTENING)
	{
		mServer->checkConnectionRequest();
	}

	if (mServer->getStatus() == ServerStatus::CONNECTED)
	{
		//mServer->writeData("hello from server");
		JSON json = mServer->readJSON();
	}
}

void EditorServer::terminate()
{
}