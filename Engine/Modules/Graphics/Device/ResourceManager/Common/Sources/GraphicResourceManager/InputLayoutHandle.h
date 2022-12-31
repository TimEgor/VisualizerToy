#pragma once

#include "InputLayout/IInputLayout.h"
#include "Multithreading/CountableObject/CountableReference.h"

#include <cstdint>


namespace VT
{
	using InputLayoutHandleID = uint16_t;

	class InputLayoutHandle : public CountableObjectBase
	{
	protected:
		InputLayoutDesc m_desc;
		InputLayoutHandleID m_id = 0;

	public:
		InputLayoutHandle(const InputLayoutDesc& desc, InputLayoutHandleID id)
			: m_desc(desc), m_id(id) {}

		const InputLayoutDesc& getDesc() const { return m_desc; }
		InputLayoutHandleID getID() const { return m_id; }
	};


	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(InputLayoutHandle, InputLayout)
}
