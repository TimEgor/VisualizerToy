#include "ModelConverterFBX.h"

#include "Core/UtilitiesMacros.h"
#include "FbxMemoryStream.h"
#include "FbxFileSystemReadStream.h"
#include "FbxHierData.h"

#include <cassert>

#include "Core/Platform.h"

bool VT_MODEL_RC::ModelConverterFBX::convertModel(void* data, size_t dataSize,
                                                  VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	FbxMemoryStream inputDataStream(m_fbxManager, data, dataSize);

	FbxImporter* fbxImporter = FbxImporter::Create(m_fbxManager, "fbxImporter");
	if (!fbxImporter->Initialize(&inputDataStream, nullptr, -1, m_fbxIOSettings))
	{
		return false;
	}

	return convertModel(fbxImporter, outFileSystem, outFilePath);
}

bool VT_MODEL_RC::ModelConverterFBX::convertModel(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	FbxFileSystemReadStream inputDataStream(m_fbxManager, inFileSystem);

	FbxImporter* fbxImporter = FbxImporter::Create(m_fbxManager, "fbxImporter");
	if (!fbxImporter->Initialize(&inputDataStream, const_cast<char*>(inFilePath.c_str()), -1, m_fbxIOSettings))
	{
		return false;
	}

	return convertModel(fbxImporter, outFileSystem, outFilePath);
}

bool VT_MODEL_RC::ModelConverterFBX::convertModel(FbxImporter* fbxImporter,
                                                  VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	FbxScene* fbxScene = FbxScene::Create(m_fbxManager, "fbxScene");
	if (!fbxImporter || !fbxScene)
	{
		return false;
	}

	if (!fbxImporter->Import(fbxScene))
	{
#ifdef _MSC_VER
		OutputDebugStringA("FBX Import scene failed: ");
		OutputDebugStringA(fbxImporter->GetStatus().GetErrorString());
		OutputDebugStringA("\n");
#endif // _MSC_VER

		return false;
	}

	FbxNode* sceneRoot = fbxScene->GetRootNode();
	if (!sceneRoot)
	{
		fbxImporter->Destroy();
		return false;
	}

	ModelHier hier;
	hier.buildHier(sceneRoot);

	hier.save(outFileSystem, outFilePath);

	fbxImporter->Destroy();

	return true;
}

bool VT_MODEL_RC::ModelConverterFBX::init()
{
	m_fbxManager = FbxManager::Create();
	VT_CHECK_INITIALIZATION(m_fbxManager);

	m_fbxIOSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	VT_CHECK_INITIALIZATION(m_fbxIOSettings);

	m_fbxManager->SetIOSettings(m_fbxIOSettings);

	return true;
}

void VT_MODEL_RC::ModelConverterFBX::release()
{
	if (m_fbxManager)
	{
		m_fbxManager->Destroy();
		m_fbxManager = nullptr;
	}

	m_fbxIOSettings = nullptr;

}

void VT_MODEL_RC::ModelConverterFBX::convert(void* inData, size_t inDataSize, void** outData, size_t& outDataSize,
	const VT::IResourceConverterArgs* args)
{
	assert(false);
}

bool VT_MODEL_RC::ModelConverterFBX::convert(void** inData, size_t& inDataSize, VT::IFileSystem& outFileSystem,
	const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args)
{
	void* outData = nullptr;
	size_t outDataSize = 0;
	convertModel(inData, inDataSize, outFileSystem, outFilePath);

	if (outData && outDataSize > 0)
	{
		return outFileSystem.writeResource(outFilePath, outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
	}

	return false;
}

void VT_MODEL_RC::ModelConverterFBX::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args)
{
	assert(false);
}

bool VT_MODEL_RC::ModelConverterFBX::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args)
{
	void* outData = nullptr;
	size_t outDataSize = 0;
	convertModel(inFileSystem, inFilePath, outFileSystem, outFilePath);

	if (outData && outDataSize > 0)
	{
		return outFileSystem.writeResource(outFilePath, outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
	}

	return false;
}

void VT_MODEL_RC::ModelConverterFBX::convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
	void** outData, size_t& outDataSize, VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath,
	const VT::IResourceConverterArgs* args)
{
	convertModel(inFileSystem, inFilePath, outFileSystem, outFilePath);

	if (outData && *outData && outDataSize > 0)
	{
		outFileSystem.writeResource(outFilePath, *outData, outDataSize, VT::WriteResourceFileFlag::OVERRIDE);
	}
}

VT::ResourceConverterVersion VT_MODEL_RC::ModelConverterFBX::getVersion() const
{
	return 2022'01'15;
}
