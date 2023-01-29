#pragma once

#include "Multithreading/CountableObject/CountableReference.h"

#include <type_traits>

namespace VT
{
	class IGraphicObject;

	template <typename HandleIDType>
	class GraphicObjectHandle : public CountableObjectBase
	{
	public:
		using HandleID = HandleIDType;
		using HandleObjectType = IGraphicObject;

		static_assert(std::is_integral<HandleIDType>::value);

	protected:
		IGraphicObject* m_objectPtr = nullptr;
		HandleIDType m_id = 0;

	public:
		GraphicObjectHandle() = default;
		GraphicObjectHandle(IGraphicObject* object, HandleIDType handleID)
			: m_objectPtr(object), m_id(handleID) {}

		IGraphicObject* getObject() const { return m_objectPtr; }

		template <typename Type>
		Type* getObjectCast() const
		{
			static_assert(std::is_base_of_v<IGraphicObject, Type>);

			return reinterpret_cast<Type*>(getObject());
		}

		HandleIDType getID() const { return m_id; }
	};

	template <typename ObjectType, typename HandleIDType>
	class GraphicTypedObjectHandle : public GraphicObjectHandle<HandleIDType>
	{
	public:
		using HandleObjectType = ObjectType;

	public:
		GraphicTypedObjectHandle() = default;
		GraphicTypedObjectHandle(ObjectType* object, HandleIDType handleID)
			: GraphicObjectHandle(object, handleID) {}

		ObjectType* getTypedObject() const { return getObjectCast<ObjectType>(); }
	};

#define GRAPHIC_OBJECT(NAME, HANDLE_ID_TYPE)									\
	using NAME##GraphicObjectHandle = GraphicObjectHandle<HANDLE_ID_TYPE>;		\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##GraphicObjectHandle, NAME)

#define GRAPHIC_TYPED_OBJECT(NAME, INTERFACE_TYPE, HANDLE_ID_TYPE)									\
	class INTERFACE_TYPE;																			\
	using NAME##GraphicObjectHandle = GraphicTypedObjectHandle<INTERFACE_TYPE, HANDLE_ID_TYPE>;		\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##GraphicObjectHandle, NAME)
}