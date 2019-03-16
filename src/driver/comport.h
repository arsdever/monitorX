#pragma once

#include <windows.h>
#include "cstring.h"

class CByteArray;

class COMPort
{
	COMPort();
	~COMPort();
	void SetfDtrControl(BYTE dtr);

public:
	void Open();
	void Close();
	void Write(CByteArray data);
	void Write(void* data, i32 size);
	void Read(CByteArray &data);
	void Read(void* data, i32 size);
	void ConfigureDefault();
	DCB GetConfiguration();

	static void SetBaudRate(DWORD br);
	static void SetByteSize(BYTE bs);
	static void SetStopBits(BYTE sb);
	static void SetParity(BYTE par);
	inline void SetDTRControl(BYTE dtr) { SetfDtrControl(dtr); }

	static void CreateInstance(CString const& portName);
	inline static COMPort * GlobalInstance() { return s_pInstance; }

private:
	static COMPort * s_pInstance;
	HANDLE m_hCOMport;
	CString m_strPortName;
	DCB m_sSettings;
	DCB m_sNewSettings;
	bool m_bUnsaved;

public:
	// Exceptions
	class E_COMPort {};
	class E_configuration : public E_COMPort {};
	class E_writing : public E_COMPort {};
	class E_reading : public E_COMPort {};
	class E_BaudRate_configuration : public E_configuration {};
	class E_ByteSize_configuration : public E_configuration {};
	class E_StopBits_configuration : public E_configuration {};
	class E_Parity_configuration : public E_configuration {};
	class E_fDtrControl_configuration : public E_configuration {};
	class E_data_loss : public E_writing, public E_reading {};
};

