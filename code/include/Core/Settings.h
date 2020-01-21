#ifndef DE_SETTINGS_H
#define DE_SETTINGS_H

#include "Basic.h"
#include "Singleton.h"

namespace DE{

  class Settings : public DE_Class, public Singleton<Settings> {

  public:

    DE_CLASS(Settings, DE_Class);

    void init();

    f32 mSceneSize;

    f32 mQuadTreeMinSize;
  };
}

#endif /* DE_SETTINGS_H */
