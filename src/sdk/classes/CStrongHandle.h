#pragma once
struct ResourceBinding_t 
{
    void* data;
};

template <class T>
class CStrongHandle 
{
public:
    explicit operator T* () const 
    {
        if (!IsValid())
            return nullptr;

        return reinterpret_cast<T*>(resource_binding->data);
    }

    T* operator->() const 
    {
        if (!IsValid())
            return nullptr;

        return reinterpret_cast<T*>(resource_binding->data);
    }

    [[nodiscard]] bool IsValid() const 
    { 
        return resource_binding->data != nullptr; 
    }

private:
    const ResourceBinding_t* resource_binding;
};