#include "RenderingData.h"

void VT::RenderingData::clear()
{
	m_transforms.clear();
	m_meshes.clear();
}

void VT::RenderingData::addMesh(const MeshConstReference& mesh, const Matrix44& transform)
{
	m_transforms.push_back(transform);
	m_meshes.emplace_back(mesh, m_transforms.size() - 1);
}
