
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <GalacticInterfaces.h>
#include <GalacticStrongPtr.h>

#include <curl.h>

// The modules internal name (Also used for the namespace name)
MODULE_MAIN("curl");

SDK::Class g_CurlClass;
SDK::Class g_CurlMimeClass;
SDK::Class g_CurlMimePartClass;

class CCurl
{
	public:
		inline CCurl(CURL* pCurl) : m_pCurl() {
			m_pCurl = curl_easy_init();
		}
		
		inline ~CCurl() {
			Free();
		}

		CURL* m_pCurl;

		void Free();
};

void CCurl::Free() {
	if(m_pCurl != nullptr) {
		curl_easy_cleanup(m_pCurl);
		m_pCurl = nullptr;
	}
}

class CCurlMime {
	public:
		inline CCurlMime(curl_mime* pCurlMime) : m_pCurlMime(CURL* pCurl) {
			m_pCurlMime = curl_mime_init(pCurl);
		}
		
		inline ~CCurlMime() {
			Free();
		}

		curl_mime* m_pCurlMime;
		
		void Free();
		void AddPart();
};

void CCurlMime::Free() {
	if(m_pCurlMime != nullptr) {
		curl_mime_free(m_pCurlMime);
		m_pCurlMime = nullptr;
	}
}

void CCurlMime::AddPart(CCurlMimePart* pCurlMimePart) {
	if(m_pCurlMime != nullptr) {
		curl_mime_addpart(pCurlMimePart);
	}
}

class CCurlMimePart {
	public:
		inline CCurlMimePart(curl_mime* CCurlMimePart) : m_pCurlMimePart() {
			
		}
		
		inline ~CCurlMimePart() {
			
		}

		curl_mimepart *m_pCurlMimePart;
		
		void Free();
		void Data();
};

void CCurlMimePart::Data(const char* szData, size_t iDataSize) {
	if(m_pCurlMimePart != nullptr) {
		curl_mime_data(m_pCurlMimePart, szData, iDataSize);
	}
}

void CCurlMimePart::Name(const char* szName) {
	if(m_pCurlMimePart != nullptr) {
		curl_mime_name(m_pCurlMimePart, szName);
	}
}

