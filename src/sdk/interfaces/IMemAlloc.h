#pragma once
#pragma warning(disable:4191)
#include <cassert>
#include <cstddef>

#include "../helpers/vfunc.h"

class IMemAlloc 
{
public:
    void* Alloc(std::size_t size) 
    {
        return GetVirtual<void*(__thiscall*)(IMemAlloc*, std::size_t)>(this, 1)(this, size);
    }

    void* ReAlloc(const void* p, std::size_t size) 
    {
        return GetVirtual<void*(__thiscall*)(IMemAlloc*, const void*, std::size_t)>(this, 2)(this, p, size);
    }

    void Free(const void* p) 
    {
        return GetVirtual<void(__thiscall*)(IMemAlloc*, const void*)>(this, 3)(this, p);
    }

    std::size_t GetSize(const void* p) 
    {
        return GetVirtual<std::size_t(__thiscall*)(IMemAlloc*, const void*)>(this, 21)(this, p);
    }
};


