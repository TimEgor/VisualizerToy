#pragma once

#include "MaterialSystem/IMaterialSystem.h"

namespace VT
{
	using MaterialHandleID = uint32_t;

	class ManagedMaterialHandle : public MaterialHandle
	{
	protected:
		IMaterial* m_material = nullptr;
		MaterialHandleID m_id = 0;

		virtual void selfDestroy() override;

	public:
		ManagedMaterialHandle(IMaterial* material, MaterialHandleID id)
			: m_material(material), m_id(id) {}

		virtual IMaterial* getMaterial() const override { return m_material; }
		void setMaterial(IMaterial* material) { m_material = material; }

		MaterialHandleID getID() const { return m_id; }
	};

	class NamedManagedMaterialHandle final : public ManagedMaterialHandle
	{
	private:
		MaterialNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedMaterialHandle(IMaterial* material, MaterialHandleID id, MaterialNameID nameID)
			: ManagedMaterialHandle(material, id), m_nameID(nameID) {}

		MaterialNameID getNameID() const { return m_nameID; }
	};
}
