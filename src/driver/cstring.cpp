#include "cstring.h"
#include "byte_array.h"

#define CHANGE_SYMBOL(_name_, _symb_)																					\
CString& CString::_name_(CString const& strArgument)																	\
{																														\
i64 nArg = IndexOf(_symb_);																							\
																														\
if (nArg < 0)																											\
	return *this;																										\
																														\
	CString strResult = m_strCore.substr(0, nArg);																		\
	strResult += strArgument;																							\
	strResult += m_strCore.substr(nArg + (i64)std::string(_symb_).size());											\
	m_strCore = strResult.m_strCore;																					\
																														\
	return *this;																										\
}																														\
																														\
CString CString::_name_(CString const& strArgument) const																\
{																														\
	CString str(m_strCore);																								\
	i64  nArg = IndexOf(_symb_);																						\
																														\
	if (nArg < 0)																										\
		return str;																										\
																														\
	CString strResult = m_strCore.substr(0, nArg);																		\
	strResult += strArgument;																							\
	strResult += m_strCore.substr(nArg + (i64)std::string(_symb_).size());											\
	str.m_strCore = strResult.m_strCore;																				\
																														\
	return str;																											\
}

CString::CString()
{
}

CString::CString(char chSymbol, i64 nNumber)
	: m_strCore(nNumber, chSymbol)
{}

CString::CString(const char *const strInit)
	: m_strCore(strInit)
{
}

CString::CString(char const& strInit)
{
	m_strCore = strInit;
}

CString::CString(std::string const& strInit)
	: m_strCore(strInit)
{
}

CString::~CString()
{
}

CHANGE_SYMBOL(Arg, "%")
CHANGE_SYMBOL(Arg1, "$_")

CString CString::ToLower() const
{
	std::string string = m_strCore;

	for (unsigned i = 0; i < string.length(); ++i)
	{
		if (string[i] >= 'A' && string[i] <= 'Z')
			string[i] += 'a' - 'A';
	}

	return string;
}

CString CString::ToUpper() const
{
	std::string string = m_strCore;

	for (unsigned i = 0; i < string.length(); ++i)
	{
		if (string[i] >= 'a' && string[i] <= 'z')
			string[i] += 'A' - 'a';
	}

	return string;
}

CString& CString::ClearEmptySpaces()
{
	for (unsigned i = 0; i < m_strCore.size(); ++i)
	{
		if (isspace(m_strCore[i]))
			m_strCore.erase(i);
	}

	return *this;
}

i64 CString::IndexOf(char chTarget, i64 pos) const
{
	return m_strCore.find_first_of(chTarget, pos);
}

i64 CString::IndexOf(CString const & strTarget, i64 pos) const
{
	i64 index = m_strCore.find_first_of(strTarget[0], pos);
	bool b = false;

	while (!b && index != std::string::npos)
	{
		b = true;
		for (int i = 0; i < strTarget.Size() && (i + index < Size()); ++i)
		{
			if (m_strCore[index + i] != strTarget[i])
			{
				b = false;
			}
		}

		if(!b)
			index = m_strCore.find_first_of(strTarget[0], index + 1);
	}

	if (index == std::string::npos)
		return std::string::npos;

	return index;
}

i64 CString::LastIndexOf(char chTarget, i64 pos) const
{
	return m_strCore.find_last_of(chTarget, pos);
}

i64 CString::LastIndexOf(CString const & strTarget, i64 pos) const
{
	i64 index = m_strCore.find_last_of(strTarget[0], pos);
	bool b = false;

	while (!b && index != std::string::npos)
	{
		b = true;
		for (int i = 0; i < strTarget.Size() && (i + index < Size()); ++i)
		{
			if (m_strCore[index + i] != strTarget[i])
			{
				b = false;
			}
		}

		if (!b)
			index = m_strCore.find_first_of(strTarget[0], index + 1);
	}

	if (index == std::string::npos)
		return std::string::npos;

	return index;
}

i64 CString::CountOf(char ch, i64 posBegin, i64 posEnd) const
{
	i64 result = 0;

	for (i64 i = posBegin; i < posEnd && i < (i64)m_strCore.size(); ++i)
	{
		if (m_strCore[i] == ch)
			++result;
	}

	return result;
}

CString CString::Substr(i64 begin, i64 end) const
{
	return m_strCore.substr(begin, end - begin);
}

CStringList CString::Split(char chSymbol, bool bFront) const
{
	CStringList list;
	std::string str = m_strCore;
	i64 nIndex;

	while ((nIndex = str.find_last_of(chSymbol)) != std::string::npos)
	{
		if(bFront)
			list.push_front(str.substr(nIndex + 1));
		else
			list.push_back(str.substr(nIndex + 1));
		str[nIndex] = '\0';
		str.resize(nIndex);
	}

	if (str.size() > 0)
	{
		if (bFront)
			list.push_front(str);
		else
			list.push_back(str.substr(nIndex + 1));
	}

	return list;
}

CString& CString::operator += (CString const& strString)
{
	m_strCore += strString.m_strCore;

	return *this;
}

CString::operator std::string()
{
	return m_strCore;
}

CString::operator std::string() const
{
	return m_strCore;
}

const char* CString::ToCString()
{
	return m_strCore.c_str();
}

const char* CString::ToCString() const
{
	return m_strCore.c_str();
}

std::istream& operator >> (std::istream &stream, CString & string)
{
	return stream >> string.m_strCore;
}

std::ostream& operator << (std::ostream &stream, CString const& string)
{
	return stream << string.m_strCore;
}

CString operator + (CString const& strString1, CString const& strString2)
{
	return strString1.m_strCore + strString2.m_strCore;
}

CString operator + (CString const& strString1, std::string const& strString2)
{
	return strString1.m_strCore + strString2;
}

CString operator + (std::string const& strString1, CString const& strString2)
{
	return strString1 + strString2.m_strCore;
}

CString operator + (char const& strString1, CString const& strString2)
{
	return CString(strString1) + strString2;
}

CString operator + (CString const& strString1, char const& strString2)
{
	return strString1.m_strCore + CString(strString2);
}

CString CStringList::Join(CString const & strJoin) const
{
	if (empty())
		return "";

	CString result = front();
	std::list<CString>::const_iterator cit = cbegin();

	for (++cit; cit != cend(); ++cit)
	{
		result += strJoin + *cit;
	}

	return result;
}