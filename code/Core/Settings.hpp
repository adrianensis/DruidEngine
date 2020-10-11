#ifndef DE_SETTINGS_H
#define DE_SETTINGS_H

#include "Basic.hpp"
#include "Singleton.hpp"
#include "ConfigMap.hpp"
#include "File.hpp"

namespace DE {

template<class K, class V> class HashMap;

class Settings: public DE_Class, public Singleton<Settings> {

private:
	ConfigMap* mConfigMap;

public:

	DE_CLASS(Settings, DE_Class);

	void init();
	void readConfigFile(const std::string &path);
	std::string getString(const std::string &key);
	u32 getU32(const std::string &key);
	f32 getF32(const std::string &key);
	bool getBool(const std::string &key);
};
}

#endif /* DE_SETTINGS_H */
