#include "comport.h"
#include "byte_array.h"

#define CONFIG(__config, __type)																						\
void COMPort::Set##__config(##__type prop)																				\
{																														\
	COMPort * obj = GlobalInstance();																					\
	obj->m_sNewSettings = obj->m_sSettings;																				\
	obj->m_sNewSettings.##__config = prop;																				\
																														\
	if (!SetCommState(obj->m_hCOMport, &obj->m_sNewSettings))															\
	{																													\
		throw E_##__config##_configuration();																			\
	}																													\
																														\
	obj->m_sSettings.##__config = prop;																					\
}

COMPort * COMPort::s_pInstance = nullptr;

void COMPort::CreateInstance(CString const& portName)
{
	s_pInstance = new COMPort();
	s_pInstance->m_strPortName = CString("\\\\.\\") + portName;
	s_pInstance->Open();
	s_pInstance->ConfigureDefault();
	Sleep(2000);
}

void COMPort::SetBaudRate(DWORD prop)
{
	COMPort * obj = GlobalInstance();
	obj->m_sNewSettings = obj->GetConfiguration();
	obj->m_sNewSettings.BaudRate = prop;

	if (!SetCommState(obj->m_hCOMport, &obj->m_sNewSettings))
	{
		throw E_BaudRate_configuration();
	}

	obj->m_sSettings.BaudRate = prop;
}

CONFIG(ByteSize, BYTE)
CONFIG(StopBits, BYTE)
CONFIG(Parity, BYTE)
CONFIG(fDtrControl, BYTE)

DCB COMPort::GetConfiguration()
{
	DCB config;
	GetCommState(GlobalInstance()->m_hCOMport, &config);
	return config;
}

//----------------------------------------------------------------------------------------------------------------------

COMPort::COMPort()
	: m_strPortName("")
	, m_sSettings{0}
	, m_sNewSettings{0}
	, m_bUnsaved(false)
{}

COMPort::~COMPort()
{
	Close();
}

void COMPort::Open()
{
	GlobalInstance()->m_hCOMport = CreateFile(GlobalInstance()->m_strPortName.ToCString(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
}

void COMPort::Close()
{
	CloseHandle(GlobalInstance()->m_hCOMport);
}

void COMPort::Write(CByteArray data)
{
	DWORD written;
	WriteFile(GlobalInstance()->m_hCOMport, data, data.Size(), &written, 0);

	if (written < data.Size())
		throw E_data_loss();
}

void COMPort::Write(void* data, i32 size)
{
	DWORD written;
	WriteFile(GlobalInstance()->m_hCOMport, data, size, &written, 0);

	if (written < size)
		throw E_data_loss();
}

void COMPort::Read(CByteArray &data)
{
	DWORD readen;
	ReadFile(GlobalInstance()->m_hCOMport, data, data.Size(), &readen, 0);

	if (readen == data.Size())
		throw E_data_loss();
}

void COMPort::Read(void* data, i32 size)
{
	DWORD readen;
	ReadFile(GlobalInstance()->m_hCOMport, data, size, &readen, 0);

	if (readen == size)
		throw E_data_loss();
}

void COMPort::ConfigureDefault()
{
	DCB m_sSettings = { 0 };
	m_sSettings.DCBlength = sizeof(m_sSettings);

	GetCommState(m_hCOMport, &m_sSettings);
	m_sSettings.BaudRate = 9600;
	m_sSettings.ByteSize = 8;
	m_sSettings.StopBits = 1;
	m_sSettings.Parity = 0;
	m_sSettings.fDtrControl = DTR_CONTROL_ENABLE;
	SetCommState(m_hCOMport, &m_sSettings);
}
