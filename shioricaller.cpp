#include <iostream>
#include <string>
#include <cstdlib>

#if defined(WIN32)||defined(_WIN32)||defined(_Windows)||defined(__CYGWIN__)
	#include <windows.h>
	#define MEMORY_HANDLE HGLOBAL
	#define SHIORI_MALLOC(len) GlobalAlloc(GMEM_FIXED, len)
	#define SHIORI_FREE(ptr) GlobalFree((HGLOBAL)ptr)
#else
	#define MEMORY_HANDLE char *
	#define SHIORI_MALLOC(len) malloc(len)
	#define SHIORI_FREE(ptr) free((void *)ptr)
#endif
#define BOOL bool
typedef BOOL (*LOAD)(const MEMORY_HANDLE,long);
typedef MEMORY_HANDLE (*REQUEST)(const MEMORY_HANDLE,long*);
typedef BOOL (*UNLOAD)(void);

using namespace std;

BOOL call_load(LOAD load, const string dirpath){
	long len = dirpath.length();
	MEMORY_HANDLE h = (MEMORY_HANDLE)SHIORI_MALLOC(len);
	memcpy(h, dirpath.c_str(), len);
	return (*load)(h, len);
}

string call_request(REQUEST request, const string req){
	long* len = (long*)SHIORI_MALLOC(sizeof(long));
	*len = req.length();
	MEMORY_HANDLE h = (MEMORY_HANDLE)SHIORI_MALLOC(*len);
	memcpy(h, req.c_str(), *len);
	MEMORY_HANDLE rh = (*request)(h, len);
	string res = string((const char*)rh, *len);
	SHIORI_FREE(rh);
	return res;
}

BOOL call_unload(UNLOAD unload){
	return (*unload)();
}

int main(int argc, char* argv[]){
	if(argc != 3){
		cout << "Usage: shioricaller shiori.dll dirpath < request.txt > response.txt" << endl;
		return 0;
	}
	const string dll = argv[1];
	const string dirpath = argv[2];

	HMODULE handle = LoadLibrary(dll.c_str());
	if(handle == NULL){
		cout << "cannot load dll [" << dll << "] code [" << GetLastError() << "]" << endl;
		return 0;
	}

	LOAD load = (LOAD)GetProcAddress(handle, "load");
	REQUEST request = (REQUEST)GetProcAddress(handle, "request");
	UNLOAD unload = (UNLOAD)GetProcAddress(handle, "unload");

	if(load == NULL || request == NULL || unload == NULL){
		cout << "cannot get functions load, request, unload [" << dll << "]" << endl;
		FreeLibrary(handle);
		return 0;
	}

	if(call_load(load, dirpath) != 1){
		cout << "load returns non 1 [" << dll << "]" << endl;
		FreeLibrary(handle);
		return 0;
	}

	string req;
	string req_line;
	while(1){
		getline(cin, req_line);
		req += req_line;
		if(!cin.eof()){
			req += "\r\n";
		}else{
			break;
		}
	}
	string res = call_request(request, req);
	cout << res << endl;

	if(call_unload(unload) != 1){
		cout << "unload returns non 1 [" << dll << "]" << endl;
		FreeLibrary(handle);
		return 0;
	}

	FreeLibrary(handle);

	return 0;
}
