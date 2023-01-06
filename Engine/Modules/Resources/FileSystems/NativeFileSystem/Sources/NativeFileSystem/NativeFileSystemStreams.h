#pragma once

#include <fstream>
#include <cassert>

#include "NativeFileSystemStreams.h"
#include "Core/FileName/FileName.h"
#include "FileSystem/IFileSystem.h"

namespace VT
{
	class NativeFileSystemWriteStream : public IFileSystem::IWriteStream
	{
		using OpenMode = int;

	private:
		std::ofstream m_stream;

	public:
		NativeFileSystemWriteStream(const FileName& name, OpenMode mode)
		{
			m_stream.open(name.c_str(), mode);
		}
		~NativeFileSystemWriteStream()
		{
			m_stream.close();
		}

		virtual size_t write(const void* data, size_t dataSize) override
		{
			assert(m_stream.is_open());

			size_t pos = m_stream.tellp();

			m_stream.write(reinterpret_cast<const char*>(data), dataSize);
			return static_cast<size_t>(m_stream.tellp()) - pos;
		}

		virtual bool isOpen() const override
		{
			return m_stream.is_open();
		}

		virtual void seek(size_t pos, SeekDir seekPos)
		{
			std::ios_base::seekdir dir;

			switch (seekPos)
			{
			case SeekDir::BEGIN:
				dir = std::ios_base::beg;
				break;
			case SeekDir::END:
				dir = std::ios_base::end;
				break;
			case SeekDir::CURRENT:
			default:
				dir = std::ios_base::cur;
				break;
			}

			m_stream.seekp(pos, dir);
		}

		virtual size_t getPos()
		{
			return m_stream.tellp();
		}
		virtual int getError() const
		{
			return m_stream.rdstate();
		}
	};

	class NativeFileSystemReadStream : public IFileSystem::IReadStream
	{
		using OpenMode = int;

	private:
		std::ifstream m_stream;

	public:
		NativeFileSystemReadStream(const FileName& name, OpenMode mode)
		{
			m_stream.open(name.c_str(), mode);
		}
		~NativeFileSystemReadStream()
		{
			m_stream.close();
		}

		virtual size_t read(void* data, size_t dataSize) override
		{
			assert(m_stream.is_open());
			m_stream.read(reinterpret_cast<char*>(data), dataSize);
			return static_cast<size_t>(m_stream.gcount());
		}

		virtual bool isOpen() const override
		{
			return m_stream.is_open();
		}

		virtual void seek(size_t pos, SeekDir seekPos)
		{
			std::ios_base::seekdir dir;

			switch (seekPos)
			{
			case SeekDir::BEGIN:
				dir = std::ios_base::beg;
				break;
			case SeekDir::END:
				dir = std::ios_base::end;
				break;
			case SeekDir::CURRENT:
			default:
				dir = std::ios_base::cur;
				break;
			}

			m_stream.seekg(pos, dir);
		}

		virtual size_t getPos()
		{
			return static_cast<size_t>(m_stream.tellg());
		}
		virtual int getError() const
		{
			return m_stream.rdstate();
		}
	};

	class NativeFileSystemReadWriteStream : public IFileSystem::IReadWriteStream
	{
		using OpenMode = int;

	private:
		std::fstream m_stream;

	public:
		NativeFileSystemReadWriteStream(const FileName& name, OpenMode mode)
		{
			m_stream.open(name.c_str(), mode);
		}
		~NativeFileSystemReadWriteStream()
		{
			m_stream.close();
		}

		virtual size_t read(void* data, size_t dataSize) override
		{
			assert(m_stream.is_open());
			m_stream.read(reinterpret_cast<char*>(data), dataSize);
			return static_cast<size_t>(m_stream.gcount());
		}

		virtual size_t write(const void* data, size_t dataSize) override
		{
			assert(m_stream.is_open());

			size_t pos = m_stream.tellp();

			m_stream.write(reinterpret_cast<const char*>(data), dataSize);
			return static_cast<size_t>(m_stream.tellp()) - pos;
		}

		virtual bool isOpen() const override
		{
			return m_stream.is_open();
		}

		virtual void seek(size_t pos, SeekDir seekPos)
		{
			std::ios_base::seekdir dir = std::ios_base::cur;

			switch (seekPos)
			{
			case SeekDir::BEGIN:
				dir = std::ios_base::beg;
				break;
			case SeekDir::END:
				dir = std::ios_base::end;
				break;
			case SeekDir::CURRENT:
			default:
				dir = std::ios_base::cur;
				break;
			}

			m_stream.seekp(pos, dir);
		}

		virtual size_t getPos()
		{
			return static_cast<size_t>(m_stream.tellp());
		}
		virtual int getError() const
		{
			return m_stream.rdstate();
		}
	};
}
