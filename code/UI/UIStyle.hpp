#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

#include "Maths/Vector4.hpp"


CLASS(UIStyle, ObjectBase)
{
	PUB(Vector4, Color, NONE);
	PUB(Vector4, ColorPressed, NONE);
	PUB(Vector4, ColorHovered, NONE);

public:

	UIStyle()
	{
		mColor = Vector4(0, 0, 0, 1);
		mColorPressed = Vector4(0.3f, 0.3f, 0.3f, 1);
		mColorHovered = Vector4(0.7f, 0.7f, 0.7f, 1);
	}

	COPY(UIStyle)
	{
		DO_COPY(Color)
		DO_COPY(ColorPressed)
		DO_COPY(ColorHovered)
	}
};

CLASS(UIStyleManager, ObjectBase), SINGLETON(UIStyleManager)
{
	PRI(UIStyle, DefaultStyle, GETREF_CONST);
	PRI(SMap(ClassId, UIStyle*), Styles, NONE);

public:

	VIR ~UIStyleManager() OVR;
	
	void init();

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	void addStyle()
	{
		MAP_INSERT(mStyles, T::getClassIdStatic(), NEW(T));
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	CNS T& getStyle()
	{
		ASSERT_MSG(MAP_CONTAINS(mStyles, T::getClassIdStatic()), "Style not found");

		return *(static_cast<T*>(mStyles.at(T::getClassIdStatic())));
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIStyle, T>::value> >
	CNS T& getOrAddStyle()
	{
		if(!MAP_CONTAINS(mStyles, T::getClassIdStatic()))
		{
			addStyle<T>();
		}
		
		return getStyle<T>();
	}
};