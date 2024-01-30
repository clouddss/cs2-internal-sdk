#pragma once

#include <iostream>

class BitFlag 
{
public:
    BitFlag() = default;

    explicit BitFlag(const std::uintptr_t flag) : flag(flag) {}

    __forceinline void AddFlag(const std::uintptr_t flag) 
    { 
        this->flag |= flag;
    }

    [[nodiscard]] __forceinline bool HasFlag(const std::uintptr_t flag) const 
    {
        return this->flag & flag;
    }

    __forceinline void RemoveFlag(const std::uintptr_t flag) 
    { 
        this->flag &= ~flag; 
    }

    [[nodiscard]] __forceinline std::uintptr_t get_flag() const 
    { 
        return this->flag; 
    }

private:
    std::uintptr_t flag;
};