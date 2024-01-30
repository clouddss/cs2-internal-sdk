#pragma once

template <typename S, typename T>
class CUtlMap
{
public:
    inline CUtlMap<S, T> begin() noexcept
    {
        return base_;
    }

    inline CUtlMap<S, T> end() noexcept
    {
        return base_ + size_;
    }

    [[nodiscard]] CUtlMap<S, T> begin() const
    {
        return base_;
    }

    [[nodiscard]] CUtlMap<S, T> end() const
    {
        return base_ + size_;
    }

    T& operator[](int i)
    {
        return base_[i];
    }

    S& Count() const;
    S& Count();

    T& Element(int i);
    const T& Element(int i) const;

private:
    T* base_ = nullptr;
    S size_ = 0;
};

