#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"

namespace DE {

class CustomScript : public Script{
private:

public:

DE_CLASS(CustomScript, Script);

void init() override;
void update() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
