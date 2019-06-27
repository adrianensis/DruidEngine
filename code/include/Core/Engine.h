#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "DE_Class.h"
#include "Singleton.h"

namespace DE {

template <class T> class List;
class Scene;
class RenderEngine;
class ScriptEngine;

// ---------------------------------------------------------------------------

DE_CLASS_SINGLETON(Engine)

  DE_PRI DE_MEMBER(f32,           FPS);
  DE_PRI DE_MEMBER(u32,           CurrentSceneIndex);
  DE_PRI DE_MEMBER(List<Scene*>*, Scenes);
  DE_PRI DE_MEMBER(RenderEngine*, RenderEngine);
  DE_PRI DE_MEMBER(ScriptEngine*, ScriptEngine);

  DE_PRI void loadScene(Scene* scene);

  DE_PUB void init();
  DE_PUB void addScene(Scene* newScene);
  DE_PUB void setScene(u32 i);
  DE_PUB void run();
  DE_PUB void terminate();

DE_END_CLASS;

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* DE_ENGINE_H */
