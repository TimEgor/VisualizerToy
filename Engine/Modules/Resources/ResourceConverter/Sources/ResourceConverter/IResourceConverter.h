#pragma once

#include "Core/FileName/FileName.h"
#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	class IResourceConverterArgs;

	using ResourceConverterType = uint32_t;
	using ResourceConverterVersion = uint32_t;

	class IResourceConverter
	{
	public:
		IResourceConverter() = default;
		virtual ~IResourceConverter() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void convert(void* inData, size_t inDataSize,
			void** outData, size_t& outDataSize, const IResourceConverterArgs* args) = 0;

		virtual ResourceConverterType getType() const = 0;
		virtual ResourceConverterVersion getVersion() const = 0;
	};

	class IFileSystem;

	class IFileResourceConverter : public IResourceConverter
	{
	public:
		IFileResourceConverter() = default;

		virtual bool convert(void** inData, size_t& inDataSize,
			IFileSystem& outFileSystem, const FileName& outFilePath, const IResourceConverterArgs* args) = 0;
		virtual void convert(const IFileSystem& inFileSystem, const FileName& inFilePath,
			void** outData, size_t& outDataSize, const IResourceConverterArgs* args) = 0;
		virtual bool convert(const IFileSystem& inFileSystem, const FileName& inFilePath,
			IFileSystem& outFileSystem, const FileName& outFilePath, const IResourceConverterArgs* args) = 0;
		virtual void convert(const IFileSystem& inFileSystem, const FileName& inFilePath,
			void** outData, size_t& outDataSize,
			IFileSystem& outFileSystem, const FileName& outFilePath, const IResourceConverterArgs* args) = 0;

		virtual const char* getFileExtensionPattern() const = 0;
		virtual const char* getTargetFileExtension() const = 0;
	};
}


#define VT_RESOURCE_CONVERTER_TYPE(RES_CONVERTER_TYPE)																		\
	static inline constexpr VT::ResourceConverterType getResourceConverterType() { return VT::crc32(#RES_CONVERTER_TYPE); }	\
	virtual VT::ResourceConverterType getType() const override { return getResourceConverterType(); }