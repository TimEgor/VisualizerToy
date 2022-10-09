#pragma once

#include <cstdint>
#include <cassert>

#include "Core/String/Format.h"

namespace VT
{
	struct GUID final
	{
		struct GUID_Data final
		{
			uint32_t m_data1;	// 0..31
			uint16_t m_data2;	// 32..47
			uint16_t m_data3;	// 48..63
			uint8_t m_data4[8];	// 64..127
		};

		union
		{
			GUID_Data m_data;
			uint8_t m_data8[16];
			struct
			{
				uint64_t m_highPartData;
				uint64_t m_lowPartData;
			};
		};

		GUID() : m_highPartData(0), m_lowPartData(0) {}
		GUID(const GUID_Data& data) : m_data(data) {}
		GUID(uint64_t highPart, uint64_t lowPart) : m_highPartData(highPart), m_lowPartData(lowPart) {}
		GUID(uint32_t d1, uint16_t d2, uint16_t d3, uint8_t d4[8])
			: GUID(d1, d2, d3, d4[0], d4[1], d4[2], d4[3], d4[4], d4[5], d4[6], d4[7])
		{}
		GUID(uint32_t d1, uint16_t d2, uint16_t d3,
			uint8_t d4_1, uint8_t d4_2, uint8_t d4_3, uint8_t d4_4,
			uint8_t d4_5, uint8_t d4_6, uint8_t d4_7, uint8_t d4_8)
			: GUID(
				(static_cast<uint64_t>(d3) << 48) | (static_cast<uint64_t>(d2) << 32) | (static_cast<uint64_t>(d1)),
				(static_cast<uint64_t>(d4_8) << 56) | (static_cast<uint64_t>(d4_7) << 48) | (static_cast<uint64_t>(d4_6) << 40) | (static_cast<uint64_t>(d4_5) << 32) |
				(static_cast<uint64_t>(d4_4) << 24) | (static_cast<uint64_t>(d4_3) << 16) | (static_cast<uint64_t>(d4_2) << 8) | (static_cast<uint64_t>(d4_5))
			)
		{}

		bool operator==(const GUID& id) const { return m_highPartData == id.m_highPartData && m_lowPartData == id.m_lowPartData; }
		bool operator!=(const GUID& id) const { return m_highPartData != id.m_highPartData || m_lowPartData != id.m_lowPartData; }
		bool operator>(const GUID& id) const { return m_highPartData > id.m_highPartData || ((m_highPartData == id.m_highPartData) && m_lowPartData > id.m_lowPartData); }
		bool operator<(const GUID& id) const { return m_highPartData < id.m_highPartData || ((m_highPartData == id.m_highPartData) && m_lowPartData < id.m_lowPartData); }
		bool operator>=(const GUID& id) const { return m_highPartData > id.m_highPartData || ((m_highPartData == id.m_highPartData) && m_lowPartData >= id.m_lowPartData); }
		bool operator<=(const GUID& id) const { return m_highPartData < id.m_highPartData || ((m_highPartData == id.m_highPartData) && m_lowPartData <= id.m_lowPartData); }

		bool isNull() const { m_highPartData == 0 && m_lowPartData == 0; }

		struct StringConverter final
		{
			//
			static uint8_t convertHexCharToUInt8(char ch)
			{
				return ch >= '0' && ch <= '9' ? ch - '0' :
					ch >= 'a' && ch <= 'f' ? ch - 'a' + 10 :
					ch >= 'A' && ch <= 'F' ? ch - 'A' + 10 : 0;
			}

			static uint16_t convertHexCharToUInt16(char ch)
			{
				return convertHexCharToUInt8(ch);
			}

			static uint32_t convertHexCharToUInt32(char ch)
			{
				return convertHexCharToUInt8(ch);
			}

			//
			static uint8_t convertHexStrToUInt8(const char* str)
			{
				return (convertHexCharToUInt8(str[0]) << 4) + convertHexCharToUInt8(str[1]);
			}

			static uint16_t convertHexStrToUInt16(const char* str)
			{
				return (convertHexCharToUInt16(str[0]) << 12) + (convertHexCharToUInt16(str[1]) << 8) +
					(convertHexCharToUInt16(str[2]) << 4) + convertHexCharToUInt16(str[3]);
			}

			static uint32_t convertHexStrToUInt32(const char* str)
			{
				return (convertHexCharToUInt32(str[0]) << 28) + (convertHexCharToUInt32(str[1]) << 24) +
					(convertHexCharToUInt32(str[2]) << 20) + (convertHexCharToUInt32(str[3]) << 16) +
					(convertHexCharToUInt32(str[4]) << 12) + (convertHexCharToUInt32(str[5]) << 8) +
					(convertHexCharToUInt32(str[6]) << 4) + convertHexCharToUInt32(str[7]);
			}

			// convert string (XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX) to GUID
			static GUID convertFromString(const char* str)
			{
				return GUID(
					convertHexStrToUInt32(str),

					convertHexStrToUInt16(str + 9),

					convertHexStrToUInt16(str + 15),

					convertHexStrToUInt8(str + 19),
					convertHexStrToUInt8(str + 21),

					convertHexStrToUInt8(str + 24),
					convertHexStrToUInt8(str + 26),
					convertHexStrToUInt8(str + 28),
					convertHexStrToUInt8(str + 30),
					convertHexStrToUInt8(str + 32),
					convertHexStrToUInt8(str + 34)
				);
			}

			static GUID convertFromFormatString(const char* str)
			{
				GUID guid;

				if (str)
				{
					const size_t strLen = strlen(str);
					if (str[0] == '{' && (strLen >= sizeof("{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}") - 1))
					{
						guid = convertFromString(str + 1);
					}
					else if (str[0] != '{' && (strLen >= sizeof("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX") - 1))
					{
						guid = convertFromString(str);
					}
					else
					{
						assert(false && "GUID string value is invalid.\n");
					}
				}

				return guid;
			}

			static std::string convertToString(const GUID& guid)
			{
				return stringFormat("%.8x-%.4-%.4-%.2%.2-%.2%.2%.2%.2%.2%.2",
					guid.m_data.m_data1,

					guid.m_data.m_data2,

					guid.m_data.m_data3,

					guid.m_data.m_data4[0],
					guid.m_data.m_data4[1],

					guid.m_data.m_data4[2],
					guid.m_data.m_data4[3],
					guid.m_data.m_data4[4],
					guid.m_data.m_data4[5],
					guid.m_data.m_data4[6],
					guid.m_data.m_data4[7]
				);
			}
		};
	};
}