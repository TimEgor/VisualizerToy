#pragma once

#include "fbxsdk.h"

#include <sstream>

namespace VT_MODEL_RC
{
	class FbxMemoryStream final : public FbxStream
	{
	private:
		FbxManager* m_manager = nullptr;

		void* m_data = nullptr;
		size_t m_dataSize = 0;

		mutable std::stringstream m_stream;
		bool m_isStreamOpened = false;


	public:
		FbxMemoryStream(FbxManager* manager, void* data, size_t dataSize)
			: m_manager(manager), m_data(data), m_dataSize(dataSize) {}
		~FbxMemoryStream() { Close(); }

		virtual EState GetState() override
		{
			return m_isStreamOpened ? FbxStream::eOpen : FbxStream::eClosed;
		}

		virtual bool Open(void*) override
		{
			if (!m_isStreamOpened)
			{
				m_stream.str(std::string(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(m_data) + m_dataSize));
				m_isStreamOpened = true;
			}

			return true;
		}

		virtual bool Close() override
		{
			m_stream.str("");
			m_isStreamOpened = false;

			return true;
		}

		virtual bool Flush() override
		{
			return true;
		}

		virtual size_t Write(const void* pData, FbxUInt64 pSize) override
		{
			if (!m_isStreamOpened)
			{
				return 0;
			}

			m_stream.write(reinterpret_cast<const char*>(pData), pSize);
			return pSize;
		}

		virtual size_t Read(void* pData, FbxUInt64 pSize) const override
		{
			if (!m_isStreamOpened)
			{
				return 0;
			}

			m_stream.read(reinterpret_cast<char*>(pData), pSize);
			return static_cast<size_t>(m_stream.gcount());
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
			switch (pSeekPos)
			{
			case FbxFile::eBegin:
				m_stream.seekg(pOffset, std::ios_base::beg);
				break;
			case FbxFile::eCurrent:
				m_stream.seekg(pOffset, std::ios_base::cur);
				break;
			case FbxFile::eEnd:
				m_stream.seekg(pOffset, std::ios_base::end);
				break;
			}
		}

		virtual FbxInt64 GetPosition() const override
		{
			if (!m_isStreamOpened)
			{
				return 0;
			}

			return m_stream.tellg();
		}
		virtual void SetPosition(FbxInt64 pPosition) override
		{
			if (m_isStreamOpened)
			{
				m_stream.seekg(pPosition, std::ios_base::beg);
			}
		}

		virtual int GetError() const override
		{
			if (!m_isStreamOpened)
			{
				return 0;
			}

			return m_stream.fail();
		}
		virtual void ClearError() override {}
	};
}