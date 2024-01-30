#include "CUtlMap.h"

template <typename S, typename T>
inline S& CUtlMap<S, T>::Count() const
{
    return size_;
}

template <typename S, typename T>
inline S& CUtlMap<S, T>::Count()
{
    return size_;
}

template <typename S, typename T>
inline T& CUtlMap<S, T>::Element(int i)
{
    assert(i < size_);
    return base_[i];
}

template <typename S, typename T>
const T& CUtlMap<S, T>::Element(int i) const
{
    assert(i < size_);
    return base_[i];
}