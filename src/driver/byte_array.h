#pragma once

#include "typedef.h"

class CString;

class CByteArray
{
public:
	CByteArray(ui64 size);
	CByteArray(CByteArray const& copy);
	~CByteArray();

	inline ui64 Size() const { return m_nSize; }
	inline ui8& operator[](ui64 index);
	inline ui8 const& operator[](ui64 index) const;
	
	inline operator void*() { return m_pBuffer; }

	static CByteArray FromString(std::string const& string);

private:
	ui64 m_nSize;
	ui8* m_pBuffer;
	
public:
	class E_ByteArray {};
	class E_bad_alloc : public E_ByteArray {};
	class E_invalid_index : public E_ByteArray {};
};

