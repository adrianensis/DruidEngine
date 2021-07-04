#include "Playground.hpp"

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
#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"

#include "ProjectileScript.hpp"

#include "Command/CommandLine.hpp"

void Playground::createSprite(const Vector2 &position, u32 type /* = 0*/)
{
	Vector2 size(150, 150);

	Material *material = MaterialManager::getInstance()->loadMaterial("resources/terrain.png");

	GameObject *sprite = NEW(GameObject);
	sprite->init();

	sprite->getTransform()->setLocalPosition(position);
	sprite->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer *renderer = NEW(Renderer);
	sprite->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	switch (type)
	{
	case 0:
		renderer->setMaterial(material);
		renderer->addAnimation("idle", Animation::create(10, true, false, Vector2(0, 0), 1.0f / 16.0f, 1.0f / 16.0f, 10));
		renderer->setAnimation("idle");
		break;
	case 1:
		renderer->setMaterial(material);
		renderer->addAnimation("idle", Animation::create(8, true, false, Vector2(8.0f / 16.0f, 10.0f / 16.0f), 1.0f / 16.0f, 1.0f / 16.0f, 3));
		renderer->setAnimation("idle");
		break;
	}

	//renderer->setIsLineMode(true);

	getGameObject()->getScene()->addGameObject(sprite);
}

void Playground::init()
{
	mTransform = getGameObject()->getTransform();
	mMaterial = nullptr;
}

void Playground::firstStep()
{
	CommandLine::getInstance()->registerCommand("create_sprite", [&](const Command& command)
	{		
		/*FOR_LIST(it, command.getArgs())
		{
			const CommandArgument& arg = *it;
			VAR("" + arg.getName())
			VAR("" + arg.getValue())
		}*/

		if(command.getArgs().size() == 2)
		{
			Vector2 position(
				std::stof(command.getArgs()[0].getName()),
				std::stof(command.getArgs()[1].getName())
			);
			createSprite(position, 0);
		}
	});

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL).
	setPosition(Vector2(-0.8f,0)).
	setLayer(0).
	setSize(Vector2(0.5f, 0.05f)).
	create(UIElementType::PANEL).
	getUIElement();

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setText("Text").
	setTextSize(Vector2(0.05f, 0.05f)).
	create(UIElementType::TEXT).
	getUIElement();

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(true).
	setText("Button").
	setTextSize(Vector2(0.05f, 0.05f)).
	create(UIElementType::BUTTON).
	getUIElement();

	UIDropdown* dropdown = UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(true).
	setText("Dropdown").
	setTextSize(Vector2(0.05f, 0.05f)).
	create(UIElementType::DROPDOWN).
	getUIElement<UIDropdown>();
	
	dropdown->addOption("Option 1", [](UIElement *uiElement)
	{

	});

	dropdown->addOption("Option 2", [](UIElement *uiElement)
	{

	});

	UIList* list = UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setTextSize(Vector2(0.05f, 0.05f)).
	setSize(Vector2(0.5f, 0.2f)).
	create(UIElementType::LIST).
	getUIElement<UIList>();

	list->addOption("Option 1", [](UIElement *uiElement)
	{

	});

	list->addOption("Option 2", [](UIElement *uiElement)
	{

	});

	list->addOption("Option 3", [](UIElement *uiElement)
	{

	});

	list->addOption("Option 4", [](UIElement *uiElement)
	{

	});

	list->toggle();

	/*mUIText = (UIText*) UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL)->
	setPosition(Vector2(0.5,0.5f))->
	setLayer(0)->
	setAdjustSizeToText(false)->
	setTextSize(Vector2(0.1f, 0.1f))->
	setText("hello")->
	create(UIElementType::TEXT)->
	getUIElement();*/

	CommandLine::getInstance()->open();
}

void Playground::step()
{
	//RenderEngine::getInstance()->drawLine(Vector3(-1000,0,0), Vector3(1000,0,0));
	//RenderEngine::getInstance()->drawLine(Vector3(0,-1000,0), Vector3(0,1000,0));
	
	if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		createSprite(Input::getInstance()->getMousePosition() * 800, 0);
		//mUIText->setText("left");

		/*UI::getInstance()->getUIBuilder().
		setLayout(UILayout::VERTICAL).
		setPosition(Input::getInstance()->getMousePosition()).
		setLayer(0).
		setAdjustSizeToText(false).
		setTextSize(Vector2(0.2f, 0.2f)).
		setText("OpenGL").
		create(UIElementType::TEXT).
		getUIElement();*/

		/*UI::getInstance()->getUIBuilder().
		setLayout(UILayout::VERTICAL).
		setPosition(Input::getInstance()->getMousePosition()).
		setLayer(0).
		setAdjustSizeToText(true).
		setText("Button!").
		setTextSize(Vector2(0.1f, 0.1f)).
		create(UIElementType::BUTTON).
		getUIElement();*/
	}

	if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		createSprite(Input::getInstance()->getMousePosition() * 800, 1);
		//mUIText->setText("right  123");

		/*UI::getInstance()->getUIBuilder().
		setLayout(UILayout::VERTICAL).
		setPosition(Input::getInstance()->getMousePosition()).
		setLayer(0).
		setAdjustSizeToText(true).
		setText("Button-longer").
		setTextSize(Vector2(0.2f, 0.2f)).
		create(UIElementType::BUTTON).
		getUIElement();*/
	}
}

void Playground::terminate()
{
}