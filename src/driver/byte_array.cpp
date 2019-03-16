#include "byte_array.h"

CByteArray::CByteArray(ui64 size)
	: m_nSize(size)
	, m_pBuffer(new ui8[Size()]())
{
	if (m_pBuffer == nullptr)
	{
		m_nSize = 0;
		throw E_bad_alloc();
	}
}

CByteArray::CByteArray(CByteArray const& copy)
	: m_nSize(copy.Size())
	, m_pBuffer(new ui8[Size()])
{
	if (m_pBuffer == nullptr)
	{
		m_nSize = 0;
		throw E_bad_alloc();
	}

	for (int i = 0; i < copy.Size(); ++i)
	{
		m_pBuffer[i] = copy[i];
	}
}

ui8& CByteArray::operator[](ui64 index)
{
	if (index > Size())
		throw E_invalid_index();

	return m_pBuffer[index];
}

ui8 const& CByteArray::operator[](ui64 index) const
{
	if (index > Size())
		throw E_invalid_index();

	return m_pBuffer[index];
}

CByteArray CByteArray::FromString(std::string const & string)
{
	CByteArray arr(string.length());
	for (int i = 0; i < string.length(); ++i)
		arr.m_pBuffer[i] = string[i];

	return arr;
}

CByteArray::~CByteArray()
{
	delete[] m_pBuffer;
}
