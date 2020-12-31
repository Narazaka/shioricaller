#include "shioricaller.hpp"

ShioriCaller::ShioriCaller(const char* dllpath) {
    handle = LoadLibrary(dllpath);
    if (handle == nullptr) {
        load_error = "LoadLibrary";
        return;
    }
	shiori_load = (LOAD)GetProcAddress(handle, "load");
    if (shiori_load == nullptr) {
        load_error = "load";
        FreeLibrary(handle);
        handle = nullptr;
        return;
    }
	shiori_request = (REQUEST)GetProcAddress(handle, "request");
    if (shiori_request == nullptr) {
        load_error = "request";
        FreeLibrary(handle);
        handle = nullptr;
        return;
    }
	shiori_unload = (UNLOAD)GetProcAddress(handle, "unload");
    if (shiori_unload == nullptr) {
        load_error = "unload";
        FreeLibrary(handle);
        handle = nullptr;
        return;
    }
    load_error = nullptr;
}

ShioriCaller::~ShioriCaller() {
    if (handle != nullptr) FreeLibrary(handle);
}

bool ShioriCaller::load(const char* dirpath) const {
	long len = strlen(dirpath);
	MEMORY_HANDLE h = (MEMORY_HANDLE)SHIORI_MALLOC(len);
	memcpy(h, dirpath, len);
	return (*shiori_load)(h, len);
}

char* ShioriCaller::request(const char* req) const {
	long* len = (long*)SHIORI_MALLOC(sizeof(long));
	*len = strlen(req);
	MEMORY_HANDLE h = (MEMORY_HANDLE)SHIORI_MALLOC(*len);
	memcpy(h, req, *len);
	MEMORY_HANDLE rh = (*shiori_request)(h, len);
    char* res = (char*)malloc(sizeof(char) * (*len + 1));
    memcpy(res, rh, *len);
	SHIORI_FREE(rh);
    res[*len] = '\0';
    free(len);
	return res;
}

bool ShioriCaller::unload() const {
	return (*shiori_unload)();
}
