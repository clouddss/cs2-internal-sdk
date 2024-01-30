#pragma once

template <class T>
class CUtlVector 
{
public:
    CUtlVector() 
    {
        m_Size = 0;
        m_pElements = nullptr;
    }

    T& Element(int i);
    const T& Element(int i) const;

    T& operator[](int i) 
    {
        return m_pElements[i];
    }

    [[nodiscard]] int Count() const 
    {
        return m_Size;
    }

    int m_Size;
    T* m_pElements;

    // Adds an element, uses default constructor
    int AddToHead();
    int AddToTail();
    int InsertBefore(int elem);
    int InsertAfter(int elem);
    // Adds an element, uses copy constructor
    int AddToHead(const T& src);
    int AddToTail(const T& src);
    int InsertBefore(int elem, const T& src);
    int InsertAfter(int elem, const T& src);

    // Grows the vector
    void GrowVector(int num = 1);

    // Shifts elements....
    void ShiftElementsRight(int elem, int num = 1);
    void ShiftElementsLeft(int elem, int num = 1);

    // Element removal
    void FastRemove(int elem); // doesn't preserve order
    void Remove(int elem); // preserves order, shifts elements
    bool FindAndRemove(const T& src); // removes first occurrence of src, preserves order, shifts elements
    bool FindAndFastRemove(const T& src); // removes first occurrence of src, doesn't preserve order

    // Finds an element (element needs operator== defined)
    int GetOffset(const T& src) const;
};


