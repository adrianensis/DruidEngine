#include "Core/Engine.hpp"
#include "Scenes/Scenes.hpp"
#include "Scripting/Scripting.hpp"

#include "Editor.hpp"

#include "generated-code/tools/Editor/generated.class.hpp"

/*#include <ft2build.h>
#include FT_FREETYPE_H */

ENGINE_MAIN()
{
	GameObject *controller = NEW(GameObject);
	controller->init();

	// NOTE: this script won't be added to the ScriptEngine in the usual way,
	// since ScriptEngine itself is still not initialized.
	Script *script = NEW(Editor);
	script->init();
	controller->addComponent<Script>(script);

	ScenesManager::getInstance().setGameObjectController(controller);
}
