/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
   Author: Leon Finker  9/2002
**************************************************************************/
#pragma once

class CIntPtrStringAnsi
{
public:
	CIntPtrStringAnsi(const gcroot<String*> str)
	{
		m_p = (char*)Marshal::StringToCoTaskMemAnsi(str).ToPointer();
	}
	~CIntPtrStringAnsi() { Free(); }
	void Free()
	{ 
		if(m_p)
			Marshal::FreeCoTaskMem(IntPtr(m_p)); 
		m_p = 0;
	}
#ifdef WINLDAP_NET
	#if LDAP_UNICODE
		#error Unicode is not supported, code has to be modified to support Unicode
	#endif
	operator const PCHAR() { return m_p; }
#elif OPENLDAP_NET
	operator const char*() { return m_p; }
#endif
private:
	char* m_p;
private:
	CIntPtrStringAnsi(const CIntPtrStringAnsi&);
	CIntPtrStringAnsi& operator=(const CIntPtrStringAnsi&);
};