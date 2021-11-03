#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

#include "Maths/Vector4.hpp"


class UIStyle: public ObjectBase
{
    GENERATE_METADATA(UIStyle)
    PUB_M(Vector4, TextColor, NONE);
	PUB_M(Vector4, BackgroundColor, NONE);
	PUB_M(Vector4, ColorPressed, NONE);
	PUB_M(Vector4, ColorHovered, NONE);

PUB

	UIStyle()
	{
        mTextColor = Vector4(1, 1, 1, 1);
		mBackgroundColor = Vector4(0, 0, 0, 1);
		mColorPressed = Vector4(0.3f, 0.3f, 0.3f, 1);
		mColorHovered = Vector4(0.7f, 0.7f, 0.7f, 1);
	}

	COPY(UIStyle)
	{
        DO_COPY(TextColor)
		DO_COPY(BackgroundColor)
		DO_COPY(ColorPressed)
		DO_COPY(ColorHovered)
	}
};

class UIStyleManager: public ObjectBase, public Singleton<UIStyleManager>
{
	GENERATE_METADATA(UIStyleManager)
	PRI_M(UIStyle, DefaultStyle, GETREF_CONST);
	PRI_M(SMap(ClassId, UIStyle*), Styles, NONE);

PUB

	virtual ~UIStyleManager() override;
	
	void init();

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	void addStyle()
	{
		MAP_INSERT(mStyles, T::getClassIdStatic(), NEW(T));
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	const T& getStyle()
	{
		ASSERT_MSG(MAP_CONTAINS(mStyles, T::getClassIdStatic()), "Style not found");

		return *(static_cast<T*>(mStyles.at(T::getClassIdStatic())));
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	const T& getOrAddStyle()
	{
		if(!MAP_CONTAINS(mStyles, T::getClassIdStatic()))
		{
			addStyle<T>();
		}
		
		return getStyle<T>();
	}
};