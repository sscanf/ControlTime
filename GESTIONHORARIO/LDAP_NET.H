/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
   Author: Leon Finker  9/2002
**************************************************************************/

// oldap.h

#pragma once

namespace ldap_net
{
	//////////////////////////////////////////////////////////////////////////////////////
	// LdapAttribute
	public __sealed __gc class LdapAttribute: public IDisposable, public Collections::IEnumerator
	{
	protected public:
		LdapAttribute(void* ldap, void* entry):m_start((LDAPMessage*)entry),m_current(0),m_ldap((LDAP*)ldap){}
	public:
		~LdapAttribute() { Dispose(false); }
		void Dispose() { Dispose(true); GC::SuppressFinalize(this); }
		__property Object* get_Current(){ return this; }
		__property String* get_Name() { return m_attribname; }
		__property String* get_StringValues()[];
		__property Array* get_BinaryValues();
		bool MoveNext();
		void Reset() 
		{ 
			if(m_current != 0)
				ber_free(m_current, 0);
			m_current=0;
		}
	private:
		void Dispose(bool disposing)
		{
			//if(!disposing){}
			if(m_current != 0)
				ber_free(m_current, 0);
			m_current=0;
		}
	private:
		LdapAttribute(){}
		LdapAttribute(const LdapAttribute&){}
		LDAPMessage* m_start;
		BerElement*  m_current;
		LDAP*		 m_ldap;
		String*		 m_attribname;
	};

	//////////////////////////////////////////////////////////////////////////////////////
	// LdapEntry
	public __sealed __gc class LdapEntry: public Collections::IEnumerator, public Collections::IEnumerable
	{
	protected public:
		LdapEntry(void* ldap, void *res):m_ldap((LDAP*)ldap),m_start((LDAPMessage*)res), m_current(0){}
	public:
		Collections::IEnumerator* GetEnumerator() {return new LdapAttribute(m_ldap, m_current);}
		__property Object* get_Current(){ return this; }
		__property String* get_DN(){ return m_dn; }
		bool MoveNext();
		void Reset() { m_current=0;}
	private:
		LdapEntry(){}
		LdapEntry(const LdapEntry&){}
		LDAPMessage* m_start;
		LDAPMessage* m_current;
		LDAP*		 m_ldap;
		String*		 m_dn;
	};

	//////////////////////////////////////////////////////////////////////////////////////
	// LdapResult
	public __sealed __gc class LdapResult: public IDisposable, public Collections::IEnumerable
	{
	protected public:
		LdapResult(void* ldap, void *res):m_ldap((LDAP*)ldap),m_res((LDAPMessage*)res){}
	public:
		~LdapResult() { Dispose(false); }
		void Dispose() { Dispose(true); GC::SuppressFinalize(this);}
		Collections::IEnumerator* GetEnumerator() {return new LdapEntry(m_ldap, m_res);}
	private:
		LdapResult(){}
		LdapResult(const LdapResult&){}
		void Dispose(bool disposing)
		{
			//if(!disposing){}
			if(m_res != 0)
				ldap_msgfree(m_res);
			m_res = 0;
		}
		LDAPMessage* m_res;
		LDAP*		 m_ldap;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////
	// LDAPModify
	class LDAPModify
	{
	public:
		::LDAPMod** ppMods;
	public:
		LDAPModify(const char* dn, ListDictionary * attrvals, long op);
		~LDAPModify();
	private:
		LDAPModify();
		LDAPModify(const LDAPModify&);
		LDAPModify& operator=(const LDAPModify&);
	};
	
	//////////////////////////////////////////////////////////////////////////////////////
	// LdapClient
	public __sealed __gc class LdapClient: public IDisposable
	{
	public:
		LdapClient(String* HostName, UInt32 port, bool prot_ver3, bool ssl);
		~LdapClient()      { Dispose(false); }
		void Dispose() { Dispose(true); /*GC::SuppressFinalize(this);*/}

		__property static UInt32 get_DefaultPort() { return LDAP_PORT;}
		__property static UInt32 get_DefaultSSLPort();
		Int32 ldap_search_ext_s(String* base, Misc::LDAPSearchScope scope, String* filter, String* attrs[], bool attrsonly, int timeout_secs, int sizelimit, [Out] LdapResult** res);
		void ldap_simple_bind_s(String* who, String* passwd);
		void ldap_bind_s(String* who, String* passwd, Misc::AuthMethod method);
		void ldap_delete_s(String* dn);
		void ldap_add_s(String* dn, ListDictionary * attrvals);
		bool ldap_compare_s(String* dn, String* attr, String* val);
		void ldap_mod_add(String* dn, ListDictionary * attrvals)
		{
			this->ldap_modify_s(dn,attrvals,LDAP_MOD_ADD);
		}
		void ldap_mod_delete(String* dn, ListDictionary * attrvals)
		{
			this->ldap_modify_s(dn,attrvals,LDAP_MOD_DELETE);
		}
		void ldap_mod_replace(String* dn, ListDictionary * attrvals)
		{
			this->ldap_modify_s(dn,attrvals,LDAP_MOD_REPLACE);
		}
		void ldap_unbind()
		{
			if(m_ldap != 0)
				::ldap_unbind_s(m_ldap);
			m_ldap = 0;
		}
		//ldap_sasl_bind_s
		//ldap_rename_ext_s ldap_rename_s
	private:
		void ldap_modify_s(String* dn, ListDictionary * attrvals, long op);
		void GetLDAPErrorThrow(LDAPINT nErr);
		void Dispose(bool disposing)
		{
			//if(!disposing){}
			this->ldap_unbind();
		}
		LDAP* m_ldap;
	private:
		LdapClient(){}
		LdapClient(const LdapClient&){}
	};
}
