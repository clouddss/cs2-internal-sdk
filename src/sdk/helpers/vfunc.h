#pragma once

//Example usage:
//GetVirtual<void*(__thiscall*)(void*, optional_arguments)>(this, func_index)(this, optional_arguments_value);
//GetVirtual<void*(__thiscall*)(IMemAlloc*, std::size_t)>(this, 1)(this, size);

template<typename FuncType>
__forceinline static FuncType GetVirtual(void* ppClass, int index)
{
    return (*static_cast<FuncType**>(ppClass))[index];
}