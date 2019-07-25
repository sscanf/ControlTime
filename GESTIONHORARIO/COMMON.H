/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.
   Author: Leon Finker  9/2002
**************************************************************************/
#pragma once

#define WINLDAP_NET 1
//#define OPENLDAP_NET 1

#pragma comment(lib, "ws2_32")

#ifdef OPENLDAP_NET
	#pragma comment(lib, "olber32")
	#pragma comment(lib, "oldap_r")
	//#pragma comment(lib, "libsasl") //if sasl is used
	//#pragma comment(lib, "oldap32") //if not using oldap_r
#elif WINLDAP_NET
	#pragma comment(lib, "wldap32")
#endif

#ifdef OPENLDAP_NET
	#define LDAPINT int
	#include <ldap.h>
	struct berelement{}; //this is needed for current MC++ compiler to work around a bug
	struct ldapmsg{};    //for forward declared types with no definition in the header file
	struct ldap{};       //like these are in ldap.h
	#include <winsock.h> //timeval
#elif WINLDAP_NET

   #define LDAPINT unsigned long

	#include <windows.h>
	#include <winldap.h>
	#include <winber.h>
	#if LDAP_UNICODE
		#error Unicode is not supported, code has to be modified to support Unicode
	#endif
#else 
	#error Specify WINLDAP_NET or OPENLDAP_NET
#endif

#using <mscorlib.dll>
#using <System.dll>
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::Serialization;
using namespace System::Collections::Specialized;

#include <vcclr.h>

#include <string.h> //strlen is used, but it's an intrinsic function

namespace ldap_net
{
namespace Exceptions
{
	[Serializable]
    public __gc class LDAPException: public ApplicationException
	{
	public:
        LDAPException(){}
        LDAPException(String* message):ApplicationException(message){}
        LDAPException(String* message, Exception* innerException)
            :ApplicationException(message,innerException){}
	protected:
		LDAPException(SerializationInfo* info, StreamingContext context)
			:ApplicationException(info,context){}
	};
	
	[Serializable]
	public __sealed __gc class LDAPExceptionPartialResult: public LDAPException, public ISerializable
	{
	public:
        LDAPExceptionPartialResult(){}
        LDAPExceptionPartialResult(String* message):LDAPException(message){}
        LDAPExceptionPartialResult(String* message, Exception* innerException)
            :LDAPException(message,innerException){}
		LDAPExceptionPartialResult(int count):partial_count(count){}
		void GetObjectData( SerializationInfo* info, StreamingContext context )
        {
            info->AddValue("partial_count", __box(partial_count), __typeof(Int32));
			LDAPException::GetObjectData(info,context);
        }

	private:
		LDAPExceptionPartialResult(SerializationInfo* info, StreamingContext context)
			:LDAPException(info,context)
		{
			partial_count = info->GetInt32("partial_count");
		}
	public:
		Int32 partial_count;
	};
}

namespace Misc
{
	public __value enum LDAPSearchScope
	{
		LDAPSCOPE_BASE=LDAP_SCOPE_BASE,
		LDAPSCOPE_ONELEVEL=LDAP_SCOPE_ONELEVEL,
		LDAPSCOPE_SUBTREE=LDAP_SCOPE_SUBTREE
	};
	[Flags]
	public __value enum LDAPOperation
	{
		LDAPMOD_ADD=LDAP_MOD_ADD,
		LDAPMOD_DELETE=LDAP_MOD_DELETE,
		LDAPMOD_REPLACE=LDAP_MOD_REPLACE,
		LDAPMOD_BVALUES=LDAP_MOD_BVALUES
	};
	public __value enum LDAPOptions
	{
		LDAPOPT_SIZELIMIT = LDAP_OPT_SIZELIMIT, // int, default: LDAP_NO_LIMIT=0 limit on the number of entries to return from a search
		LDAPOPT_TIMELIMIT = LDAP_OPT_TIMELIMIT, // int, default: LDAP_NO_LIMIT=0 limit on the number of seconds the server spends on a search
		LDAPOPT_DESC	  = LDAP_OPT_DESC,		//SOCKET, underlying socket descriptor corresponding to the default LDAP connection
		LDAPOPT_PROTOCOL_VERSION = LDAP_OPT_PROTOCOL_VERSION //LDAP_VERSION2/LDAP_VERSION3 the version of the default LDAP server
	};
	public __value enum LDAPOptValues
	{
		LDAPOPT_ON   = 1,//LDAP_OPT_ON,
		LDAPOPT_OFF  = 0,//LDAP_OPT_OFF,
		LDAPNO_LIMIT = LDAP_NO_LIMIT,
		LDAPVERSION2 = LDAP_VERSION2,
		LDAPVERSION3= LDAP_VERSION3
	};
	public __value enum AuthMethod
	{
#ifdef OPENLDAP_NET
		LDAPAUTH_SIMPLE = LDAP_AUTH_SIMPLE,
		LDAPAUTH_KRBV4 = LDAP_AUTH_KRBV4

#elif WINLDAP_NET
		LDAPAUTH_SIMPLE = LDAP_AUTH_SIMPLE,
		LDAPAUTH_DPA = LDAP_AUTH_DPA,
		LDAPAUTH_MSN = LDAP_AUTH_MSN,
		LDAPAUTH_NEGOTIATE = LDAP_AUTH_NEGOTIATE, //Use GSSAPI Negotiate package to negotiate security
												   //package of either Kerberos v5 or NTLM (or any other
										           //package the client and server negotiate)
												   //To log in as the current user, set the dn and cred parameters to NULL
												   //To log in as another user, pass a pointer to a SEC_WINNT_AUTH_IDENTITY
												   //structure with the appropriate user name and password
		LDAPAUTH_NTLM = LDAP_AUTH_NTLM			   
#endif
	};
}

}