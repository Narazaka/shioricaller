#ifndef SHIORICALLER_H__
#define SHIORICALLER_H__
#include "shiori_define.h"

// ^ from shiori.h

typedef BOOL (*LOAD)(const MEMORY_HANDLE,long);
typedef MEMORY_HANDLE (*REQUEST)(const MEMORY_HANDLE,long*);
typedef BOOL (*UNLOAD)(void);

extern "C" class __declspec(dllexport) ShioriCaller {
public:
    inline static ShioriCaller* new_class(const char* dllpath) {
        return new ShioriCaller(dllpath);
    }
    inline static void delete_class(ShioriCaller* instance) {
        delete instance;
    }

    const char* load_error;

    ShioriCaller(const char* dllpath);
    ~ShioriCaller();
    bool load(const char* dirpath) const;
    char* request(const char* req) const;
    bool unload() const;
private:
    HMODULE handle;
    LOAD shiori_load;
    REQUEST shiori_request;
    UNLOAD shiori_unload;
};

#endif // SHIORICALLER_H__
