/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
   Author: Leon Finker  9/2002
**************************************************************************/
#include "common.h"
#include "ldap_net.h"
#include "helpers.h"

namespace ldap_net
{
////////////////////////////////////////////////////////////////////////
// LdapAttribute
String* LdapAttribute::get_StringValues()[]
{ 
	CIntPtrStringAnsi ptrAttribname(m_attribname);
	char** vals = ::ldap_get_values(m_ldap, m_start, ptrAttribname);
	ptrAttribname.Free();
	Collections::Specialized::StringCollection* sc = new Collections::Specialized::StringCollection();
	for(int i=0; vals[i] != 0; ++i)
		sc->Add(new String(vals[i]));

	ldap_value_free(vals);

	String* sa[] = new String*[sc->Count];
	sc->CopyTo(sa, 0);
	sc->Clear();
	return sa;
}
/* get_BinaryValues */
Array* LdapAttribute::get_BinaryValues()
{ 
	CIntPtrStringAnsi ptrAttribname(m_attribname);
	berval** vals = ldap_get_values_len(m_ldap, m_start, ptrAttribname);
	ptrAttribname.Free();
	Array* arr[] = new Array*[ldap_count_values_len(vals)];
	for(int i=0; vals[i] != 0; ++i)
	{
		Byte ba[] = new Byte[vals[i]->bv_len];
		Marshal::Copy(IntPtr(vals[i]->bv_val), ba, 0, ba->Length);
		arr[i]=ba;
	}

	ldap_value_free_len(vals);
	return arr;
}
/* MoveNext */
bool LdapAttribute::MoveNext()
{  
	char* attrn;
	if(m_current == 0)
	{
		BerElement* temp;
		attrn = ldap_first_attribute(m_ldap, m_start, &temp);
		m_current = temp;
	}
	else
	{
		attrn = ldap_next_attribute(m_ldap, m_start, m_current);
	}
	if(attrn != 0)
	{
		m_attribname = Marshal::PtrToStringAnsi(attrn);
		ldap_memfree(attrn);
	}
	else
	{
		if(m_current != 0)
			ber_free(m_current, 0);
		
		m_current = 0;
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// LdapEntry

bool LdapEntry::MoveNext()
{
	if(m_current == 0)
		m_current = ldap_first_entry(m_ldap, m_start);
	else
		m_current = ldap_next_entry(m_ldap,m_current);
	if(m_current != 0)
	{
		char* dn = ldap_get_dn(m_ldap, m_current);
		if(dn != 0)
		{
			m_dn = Marshal::PtrToStringAnsi(dn);
			ldap_memfree(dn);
		}
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
// LDAPModify
// WHAT a f MESS this is!!!
//might have a bug, be carefull
LDAPModify::LDAPModify(const char* dn, ListDictionary * attrvals, long op)
{
	Collections::IDictionaryEnumerator* dicenum = attrvals->GetEnumerator();
	IntPtr ptrMods = Marshal::AllocCoTaskMem((attrvals->Count+1) * sizeof(::LDAPMod*)); //+1 for null
	/*1*/ppMods = (LDAPMod**)ptrMods.ToPointer();
	for(int i=0; i< attrvals->Count; ++i)
		/*2*/ppMods[i] = (::LDAPMod*)Marshal::AllocCoTaskMem(sizeof(::LDAPMod)).ToPointer();

	for(int i=0; dicenum->MoveNext(); ++i)
	{
		String* skey=dynamic_cast<String*>(dicenum->Key);
		if(skey == 0)
			throw new InvalidCastException(S"Invalid type for attribute name, String expected");
		
		ppMods[i]->mod_op = op;
		bool binary=false;
		
		String* svalarr[]= dynamic_cast<String*[]>(dicenum->Value);
		Object* barr[]   = dynamic_cast<Object*[]>(dicenum->Value);
		Byte ba[]		 = dynamic_cast<Byte[]>(dicenum->Value);
		String* sval	 = dynamic_cast<String*>(dicenum->Value);

		int nValsCount = 0;
		if(svalarr != 0)
		{
			binary = false;
			nValsCount = svalarr->Count;
		}
		else if(barr != 0) //check has to be after svalarr
		{
			binary = true;
			nValsCount = barr->Count;
		}
		else if(sval != 0)
		{
			binary = false;
			nValsCount = 1;
		}
		else if(ba != 0)
		{
			binary = true;
			nValsCount = 1;
		}
		else
		{
			if(dicenum->Value != 0)
				throw new InvalidCastException(String::Concat("Expected String, String[], Array of Byte[], or Byte[] for values but got ", dicenum->Value->GetType()->ToString()));
		}
		
		ppMods[i]->mod_op |= LDAP_MOD_BVALUES;
		/*3*/ppMods[i]->mod_type=(char*)Marshal::StringToCoTaskMemAnsi(skey).ToPointer();
		
		/*4*/ppMods[i]->mod_vals.modv_bvals = (berval**)Marshal::AllocCoTaskMem((nValsCount+1) * sizeof(berval*)).ToPointer(); //+1 for null;
		for(int j=0; j < nValsCount; ++j)
		{
			/*5*/ppMods[i]->mod_vals.modv_bvals[j]=(berval*)Marshal::AllocCoTaskMem(sizeof(berval)).ToPointer();
			char* pTemp=0;
			int nLen = 0;
			if(!binary)
			{
				if(sval)
					pTemp = (char*)Marshal::StringToCoTaskMemAnsi(sval).ToPointer();
				else
					pTemp = (char*)Marshal::StringToCoTaskMemAnsi(svalarr[j]).ToPointer();
				nLen = strlen(pTemp);
			}
			else
			{
				Byte baTemp[];
				if(ba != 0)
				{
					baTemp = ba;
					nLen = baTemp->Count;
					pTemp = (char*)Marshal::AllocCoTaskMem(nLen).ToPointer();
					Marshal::Copy(baTemp, 0, IntPtr(pTemp), nLen);
				}
				else
				{
					if(dynamic_cast<Byte[]>(barr[j]) == 0)
						throw new InvalidCastException(String::Concat("Expected Byte[] for values but got ", dicenum->Value->GetType()->ToString()));
					baTemp = (Byte[])barr[j];
					nLen = baTemp->Count;
					pTemp = (char*)Marshal::AllocCoTaskMem(nLen).ToPointer();
					Marshal::Copy(baTemp, 0, IntPtr(pTemp), nLen);
				}
			}
			/*6*/ppMods[i]->mod_vals.modv_bvals[j]->bv_val=pTemp;
			ppMods[i]->mod_vals.modv_bvals[j]->bv_len = nLen;
		}
		ppMods[i]->mod_vals.modv_bvals[nValsCount]=0;//must null terminate for ldap
	}
	ppMods[attrvals->Count] = 0; //need to null terminate for ldap
}

/* ~LDAPModify */
LDAPModify::~LDAPModify()
{
	for(int i=0; ppMods[i] != 0; ++i)
	{
		for(int j=0; ppMods[i]->mod_vals.modv_bvals[j] != 0; ++j)
		{
			/*6*/Marshal::FreeCoTaskMem(IntPtr(ppMods[i]->mod_vals.modv_bvals[j]->bv_val));
			/*5*/Marshal::FreeCoTaskMem(IntPtr(ppMods[i]->mod_vals.modv_bvals[j]));
		}
		
		/*4*/Marshal::FreeCoTaskMem(IntPtr(ppMods[i]->mod_vals.modv_bvals));

		/*3*/Marshal::FreeCoTaskMem(IntPtr(ppMods[i]->mod_type));
		/*2*/Marshal::FreeCoTaskMem(IntPtr(ppMods[i]));
	}
	/*1*/Marshal::FreeCoTaskMem(ppMods);
}

/////////////////////////////////////////////////////////////////////////////////
// LdapClient
LdapClient::LdapClient(String* HostName, UInt32 port, bool prot_ver3, bool ssl)
{
	CIntPtrStringAnsi ptrHost(HostName);
	
#ifdef OPENLDAP_NET
	m_ldap = ::ldap_init(ptrHost, port);	
	//LDAP* temp; //this code is needed for connecting with ssl to AD, because ldap_start_tls_s won't work on win2k
	              //but ldap_start_tls_s should work with windows 2003 and AD
	//ldap_initialize(&temp, (char*)ptr.ToPointer()); //for uri based hostname i.e. ldaps://localhost (ssl)
	//m_ldap = ldap;
#else
	m_ldap = ::ldap_sslinit(ptrHost, port, ssl);	
#endif

	ptrHost.Free();

	if(m_ldap == 0)
		GetLDAPErrorThrow(0);
	if(prot_ver3)
	{
		LDAPINT ver = LDAP_VERSION3;
		LDAPINT nRet = ::ldap_set_option(m_ldap, LDAP_OPT_PROTOCOL_VERSION, &ver);
		//MS specific: nRet = ::ldap_set_option(m_ldap, LDAP_OPT_SERVER_CERTIFICATE, &ServerCertCallback);
		if(nRet != LDAP_SUCCESS)
			GetLDAPErrorThrow(nRet);
	}
#ifdef OPENLDAP_NET
	if(ssl)
	{
		//when using winldap this function is only available on winxp+
		LDAPINT nRet = ::ldap_start_tls_s(m_ldap,0,0);
		if(nRet != LDAP_SUCCESS)
			GetLDAPErrorThrow(nRet);
	}
#endif

}

/* get_DefaultSSLPort */
UInt32 LdapClient::get_DefaultSSLPort() 
{ 
#ifdef OPENLDAP_NET
	return LDAPS_PORT;
#else
	return LDAP_SSL_PORT;
#endif
}

/* ldap_search_s */
Int32 LdapClient::ldap_search_ext_s(String* base, Misc::LDAPSearchScope scope, String* filter, String* attrs[], bool attrsonly, int timeoutsecs, int sizelimit, [Out] LdapResult** res)
{
	CIntPtrStringAnsi ptrBase(base);
	CIntPtrStringAnsi ptrFilter(filter);
	LDAPMessage *msg=0;

	char** pAttrs = 0;
	if(attrs->Count > 0)
	{
		pAttrs = (char**)(Marshal::AllocCoTaskMem((attrs->Count+1) * sizeof(char*)).ToPointer());
		for(int i=0; i<attrs->Count; ++i)
		{
			IntPtr ptr = Marshal::StringToCoTaskMemAnsi(attrs[i]);
			pAttrs[i] = (char*)ptr.ToPointer();
		}
		pAttrs[attrs->Count] = 0;
	}

	//LDAPINT nRet = ::ldap_search_s(m_ldap, ptrBase, scope, ptrFilter, pAttrs, attrsonly, &msg);
#ifdef WINLDAP_NET
	l_timeval time={0};
#elif OPENLDAP_NET
	timeval	  time={0};
#endif
	time.tv_sec = timeoutsecs;
	LDAPINT nRet = ::ldap_search_ext_s(m_ldap, ptrBase, scope, ptrFilter, pAttrs, attrsonly, 0, 0, &time, sizelimit, &msg);
	ptrBase.Free();
	ptrFilter.Free();
	if(attrs->Count > 0)
	{
		for(int i=0; i < attrs->Count; ++i)
		{
			char* p = pAttrs[i];
			Marshal::FreeCoTaskMem(IntPtr(p));
			pAttrs[i]=0;
		}
		Marshal::FreeCoTaskMem(pAttrs);
	}
	if(nRet != LDAP_SUCCESS && nRet != LDAP_PARTIAL_RESULTS && nRet != LDAP_SIZELIMIT_EXCEEDED)
	{
		if(msg)
			ldap_msgfree(msg);
		GetLDAPErrorThrow(nRet);
	}
	LDAPINT nCount = ldap_count_entries(m_ldap, msg);
	*res = new LdapResult(m_ldap, msg);
	if(nRet == LDAP_PARTIAL_RESULTS || nRet == LDAP_SIZELIMIT_EXCEEDED)
		throw new Exceptions::LDAPExceptionPartialResult(nCount);
	return nCount;
}

/* ldap_add_s */
void LdapClient::ldap_add_s(String* dn, ListDictionary * attrvals)
{
#ifdef _DEBUG
	Diagnostics::Debug::Assert(attrvals->Count > 0,"Nothing to add.");
#endif
	CIntPtrStringAnsi ptrDN(dn);
	LDAPModify mod(ptrDN, attrvals, LDAP_MOD_ADD);
	LDAPINT nRet = ::ldap_add_s(m_ldap, ptrDN, mod.ppMods);
	ptrDN.Free();
	if(nRet != LDAP_SUCCESS)
		GetLDAPErrorThrow(nRet);
}

/* ldap_compare_s */
bool LdapClient::ldap_compare_s(String* dn, String* attr, String* val)
{
	CIntPtrStringAnsi ptrDN(dn);
	CIntPtrStringAnsi ptrAttr(attr);
	CIntPtrStringAnsi ptrVal(val);
	LDAPINT nRet = ::ldap_compare_s(m_ldap, ptrDN, ptrAttr, ptrVal);
	ptrDN.Free();
	ptrAttr.Free();
	ptrVal.Free();
	if(nRet != LDAP_COMPARE_TRUE && nRet != LDAP_COMPARE_FALSE)
		GetLDAPErrorThrow(nRet);

	return nRet == LDAP_COMPARE_TRUE;
}

/* ldap_delete_s */
void LdapClient::ldap_delete_s(String* dn)
{
	CIntPtrStringAnsi ptrDN(dn);
	LDAPINT nRet = ::ldap_delete_s(m_ldap, ptrDN);
	ptrDN.Free();
	if(nRet != LDAP_SUCCESS)
		GetLDAPErrorThrow(nRet);
}

/* ldap_simple_bind_s */
void LdapClient::ldap_simple_bind_s(String* who, String* passwd)
{
	CIntPtrStringAnsi ptrWho(who);
	CIntPtrStringAnsi ptrPasswd(passwd);
	LDAPINT nRet = ::ldap_simple_bind_s(m_ldap, ptrWho, ptrPasswd);
	ptrWho.Free();
	ptrPasswd.Free();
	if(nRet != LDAP_SUCCESS)
		GetLDAPErrorThrow(nRet);
}

/*ldap_modify_s*/
void LdapClient::ldap_bind_s(String* who, String* passwd, Misc::AuthMethod method)
{
	CIntPtrStringAnsi ptrWho(who);
	CIntPtrStringAnsi ptrPasswd(passwd);
	LDAPINT nRet = ::ldap_bind_s(m_ldap, ptrWho, ptrPasswd, method);
	ptrWho.Free();
	ptrPasswd.Free();
	if(nRet != LDAP_SUCCESS)
		GetLDAPErrorThrow(nRet);	
}

/*ldap_modify_s*/
void LdapClient::ldap_modify_s(String* dn, ListDictionary * attrvals, long op)
{
#ifdef _DEBUG
	Diagnostics::Debug::Assert(attrvals->Count > 0,"Nothing to modify.");
#endif
	CIntPtrStringAnsi ptrDN(dn);
	LDAPModify mod(ptrDN, attrvals, op);
	LDAPINT nRet = ::ldap_modify_s(m_ldap, ptrDN, mod.ppMods);
	ptrDN.Free();
	if(nRet != LDAP_SUCCESS)
		GetLDAPErrorThrow(nRet);
}

/*GetLDAPErrorThrow*/
void LdapClient::GetLDAPErrorThrow(LDAPINT nErr)
{
	if(nErr == 0)
		::ldap_get_option(m_ldap, LDAP_OPT_ERROR_NUMBER, &nErr);
	char* pError = ::ldap_err2string(nErr);
	if(pError == 0)
		pError = "unknown error";
	throw new Exceptions::LDAPException(pError);
}

}