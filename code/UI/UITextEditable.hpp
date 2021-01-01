#ifndef DE_UITEXTEDITABLE_H
#define DE_UITEXTEDITABLE_H

#include "UI/UIText.hpp"

namespace DE {

class UITextEditable: public UIText {

public:

	DE_CLASS(UITextEditable)

	virtual void init();
	virtual void onFocusLost();

};

} /* namespace DE */

#endif /* DE_UITEXTEDITABLE_H */
