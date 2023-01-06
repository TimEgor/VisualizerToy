#pragma once

#include "fbxsdk.h"
#include "FileSystem/IFileSystem.h"

#include <sstream>


namespace VT_MODEL_RC
{
	class FbxFileSystemReadStream final : public FbxStream
	{
	private:
		FbxManager* m_manager = nullptr;

		const VT::IFileSystem& m_fileSystem;
		VT::IFileSystem::IReadStream* m_stream = nullptr;

	public:
		FbxFileSystemReadStream(FbxManager* manager, const VT::IFileSystem& fileSystem)
			: m_manager(manager), m_fileSystem(fileSystem) {}
		~FbxFileSystemReadStream() { Close(); }

		virtual EState GetState() override
		{
			return m_stream && m_stream->isOpen() ? FbxStream::eOpen : FbxStream::eClosed;
		}

		virtual bool Open(void* path) override
		{
			m_stream = m_fileSystem.openBinaryReadStream(reinterpret_cast<const char*>(path));
			return m_stream && m_stream->isOpen();
		}

		virtual bool Close() override
		{
			if (m_stream && m_stream->isOpen())
			{
				m_fileSystem.closeReadStream(m_stream);
				m_stream = nullptr;

				return true;
			}

			return false;
		}

		virtual bool Flush() override
		{
			return true;
		}

		virtual size_t Write(const void* pData, FbxUInt64 pSize) override
		{
			assert(false && "FbxFileSystemReadStream should be used only for reading.");
			return 0;
		}

		virtual size_t Read(void* pData, FbxUInt64 pSize) const override
		{
			if (m_stream && m_stream->isOpen())
			{
				return m_stream->read(pData, pSize);
			}

			return 0;
		}

		virtual int GetReaderID() const override
		{
			const char* format = "FBX (*.fbx)";
			return m_manager->GetIOPluginRegistry()->FindReaderIDByDescription(format);
		}

		virtual int GetWriterID() const override
		{
			const char* format = "FBX (*.fbx)";
			return m_manager->GetIOPluginRegistry()->FindWriterIDByDescription(format);
		}

		virtual void Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos) override
		{
			VT::IFileSystem::IStream::SeekDir dir;

			switch (pSeekPos)
			{
			case FbxFile::eBegin:
				dir = VT::IFileSystem::IStream::SeekDir::BEGIN;
				break;
			case FbxFile::eCurrent:
				dir = VT::IFileSystem::IStream::SeekDir::CURRENT;
				break;
			case FbxFile::eEnd:
				dir = VT::IFileSystem::IStream::SeekDir::END;
				break;
			}

			m_stream->seek(pOffset, dir);
		}

		virtual FbxInt64 GetPosition() const override
		{
			if (m_stream && m_stream->isOpen())
			{
				return m_stream->getPos();
			}

			return 0;
		}
		virtual void SetPosition(FbxInt64 pPosition) override
		{
			if (m_stream && m_stream->isOpen())
			{
				m_stream->seek(pPosition, VT::IFileSystem::IStream::SeekDir::BEGIN);
			}
		}

		virtual int GetError() const override
		{
			if (m_stream && m_stream->isOpen())
			{
				return m_stream->getError();
			}

			return 0;
		}

		virtual void ClearError() override {}
	};
}
