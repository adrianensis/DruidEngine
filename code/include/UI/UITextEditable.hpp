#ifndef DE_UITEXTEDITABLE_H
#define DE_UITEXTEDITABLE_H

#include "UIText.hpp"

namespace DE {

class UITextEditable: public UIText {

public:

	DE_CLASS(UITextEditable, UIText);

	virtual void init();

};

} /* namespace DE */

#endif /* DE_UITEXTEDITABLE_H */
