#ifndef DE_SETTINGS_H
#define DE_SETTINGS_H

#include "Basic.h"
#include "Singleton.h"
#include "File.h"

namespace DE{

  template <class K, class V> class HashMap;

  class Settings : public DE_Class, public Singleton<Settings> {

  private:
    HashMap<std::string, std::string>* mSettingsMap;

  public:

    DE_CLASS(Settings, DE_Class);

    void init();
    void readConfigFile(const std::string& path);
    void writeConfigFile(const std::string& path, HashMap<std::string,std::string>* settinsMap);
    const std::string& getString(const std::string& key);
    u32 getU32(const std::string& key);
    f32 getF32(const std::string& key);
  };
}

#endif /* DE_SETTINGS_H */