void ModuleRegister()
{
	g_CurlClass = SDK::Class("Curl");
	g_CurlMimeClass = SDK::Class("CurlMime");
	g_CurlMimePartClass = SDK::Class("CurlMimePart");
	
	
	// Unfinished!
	// This will eventually be a complete CURL wrapper.
	SDK::RegisterFunction("init", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);
		
		pCurl = curl_easy_init();
		
		SDK::ClassValue<CCurl, g_CurlClass> Value(new CCurl(pCurl));	

		return true;

		SDK_ENDTRY;
	});
	
	g_CurlClass.RegisterFunction("mime", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurl, g_CurlClass>();
		CURL* pCurl = pThis->m_pCurl;

		pCurlMime = curl_mime_init(pCurl);
		SDK::ClassValue<CCurlMime, g_CurlMimeClass> Value(new CCurlMime(pCurlMime));	
		return true;

		SDK_ENDTRY;
	});	
	
	g_CurlMimeClass.RegisterFunction("part", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurlMime, g_CurlMimeClass>();
		curl_mime* pCurlMime = pThis->m_pCurlMime;

		pCurlMimePart = curl_mime_addpart(pCurlMime);
		SDK::ClassValue<CCurlMimePart, g_CurlMimePartClass> Value(new CCurlMimePart(pCurlMimePart));	
		return true;

		SDK_ENDTRY;
	});	
	
	g_CurlMimePartClass.RegisterFunction("data", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurlMime, g_CurlMimeClass>();
		curl_mimepart* pCurlMimePart = pThis->m_pCurlMimePart;
		
		const char* szData = State.CheckString(0);
		
		size_t iDataSize;
		State.CheckNumber(1, iDataSize);			

		curl_mime_data(pCurlMimePart, szData, iDataSize);
		return true;

		SDK_ENDTRY;
	});

	g_CurlMimePartClass.RegisterFunction("name", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurlMime, g_CurlMimeClass>();
		curl_mimepart* pCurlMimePart = pThis->m_pCurlMimePart;
		
		const char* szName = State.CheckString(0);

		curl_mime_name(pCurlMimePart, szName);
		return true;

		SDK_ENDTRY;
	});			

	g_CurlClass.RegisterFunction("setOpt", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);
		
		int iValueIndex = 1;

		auto pThis = State.CheckThis<CCurl, g_CurlClass>();
		CURL* pCurl = pThis->pCurl;
		
		int iOptID;
		State.CheckNumber(0, iOptID);		
		
		switch(iOptID) {
			case CURLOPT_ABSTRACT_UNIX_SOCKET:
			case CURLOPT_ACCEPT_ENCODING:
			case CURLOPT_ALTSVC:
			case CURLOPT_CAINFO:
			case CURLOPT_CAPATH:
			case CURLOPT_COOKIE:
			case CURLOPT_COOKIEFILE:
			case CURLOPT_COOKIEJAR:
			case CURLOPT_COOKIELIST:
			case CURLOPT_COPYPOSTFIELDS:
			case CURLOPT_CRLFILE:
			case CURLOPT_CUSTOMREQUEST:
			case CURLOPT_DEFAULT_PROTOCOL:
			case CURLOPT_DNS_INTERFACE:
			case CURLOPT_DNS_LOCAL_IP4:
			case CURLOPT_DNS_LOCAL_IP6:
			case CURLOPT_DNS_SERVERS:
			case CURLOPT_DOH_URL:
			case CURLOPT_EGDSOCKET:
			case CURLOPT_ERRORBUFFER:
			case CURLOPT_FTPPORT:
			case CURLOPT_FTP_ACCOUNT:
			case CURLOPT_FTP_ALTERNATIVE_TO_USER:
			case CURLOPT_INTERFACE:
			case CURLOPT_ISSUERCERT:
			case CURLOPT_KEYPASSWD:
			case CURLOPT_KRBLEVEL:
			case CURLOPT_LOGIN_OPTIONS:
			case CURLOPT_MAIL_AUTH:
			case CURLOPT_MAIL_FROM:
			case CURLOPT_NETRC_FILE:
			case CURLOPT_NOPROXY:
			case CURLOPT_PASSWORD:
			case CURLOPT_PINNEDPUBLICKEY:
			case CURLOPT_POSTFIELDS:
			case CURLOPT_PRE_PROXY:
			case CURLOPT_PROXY:
			case CURLOPT_PROXYUSERNAME:
			case CURLOPT_PROXYUSERPWD:
			case CURLOPT_PROXY_CAINFO:
			case CURLOPT_PROXY_CAPATH:
			case CURLOPT_PROXY_CRLFILE:
			case CURLOPT_PROXY_KEYPASSWD:
			case CURLOPT_PROXY_PINNEDPUBLICKEY:
			case CURLOPT_PROXY_SERVICE_NAME:
			case CURLOPT_PROXY_SSLCERT:
			case CURLOPT_PROXY_SSLCERTTYPE:
			case CURLOPT_PROXY_SSLKEY:
			case CURLOPT_PROXY_SSLKEYTYPE:
			case CURLOPT_PROXY_SSL_CIPHER_LIST:
			case CURLOPT_PROXY_TLS13_CIPHERS:
			case CURLOPT_PROXY_TLSAUTH_PASSWORD:
			case CURLOPT_PROXY_TLSAUTH_TYPE:
			case CURLOPT_PROXY_TLSAUTH_USERNAME:
			case CURLOPT_RANDOM_FILE:
			case CURLOPT_RANGE:
			case CURLOPT_REFERER:
			case CURLOPT_REQUEST_TARGET:
			case CURLOPT_RTSP_SESSION_ID:
			case CURLOPT_RTSP_STREAM_URI:
			case CURLOPT_RTSP_TRANSPORT:
			case CURLOPT_SASL_AUTHZID:
			case CURLOPT_SERVICE_NAME:
			case CURLOPT_SOCKS5_GSSAPI_SERVICE:
			case CURLOPT_SSH_HOST_PUBLIC_KEY_MD5:
			case CURLOPT_SSH_KNOWNHOSTS:
			case CURLOPT_SSH_PRIVATE_KEYFILE:
			case CURLOPT_SSH_PUBLIC_KEYFILE:
			case CURLOPT_SSLCERT:
			case CURLOPT_SSLCERTTYPE:
			case CURLOPT_SSLENGINE:
			case CURLOPT_SSLKEY:
			case CURLOPT_SSLKEYTYPE:
			case CURLOPT_SSL_CIPHER_LIST:
			case CURLOPT_TLS13_CIPHERS:
			case CURLOPT_TLSAUTH_PASSWORD:
			case CURLOPT_TLSAUTH_TYPE:
			case CURLOPT_TLSAUTH_USERNAME:
			case CURLOPT_UNIX_SOCKET_PATH:
			case CURLOPT_URL:
			case CURLOPT_USERAGENT:
			case CURLOPT_USERNAME:
			case CURLOPT_USERPWD:
			case CURLOPT_XOAUTH2_BEARER:
				const char* pValue = State.CheckString(iValueIndex);
				break;
				
			case CURLOPT_STREAM_DEPENDS:
			case CURLOPT_STREAM_DEPENDS_E:
				CURL* pValue = State.CheckClass(iValueIndex, g_CurlClass);
				break;
				
			case CURLOPT_SHARE:
				// Curl Shares (CURLSH)
				// CURLSH* pValue;
				void pValue = NULL;
				break;
				
			case CURLOPT_TIMEVALUE_LARGE:
			case CURLOPT_INFILESIZE_LARGE:
			case CURLOPT_MAXFILESIZE_LARGE:
			case CURLOPT_MAX_RECV_SPEED_LARGE:
			case CURLOPT_MAX_SEND_SPEED_LARGE:
			case CURLOPT_POSTFIELDSIZE_LARGE:
			case CURLOPT_RESUME_FROM_LARGE:
				int pValue;
				State.CheckNumber(iValueIndex, pValue);
				break;
				
			case CURLOPT_STDERR:
				// File
				// Maybe use CheckStream for this?
				void pValue = NULL;
				break;
				
			case CURLOPT_CHUNK_BGN_FUNCTION:
			case CURLOPT_CHUNK_END_FUNCTION:
			case CURLOPT_CLOSESOCKETFUNCTION:
			case CURLOPT_CONV_FROM_NETWORK_FUNCTION:
			case CURLOPT_CONV_FROM_UTF8_FUNCTION:
			case CURLOPT_CONV_TO_NETWORK_FUNCTION:
			case CURLOPT_DEBUGFUNCTION:
			case CURLOPT_FNMATCH_FUNCTION:
			case CURLOPT_HEADERFUNCTION:
			case CURLOPT_INTERLEAVEFUNCTION:
			case CURLOPT_IOCTLFUNCTION:
			case CURLOPT_OPENSOCKETFUNCTION:
			case CURLOPT_READFUNCTION:
			case CURLOPT_RESOLVER_START_FUNCTION:
			case CURLOPT_SEEKFUNCTION:
			case CURLOPT_SOCKOPTFUNCTION:
			case CURLOPT_SSH_KEYFUNCTION:
			case CURLOPT_TRAILERFUNCTION:
			case CURLOPT_WRITEFUNCTION:
			case CURLOPT_XFERINFOFUNCTION:
			case CURLOPT_PROGRESSFUNCTION:
			case CURLOPT_SSL_CTX_FUNCTION:
				// Functions
				auto pValue;
				State.CheckFunction(iValueIndex, pValue);
				break;
				
			case CURLOPT_PORT:
			case CURLOPT_CONNECTTIMEOUT:
			case CURLOPT_WILDCARDMATCH:
			case CURLOPT_ACCEPTTIMEOUT_MS:
			case CURLOPT_ADDRESS_SCOPE:
			case CURLOPT_ALTSVC_CTRL:
			case CURLOPT_APPEND:
			case CURLOPT_AUTOREFERER:
			case CURLOPT_BUFFERSIZE:
			case CURLOPT_CERTINFO:
			case CURLOPT_CONNECTTIMEOUT_MS:
			case CURLOPT_CONNECT_ONLY:
			case CURLOPT_CONNECT_TO:
			case CURLOPT_COOKIESESSION:
			case CURLOPT_CRLF:
			case CURLOPT_DIRLISTONLY:
			case CURLOPT_DISALLOW_USERNAME_IN_URL:
			case CURLOPT_DNS_CACHE_TIMEOUT:
			case CURLOPT_DNS_SHUFFLE_ADDRESSES:
			case CURLOPT_DNS_USE_GLOBAL_CACHE:
			case CURLOPT_EXPECT_100_TIMEOUT_MS:
			case CURLOPT_FAILONERROR:
			case CURLOPT_FILETIME:
			case CURLOPT_FOLLOWLOCATION:
			case CURLOPT_FORBID_REUSE:
			case CURLOPT_FRESH_CONNECT:
			case CURLOPT_FTPSSLAUTH:
			case CURLOPT_FTP_CREATE_MISSING_DIRS:
			case CURLOPT_FTP_FILEMETHOD:
			case CURLOPT_FTP_RESPONSE_TIMEOUT:
			case CURLOPT_FTP_SKIP_PASV_IP:
			case CURLOPT_FTP_SSL_CCC:
			case CURLOPT_FTP_USE_EPRT:
			case CURLOPT_FTP_USE_EPSV:
			case CURLOPT_FTP_USE_PRET:
			case CURLOPT_GSSAPI_DELEGATION:
			case CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS:
			case CURLOPT_HAPROXYPROTOCOL:
			case CURLOPT_HEADER:
			case CURLOPT_HTTP09_ALLOWED:
			case CURLOPT_HTTPAUTH:
			case CURLOPT_HTTPGET:
			case CURLOPT_HTTPPROXYTUNNEL:
			case CURLOPT_HTTP_CONTENT_DECODING:
			case CURLOPT_HTTP_TRANSFER_DECODING:
			case CURLOPT_HTTP_VERSION:
			case CURLOPT_IGNORE_CONTENT_LENGTH:
			case CURLOPT_INFILESIZE:
			case CURLOPT_IPRESOLVE:
			case CURLOPT_KEEP_SENDING_ON_ERROR:
			case CURLOPT_LOCALPORT:
			case CURLOPT_LOCALPORTRANGE:
			case CURLOPT_LOW_SPEED_LIMIT:
			case CURLOPT_LOW_SPEED_TIME:
			case CURLOPT_MAXAGE_CONN:
			case CURLOPT_MAXCONNECTS:
			case CURLOPT_MAXFILESIZE:
			case CURLOPT_MAXREDIRS:
			case CURLOPT_NETRC:
			case CURLOPT_NEW_DIRECTORY_PERMS:
			case CURLOPT_NEW_FILE_PERMS:
			case CURLOPT_NOBODY:
			case CURLOPT_NOPROGRESS:
			case CURLOPT_NOSIGNAL:
			case CURLOPT_PATH_AS_IS:
			case CURLOPT_PIPEWAIT:
			case CURLOPT_POST:
			case CURLOPT_POSTFIELDSIZE:
			case CURLOPT_POSTREDIR:
			case CURLOPT_PROTOCOLS:
			case CURLOPT_PROXYAUTH:
			case CURLOPT_PROXYPASSWORD:
			case CURLOPT_PROXYPORT:
			case CURLOPT_PROXYTYPE:
			case CURLOPT_PROXY_SSLVERSION:
			case CURLOPT_PROXY_SSL_OPTIONS:
			case CURLOPT_PROXY_SSL_VERIFYHOST:
			case CURLOPT_PROXY_SSL_VERIFYPEER:
			case CURLOPT_PROXY_TRANSFER_MODE:
			case CURLOPT_PUT:
			case CURLOPT_REDIR_PROTOCOLS:
			case CURLOPT_RESUME_FROM:
			case CURLOPT_RTSP_CLIENT_CSEQ:
			case CURLOPT_RTSP_REQUEST:
			case CURLOPT_RTSP_SERVER_CSEQ:
			case CURLOPT_SASL_IR:
			case CURLOPT_SOCKS5_AUTH:
			case CURLOPT_SOCKS5_GSSAPI_NEC:
			case CURLOPT_SSH_AUTH_TYPES:
			case CURLOPT_SSH_COMPRESSION:
			case CURLOPT_SSLENGINE_DEFAULT:
			case CURLOPT_SSLVERSION:
			case CURLOPT_SSL_ENABLE_ALPN:
			case CURLOPT_SSL_ENABLE_NPN:
			case CURLOPT_SSL_FALSESTART:
			case CURLOPT_SSL_OPTIONS:
			case CURLOPT_SSL_SESSIONID_CACHE:
			case CURLOPT_SSL_VERIFYHOST:
			case CURLOPT_SSL_VERIFYPEER:
			case CURLOPT_SSL_VERIFYSTATUS:
			case CURLOPT_STREAM_WEIGHT:
			case CURLOPT_SUPPRESS_CONNECT_HEADERS:
			case CURLOPT_TCP_FASTOPEN:
			case CURLOPT_TCP_KEEPALIVE:
			case CURLOPT_TCP_KEEPIDLE:
			case CURLOPT_TCP_KEEPINTVL:
			case CURLOPT_TCP_NODELAY:
			case CURLOPT_TFTP_BLKSIZE:
			case CURLOPT_TFTP_NO_OPTIONS:
			case CURLOPT_TIMECONDITION:
			case CURLOPT_TIMEOUT:
			case CURLOPT_TIMEOUT_MS:
			case CURLOPT_TIMEVALUE:
			case CURLOPT_TRANSFERTEXT:
			case CURLOPT_TRANSFER_ENCODING:
			case CURLOPT_UNRESTRICTED_AUTH:
			case CURLOPT_UPKEEP_INTERVAL_MS:
			case CURLOPT_UPLOAD:
			case CURLOPT_UPLOAD_BUFFERSIZE:
			case CURLOPT_USE_SSL:
			case CURLOPT_VERBOSE:
				int pValue;
				State.CheckNumber(iValueIndex, pValue);
				break;
				
			case CURLOPT_MIMEPOST:
				// Mime (via curl_mime_init)
				auto pValue = State.CheckClass(iValueIndex, g_CurlMimeClass);
				break;
				
			case CURLOPT_CHUNK_DATA:
			case CURLOPT_CLOSESOCKETDATA:
			case CURLOPT_WRITEDATA:
			case CURLOPT_XFERINFODATA:
				// Pointer
				break;
				
			case CURLOPT_HTTPPOST:
				// Struct (curl_httppost)
				// curl_httppost* pValue;
				break;
				
			case CURLOPT_HTTP200ALIASES:
			case CURLOPT_MAIL_RCPT:
			case CURLOPT_POSTQUOTE:
			case CURLOPT_PREQUOTE:
			case CURLOPT_PROXYHEADER:
			case CURLOPT_TELNETOPTIONS:
			case CURLOPT_HTTPHEADER:
			case CURLOPT_QUOTE:
			case CURLOPT_RESOLVE:
				// Struct (curl_slist)
				// curl_slist* pValue;
				break;
				
			case CURLOPT_CURLU:
			case CURLOPT_DEBUGDATA:
			case CURLOPT_FNMATCH_DATA:
			case CURLOPT_SEEKDATA:
			case CURLOPT_SOCKOPTDATA:
			case CURLOPT_SSH_KEYDATA:
			case CURLOPT_SSL_CTX_DATA:
			case CURLOPT_TRAILERDATA:
			case CURLOPT_HEADERDATA:
			case CURLOPT_INTERLEAVEDATA:
			case CURLOPT_IOCTLDATA:
			case CURLOPT_OPENSOCKETDATA:
			case CURLOPT_PRIVATE:
			case CURLOPT_PROGRESSDATA:
			case CURLOPT_READDATA:
			case CURLOPT_RESOLVER_START_DATA:
			case CURLOPT_HEADEROPT:
				// Unknown (void)
				break;		
		};

		curl_easy_setopt(pCurl, iOptID, szValue);
		return true;

		SDK_ENDTRY;
	});
	
	g_CurlClass.RegisterFunction("perform", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurl, g_CurlClass>();
		CURL* pCurl = pThis->pCurl;

		curl_easy_perform(pCurl);
		return true;

		SDK_ENDTRY;
	});	
	
	g_CurlClass.RegisterFunction("reset", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurl, g_CurlClass>();
		CURL* pCurl = pThis->pCurl;

		curl_easy_reset(pCurl);
		return true;

		SDK_ENDTRY;
	});

	g_CurlClass.RegisterFunction("getInfo", [](Galactic3D::Interfaces::INativeState* pState, int32_t argc, void* pUser) {
		SDK_TRY;

		SDK::State State(pState);

		auto pThis = State.CheckThis<CCurl, g_CurlClass>();
		CURL* pCurl = pThis->pCurl;
		
		int iInfoID;
		State.CheckNumber(0, iInfoID);
		
		CURLINFO pCurlInfo = iInfoID;
		
		template pReturnValue;

		curl_easy_getinfo(pThis, pCurlInfo, pReturnValue);
		
		switch(typeid(pReturnValue))
		{
			case typeid(uint32_t):
				break;
				{
					SDK::NumberValue Value(pReturnValue));
					break;
				}

			case typeid(const char):
			case typeid(char):
			default:
				{
					SDK::StringValue Value(pReturnValue);
					break;
				}
		}
		
		State.Return(Value);		
		return true;

		SDK_ENDTRY;
	});
}

