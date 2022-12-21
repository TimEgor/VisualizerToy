#pragma once

#include "MaterialSystem/IMaterialSystem.h"
#include "Material.h"
#include "MaterialHandle.h"
#include "NamedReferencePool/NamedObjectPool.h"
#include "NamedReferencePool/NamedObjectPoolHandle.h"
#include "ObjectPool/ThreadSafeObjectPool.h"

namespace VT
{
	class MaterialSystem final : public IMaterialSystem
	{
		friend ManagedMaterial;
		friend ManagedMaterialHandle;
		friend NamedManagedMaterialHandle;

		using MaterialPool = ThreadSafeObjectPool<Material, ObjectPoolHandle32>;
		using MaterialHandlePool = ThreadSafeObjectPool<ManagedMaterialHandle, NamedObjectPoolHandle32>;
		using NamedMaterialHandlePool = NamedObjectPool<NamedManagedMaterialHandle, MaterialReference, NamedObjectPoolHandle32>;

	private:
		MaterialPool m_materials;
		MaterialHandlePool m_handles;
		NamedMaterialHandlePool m_namedHandles;

		IMaterial* createMaterialInternal(const PipelineStateDesc& desc);

		void deleteMaterial(MaterialID id);
		void deleteMaterialHandle(MaterialHandleID id);
		void deleteNamedMaterialHandle(MaterialNameID id);

	public:
		MaterialSystem() = default;
		~MaterialSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual MaterialReference createMaterial(const PipelineStateDesc& desc) override;
		virtual MaterialReference getMaterial(MaterialID id) override;
		virtual MaterialReference getNamedMaterial(const MaterialName& name) override;
		virtual MaterialReference getNamedMaterial(MaterialNameID id) override;

		virtual MaterialReference loadMaterial(const FileName& name) override;
		virtual MaterialReference loadMaterialAsync(const FileName& name) override;
	};
}