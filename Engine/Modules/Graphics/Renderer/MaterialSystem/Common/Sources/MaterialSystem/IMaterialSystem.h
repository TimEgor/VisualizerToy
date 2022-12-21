#pragma once


#include "IMaterial.h"
#include "Core/FileName/FileName.h"
#include "Multithreading/CountableObject/CountableReference.h"

#include <cstdint>


namespace VT
{
	class FileName;
	struct PipelineStateDesc;
	using MaterialID = uint32_t;
	using MaterialName = FileName;
	using MaterialNameID = FileNameID;

	class MaterialHandle : public CountableObjectBase
	{
	public:
		MaterialHandle() = default;

		virtual IMaterial* getMaterial() const = 0;
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(MaterialHandle, Material)

	class IMaterialSystem
	{
	public:
		IMaterialSystem() = default;
		virtual ~IMaterialSystem() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual MaterialReference createMaterial(const PipelineStateDesc& desc) = 0;
		virtual MaterialReference getMaterial(MaterialID id) = 0;
		virtual MaterialReference getNamedMaterial(const MaterialName& name) = 0;
		virtual MaterialReference getNamedMaterial(MaterialNameID id) = 0;

		virtual MaterialReference loadMaterial(const FileName& name) = 0;
		virtual MaterialReference loadMaterialAsync(const FileName& name) = 0;
	};
}
