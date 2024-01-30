#pragma once
#include "../helpers/vfunc.h"

template <class T>
inline T* Construct(T* pMemory) 
{
    return ::new (pMemory) T;
}

template <class T, typename ARG1>
inline T* Construct(T* pMemory, ARG1 a1) 
{
    return ::new (pMemory) T(a1);
}

template <class T, typename ARG1, typename ARG2>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2) 
{
    return ::new (pMemory) T(a1, a2);
}

template <class T, typename ARG1, typename ARG2, typename ARG3>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3) 
{
    return ::new (pMemory) T(a1, a2, a3);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4) 
{
    return ::new (pMemory) T(a1, a2, a3, a4);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
inline T* Construct(T* pMemory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4, ARG5 a5) 
{
    return ::new (pMemory) T(a1, a2, a3, a4, a5);
}

template <class T>
inline T* CopyConstruct(T* pMemory, const T& src) 
{
    return ::new (pMemory) T(src);
}

template <class T>
inline void Destruct(T* pMemory) 
{
    pMemory->~T();

#ifdef _DEBUG
    memset(pMemory, 0xDD, sizeof(T));
#endif
}

template <class T, class I = int>
class CUtlMemory 
{
public:
    // constructor, destructor
    CUtlMemory(int nGrowSize = 0, int nInitSize = 0);
    CUtlMemory(T* pMemory, int numElements);
    CUtlMemory(const T* pMemory, int numElements);
    ~CUtlMemory();

    // Set the size by which the memory grows
    void Init(int nGrowSize = 0, int nInitSize = 0);

    class Iterator_t 
    {
    public:
        Iterator_t(I i): index(i) { }

        I index;

        bool operator==(const Iterator_t& it) const 
        {
            return index == it.index;
        }
        bool operator!=(const Iterator_t& it) const 
        {
            return index != it.index;
        }
    };

    Iterator_t First() const 
    {
        return Iterator_t(IsIdxValid(0) ? 0 : InvalidIndex());
    }

    Iterator_t Next(const Iterator_t& it) const 
    {
        return Iterator_t(IsIdxValid(it.index + 1) ? it.index + 1 : InvalidIndex());
    }

    I GetIndex(const Iterator_t& it) const 
    {
        return it.index;
    }
    bool IsIdxAfter(I i, const Iterator_t& it) const 
    {
        return i > it.index;
    }
    bool IsValidIterator(const Iterator_t& it) const 
    {
        return IsIdxValid(it.index);
    }
    Iterator_t InvalidIterator() const 
    {
        return Iterator_t(InvalidIndex());
    }

    // element access
    T& operator[](I i);
    const T& operator[](I i) const;
    T& Element(I i);
    const T& Element(I i) const;

    bool IsIdxValid(I i) const;

    static const I INVALID_INDEX = (I)-1; // For use with COMPILE_TIME_ASSERT
    static I InvalidIndex() 
    {
        return INVALID_INDEX;
    }

    T* Base();
    const T* Base() const;

    void SetExternalBuffer(T* pMemory, int numElements);
    void SetExternalBuffer(const T* pMemory, int numElements);
    void AssumeMemory(T* pMemory, int nSize);
    T* Detach();
    void* DetachMemory();

    void Swap(CUtlMemory<T, I>& mem);
    void ConvertToGrowableMemory(int nGrowSize);
    int NumAllocated() const;
    int Count() const;
    void Grow(int num = 1);
    void EnsureCapacity(int num);
    void Purge();
    void Purge(int numElements);
    bool IsExternallyAllocated() const;
    bool IsReadOnly() const;
    void SetGrowSize(int size);
protected:
    void ValidateGrowSize() const { }

    enum {
        EXTERNAL_BUFFER_MARKER = -1,
        EXTERNAL_CONST_BUFFER_MARKER = -2,
    };
public:
    T* m_pMemory;
    int m_nAllocationCount;
    int m_nGrowSize;
};