void ModuleUnregister()
{
}

/*
int iOptID;
State.CheckNumber(0, iOptID);

if(iOptID == 1 || iOptID == 2)
    const char* szValue = State.CheckString(1);
else
    int iValue = State.CheckInteger(1);
*/

// For defines once I have a way to add them
/*
CURLOPT_ABSTRACT_UNIX_SOCKET
CURLOPT_ACCEPTTIMEOUT_MS
CURLOPT_ACCEPT_ENCODING
CURLOPT_ADDRESS_SCOPE
CURLOPT_ALTSVC
CURLOPT_ALTSVC_CTRL
CURLOPT_APPEND
CURLOPT_AUTOREFERER
CURLOPT_BUFFERSIZE
CURLOPT_CAINFO
CURLOPT_CAPATH
CURLOPT_CERTINFO
CURLOPT_CHUNK_BGN_FUNCTION
CURLOPT_CHUNK_DATA
CURLOPT_CHUNK_END_FUNCTION
CURLOPT_CLOSESOCKETDATA
CURLOPT_CLOSESOCKETFUNCTION
CURLOPT_CONNECTTIMEOUT
CURLOPT_CONNECTTIMEOUT_MS
CURLOPT_CONNECT_ONLY
CURLOPT_CONNECT_TO
CURLOPT_CONV_FROM_NETWORK_FUNCTION
CURLOPT_CONV_FROM_UTF8_FUNCTION
CURLOPT_CONV_TO_NETWORK_FUNCTION
CURLOPT_COOKIE
CURLOPT_COOKIEFILE
CURLOPT_COOKIEJAR
CURLOPT_COOKIELIST
CURLOPT_COOKIESESSION
CURLOPT_COPYPOSTFIELDS
CURLOPT_CRLF
CURLOPT_CRLFILE
CURLOPT_CURLU
CURLOPT_CUSTOMREQUEST
CURLOPT_DEBUGDATA
CURLOPT_DEBUGFUNCTION
CURLOPT_DEFAULT_PROTOCOL
CURLOPT_DIRLISTONLY
CURLOPT_DISALLOW_USERNAME_IN_URL
CURLOPT_DNS_CACHE_TIMEOUT
CURLOPT_DNS_INTERFACE
CURLOPT_DNS_LOCAL_IP4
CURLOPT_DNS_LOCAL_IP6
CURLOPT_DNS_SERVERS
CURLOPT_DNS_SHUFFLE_ADDRESSES
CURLOPT_DNS_USE_GLOBAL_CACHE
CURLOPT_DOH_URL
CURLOPT_EGDSOCKET
CURLOPT_ERRORBUFFER
CURLOPT_EXPECT_100_TIMEOUT_MS
CURLOPT_FAILONERROR
CURLOPT_FILETIME
CURLOPT_FNMATCH_DATA
CURLOPT_FNMATCH_FUNCTION
CURLOPT_FOLLOWLOCATION
CURLOPT_FORBID_REUSE
CURLOPT_FRESH_CONNECT
CURLOPT_FTPPORT
CURLOPT_FTPSSLAUTH
CURLOPT_FTP_ACCOUNT
CURLOPT_FTP_ALTERNATIVE_TO_USER
CURLOPT_FTP_CREATE_MISSING_DIRS
CURLOPT_FTP_FILEMETHOD
CURLOPT_FTP_RESPONSE_TIMEOUT
CURLOPT_FTP_SKIP_PASV_IP
CURLOPT_FTP_SSL_CCC
CURLOPT_FTP_USE_EPRT
CURLOPT_FTP_USE_EPSV
CURLOPT_FTP_USE_PRET
CURLOPT_GSSAPI_DELEGATION
CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS
CURLOPT_HAPROXYPROTOCOL
CURLOPT_HEADER
CURLOPT_HEADERDATA
CURLOPT_HEADERFUNCTION
CURLOPT_HEADEROPT
CURLOPT_HTTP09_ALLOWED
CURLOPT_HTTP200ALIASES
CURLOPT_HTTPAUTH
CURLOPT_HTTPGET
CURLOPT_HTTPHEADER
CURLOPT_HTTPPOST
CURLOPT_HTTPPROXYTUNNEL
CURLOPT_HTTP_CONTENT_DECODING
CURLOPT_HTTP_TRANSFER_DECODING
CURLOPT_HTTP_VERSION
CURLOPT_IGNORE_CONTENT_LENGTH
CURLOPT_INFILESIZE
CURLOPT_INFILESIZE_LARGE
CURLOPT_INTERFACE
CURLOPT_INTERLEAVEDATA
CURLOPT_INTERLEAVEFUNCTION
CURLOPT_IOCTLDATA
CURLOPT_IOCTLFUNCTION
CURLOPT_IPRESOLVE
CURLOPT_ISSUERCERT
CURLOPT_KEEP_SENDING_ON_ERROR
CURLOPT_KEYPASSWD
CURLOPT_KRBLEVEL
CURLOPT_LOCALPORT
CURLOPT_LOCALPORTRANGE
CURLOPT_LOGIN_OPTIONS
CURLOPT_LOW_SPEED_LIMIT
CURLOPT_LOW_SPEED_TIME
CURLOPT_MAIL_AUTH
CURLOPT_MAIL_FROM
CURLOPT_MAIL_RCPT
CURLOPT_MAXAGE_CONN
CURLOPT_MAXCONNECTS
CURLOPT_MAXFILESIZE
CURLOPT_MAXFILESIZE_LARGE
CURLOPT_MAXREDIRS
CURLOPT_MAX_RECV_SPEED_LARGE
CURLOPT_MAX_SEND_SPEED_LARGE
CURLOPT_MIMEPOST
CURLOPT_NETRC
CURLOPT_NETRC_FILE
CURLOPT_NEW_DIRECTORY_PERMS
CURLOPT_NEW_FILE_PERMS
CURLOPT_NOBODY
CURLOPT_NOPROGRESS
CURLOPT_NOPROXY
CURLOPT_NOSIGNAL
CURLOPT_OPENSOCKETDATA
CURLOPT_OPENSOCKETFUNCTION
CURLOPT_PASSWORD
CURLOPT_PATH_AS_IS
CURLOPT_PINNEDPUBLICKEY
CURLOPT_PIPEWAIT
CURLOPT_PORT
CURLOPT_POST
CURLOPT_POSTFIELDS
CURLOPT_POSTFIELDSIZE
CURLOPT_POSTFIELDSIZE_LARGE
CURLOPT_POSTQUOTE
CURLOPT_POSTREDIR
CURLOPT_PREQUOTE
CURLOPT_PRE_PROXY
CURLOPT_PRIVATE
CURLOPT_PROGRESSDATA
CURLOPT_PROGRESSFUNCTION
CURLOPT_PROTOCOLS
CURLOPT_PROXY
CURLOPT_PROXYAUTH
CURLOPT_PROXYHEADER
CURLOPT_PROXYPASSWORD
CURLOPT_PROXYPORT
CURLOPT_PROXYTYPE
CURLOPT_PROXYUSERNAME
CURLOPT_PROXYUSERPWD
CURLOPT_PROXY_CAINFO
CURLOPT_PROXY_CAPATH
CURLOPT_PROXY_CRLFILE
CURLOPT_PROXY_KEYPASSWD
CURLOPT_PROXY_PINNEDPUBLICKEY
CURLOPT_PROXY_SERVICE_NAME
CURLOPT_PROXY_SSLCERT
CURLOPT_PROXY_SSLCERTTYPE
CURLOPT_PROXY_SSLKEY
CURLOPT_PROXY_SSLKEYTYPE
CURLOPT_PROXY_SSLVERSION
CURLOPT_PROXY_SSL_CIPHER_LIST
CURLOPT_PROXY_SSL_OPTIONS
CURLOPT_PROXY_SSL_VERIFYHOST
CURLOPT_PROXY_SSL_VERIFYPEER
CURLOPT_PROXY_TLS13_CIPHERS
CURLOPT_PROXY_TLSAUTH_PASSWORD
CURLOPT_PROXY_TLSAUTH_TYPE
CURLOPT_PROXY_TLSAUTH_USERNAME
CURLOPT_PROXY_TRANSFER_MODE
CURLOPT_PUT
CURLOPT_QUOTE
CURLOPT_RANDOM_FILE
CURLOPT_RANGE
CURLOPT_READDATA
CURLOPT_READFUNCTION
CURLOPT_REDIR_PROTOCOLS
CURLOPT_REFERER
CURLOPT_REQUEST_TARGET
CURLOPT_RESOLVE
CURLOPT_RESOLVER_START_DATA
CURLOPT_RESOLVER_START_FUNCTION
CURLOPT_RESUME_FROM
CURLOPT_RESUME_FROM_LARGE
CURLOPT_RTSP_CLIENT_CSEQ
CURLOPT_RTSP_REQUEST
CURLOPT_RTSP_SERVER_CSEQ
CURLOPT_RTSP_SESSION_ID
CURLOPT_RTSP_STREAM_URI
CURLOPT_RTSP_TRANSPORT
CURLOPT_SASL_AUTHZID
CURLOPT_SASL_IR
CURLOPT_SEEKDATA
CURLOPT_SEEKFUNCTION
CURLOPT_SERVICE_NAME
CURLOPT_SHARE
CURLOPT_SOCKOPTDATA
CURLOPT_SOCKOPTFUNCTION
CURLOPT_SOCKS5_AUTH
CURLOPT_SOCKS5_GSSAPI_NEC
CURLOPT_SOCKS5_GSSAPI_SERVICE
CURLOPT_SSH_AUTH_TYPES
CURLOPT_SSH_COMPRESSION
CURLOPT_SSH_HOST_PUBLIC_KEY_MD5
CURLOPT_SSH_KEYDATA
CURLOPT_SSH_KEYFUNCTION
CURLOPT_SSH_KNOWNHOSTS
CURLOPT_SSH_PRIVATE_KEYFILE
CURLOPT_SSH_PUBLIC_KEYFILE
CURLOPT_SSLCERT
CURLOPT_SSLCERTTYPE
CURLOPT_SSLENGINE
CURLOPT_SSLENGINE_DEFAULT
CURLOPT_SSLKEY
CURLOPT_SSLKEYTYPE
CURLOPT_SSLVERSION
CURLOPT_SSL_CIPHER_LIST
CURLOPT_SSL_CTX_DATA
CURLOPT_SSL_CTX_FUNCTION
CURLOPT_SSL_ENABLE_ALPN
CURLOPT_SSL_ENABLE_NPN
CURLOPT_SSL_FALSESTART
CURLOPT_SSL_OPTIONS
CURLOPT_SSL_SESSIONID_CACHE
CURLOPT_SSL_VERIFYHOST
CURLOPT_SSL_VERIFYPEER
CURLOPT_SSL_VERIFYSTATUS
CURLOPT_STDERR
CURLOPT_STREAM_DEPENDS
CURLOPT_STREAM_DEPENDS_E
CURLOPT_STREAM_WEIGHT
CURLOPT_SUPPRESS_CONNECT_HEADERS
CURLOPT_TCP_FASTOPEN
CURLOPT_TCP_KEEPALIVE
CURLOPT_TCP_KEEPIDLE
CURLOPT_TCP_KEEPINTVL
CURLOPT_TCP_NODELAY
CURLOPT_TELNETOPTIONS
CURLOPT_TFTP_BLKSIZE
CURLOPT_TFTP_NO_OPTIONS
CURLOPT_TIMECONDITION
CURLOPT_TIMEOUT
CURLOPT_TIMEOUT_MS
CURLOPT_TIMEVALUE
CURLOPT_TIMEVALUE_LARGE
CURLOPT_TLS13_CIPHERS
CURLOPT_TLSAUTH_PASSWORD
CURLOPT_TLSAUTH_TYPE
CURLOPT_TLSAUTH_USERNAME
CURLOPT_TRAILERDATA
CURLOPT_TRAILERFUNCTION
CURLOPT_TRANSFERTEXT
CURLOPT_TRANSFER_ENCODING
CURLOPT_UNIX_SOCKET_PATH
CURLOPT_UNRESTRICTED_AUTH
CURLOPT_UPKEEP_INTERVAL_MS
CURLOPT_UPLOAD
CURLOPT_UPLOAD_BUFFERSIZE
CURLOPT_URL
CURLOPT_USERAGENT
CURLOPT_USERNAME
CURLOPT_USERPWD
CURLOPT_USE_SSL
CURLOPT_VERBOSE
CURLOPT_WILDCARDMATCH
CURLOPT_WRITEDATA
CURLOPT_WRITEFUNCTION
CURLOPT_XFERINFODATA
CURLOPT_XFERINFOFUNCTION
CURLOPT_XOAUTH2_BEARER
*/

// For Reference
	//curl_easy_setopt(handle, CURLOPT_URL, szURL);
	//curl_easy_setopt(handle, CURLOPT_POST, szURL);
	//size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp); 
	//curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data); 
	//curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &internal_struct); 
	//success = curl_easy_perform(easyhandle); 	