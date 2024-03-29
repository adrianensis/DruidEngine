#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

#include "Maths/Vector4.hpp"

#include <map>

CLASS(UIStyle, ObjectBase)
{
	PUB(Color, NONE, Vector4);
	PUB(ColorPressed, NONE, Vector4);
	PUB(ColorHovered, NONE, Vector4);

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
	PRI(DefaultStyle, GETREF_CONST, UIStyle);
	PRI(Styles, NONE, std::map<ClassId, UIStyle*>);

public:

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