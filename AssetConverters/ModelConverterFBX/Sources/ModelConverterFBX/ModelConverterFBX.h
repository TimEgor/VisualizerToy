#pragma once

#include "ResourceConverter/IResourceConverter.h"

#include "fbxsdk.h"

#define VT_RESOURCE_CONVERTER_FBX

namespace VT_MODEL_RC
{
	class ModelConverterFBX final : public VT::IFileResourceConverter
	{
	private:
		FbxManager* m_fbxManager = nullptr;
		FbxIOSettings* m_fbxIOSettings = nullptr;

		bool convertModel(void* data, size_t dataSize, VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);
		bool convertModel(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);
		bool convertModel(FbxImporter* fbxImporter, VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);

	public:
		ModelConverterFBX() = default;
		~ModelConverterFBX() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void convert(void* inData, size_t inDataSize,
			void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args) override;
		virtual bool convert(void** inData, size_t& inDataSize,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;
		virtual void convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			void** outData, size_t& outDataSize, const VT::IResourceConverterArgs* args) override;
		virtual bool convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;
		virtual void convert(const VT::IFileSystem& inFileSystem, const VT::FileName& inFilePath,
			void** outData, size_t& outDataSize,
			VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath, const VT::IResourceConverterArgs* args) override;

		virtual const char* getFileExtensionPattern() const override { return "*.fbx"; }
		virtual const char* getTargetFileExtension() const override { return ".model"; }

		virtual VT::ResourceConverterVersion getVersion() const override;

		VT_RESOURCE_CONVERTER_TYPE(VT_RESOURCE_CONVERTER_FBX)
	};
}