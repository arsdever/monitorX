#pragma once

#include "typedef.h"
#include <string>
#include <list>

class CStringList;
class CByteArray;

// String wrapper
class CString
{
public:
	CString();
	CString(char chSymbol, i64 nNumber);
	CString(const char *const strInit);
	CString(char const& strInit);
	CString(std::string const& strInit);
	CString(CString const& strInit) = default;
	CString(int strInit) { InitFromNum(strInit); }
	CString(long strInit) { InitFromNum(strInit); }
	CString(short strInit) { InitFromNum(strInit); }
	CString(i64 strInit) { InitFromNum(strInit); }
	CString(unsigned int strInit) { InitFromNum(strInit); }
	CString(unsigned long strInit) { InitFromNum(strInit); }
	CString(unsigned short strInit) { InitFromNum(strInit); }
	CString(ui64 strInit) { InitFromNum(strInit); }
	CString(float strInit) { InitFromNum(strInit); }
	CString(double strInit) { InitFromNum(strInit); }
	CString(long double strInit) { InitFromNum(strInit); }
	// Destructor
	virtual ~CString();

	// Swap '%' symbol with argument
	CString& Arg(CString const& strArgument);
	// Swap '%' symbol with argument
	CString Arg(CString const& strArgument) const;
	// Swap '$' symbol with argument
	CString& Arg1(CString const& strArgument);
	// Swap '$' symbol with argument
	CString Arg1(CString const& strArgument) const;

	// Return equivalent string transfered to lowcase
	CString ToLower() const;
	// Return equivalent string transferet to uppercase
	CString ToUpper() const;
	// Get size of string
	inline i64 Size() const { return m_strCore.size(); }
	inline void Clear() { m_strCore.clear(); }
	// Return equivalent std string
	CString& ClearEmptySpaces();
	// Convert to std::string
	inline std::string ToStdString() const { return m_strCore; }
	// Convert to std::string
	inline std::string & Core() { return m_strCore; }
	// Find first match
	i64 IndexOf(char chTarget, i64 pos = 0) const;
	// Find first match
	i64 IndexOf(CString const & strTarget, i64 pos = 0) const;
	// Find last match
	i64 LastIndexOf(CString const & strTarget, i64 pos = 0) const;
	// Find last match
	i64 LastIndexOf(char chTarget, i64 pos = 0) const;
	// Count the number of copies of char in range
	i64 CountOf(char ch, i64 posBegin, i64 posEnd) const;
	// Substring function
	CString Substr(i64 begin, i64 end) const;
	// Substring function
	inline CString Substring(i64 begin, i64 end) const { return Substr(begin, end); }

	// Split by symbol
	CStringList Split(char chSymbol = ' ', bool bFront = true) const;
	// Get extention of file
	inline CString GetExtention() const;
	// Convert to string
	inline int ToNumber() const { return std::stoi(m_strCore); }

public:
	inline void push_back(char chSymbol);

	// Operators

	CString& operator += (CString const& strString);

	friend std::istream& operator >> (std::istream &stream, CString & string);
	friend std::ostream& operator << (std::ostream &stream, CString const& string);

	friend CString operator + (CString const& strString1, CString const& strString2);
	friend CString operator + (std::string const& strString1, CString const& strString2);
	friend CString operator + (CString const& strString1, std::string const& strString2);
	friend CString operator + (char const& strString1, CString const& strString2);
	friend CString operator + (CString const& strString1, char const& strString2);

	inline CString& operator = (CString const& strString2) = default;
	inline CString& operator = (std::string const& strString2);
	inline CString& operator = (char const& strString2);

	inline bool operator == (CString const& strString) const { return m_strCore == strString.m_strCore; }
	inline bool operator == (std::string const& strString) const { return m_strCore == strString; }
	inline bool operator == (const char *const strString) const { return m_strCore == std::string(strString); }
	inline bool operator != (CString const& strString) const { return m_strCore != strString.m_strCore; }
	inline bool operator != (std::string const& strString) const { return m_strCore != strString; }
	inline bool operator != (const char *const strString) const { return m_strCore != std::string(strString); }
	inline bool operator < (CString const& strString) const { return m_strCore < strString.m_strCore; }
	inline bool operator < (std::string const& strString) const { return m_strCore < strString; }
	inline bool operator < (const char *const strString) const { return m_strCore < std::string(strString); }
	inline bool operator > (CString const& strString) const { return m_strCore > strString.m_strCore; }
	inline bool operator > (std::string const& strString) const { return m_strCore > strString; }
	inline bool operator > (const char *const strString) const { return m_strCore > std::string(strString); }

	inline char operator [] (unsigned nIndex) const;

	// std::string cast
	operator std::string();
	operator std::string() const;

	const char* ToCString();
	const char* ToCString() const;

private:
	template<typename T>
	void InitFromNum(T const & number);

private:
	std::string m_strCore;

};

class CStringList : public std::list<CString>
{
public:
	CString Join(CString const & strJoin) const;
};

template<typename T>
inline void CString::InitFromNum(T const & number)
{
	m_strCore = std::to_string(number);
}

#include "cstring_inline.h"