#ifndef DE_SETTINGS_H
#define DE_SETTINGS_H

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"
#include "Core/ConfigMap.hpp"
#include "File/File.hpp"

namespace DE {

template<class K, class V> class HashMap;

class Settings: public DE_Class, public Singleton<Settings> {

private:
	DE_M(ConfigMap, ConfigMap*)

public:

	DE_CLASS(Settings, DE_Class)

	void init();
	void readConfigFile(const std::string &path);
	std::string getString(const std::string &key);
	u32 getU32(const std::string &key);
	f32 getF32(const std::string &key);
	bool getBool(const std::string &key);
};
}

#endif /* DE_SETTINGS_H */
