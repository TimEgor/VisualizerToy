#include "ManagedInputLayoutHandle.h"

#include "ManagedResourceHandleReleaseDeclaration.h"

void VT::ManagedInputLayoutHandle::selfDestroy()
{
	getResourceManager()->deleteInputLayoutReference(m_id);
}
