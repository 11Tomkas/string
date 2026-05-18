#ifndef STRING_HPP
#define STRING_HPP

#include <iterator>
#include <initializer_list>
#include <ostream>

class String
{
    public:
        using value_type = char;
        using size_type = unsigned long long;
        using difference_type = long long;
        using reference = char&;
        using const_reference = const char&;
        using pointer = char*;
        using const_pointer = const char*;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<pointer>;
        using const_reverse_iterator = std::reverse_iterator<const_pointer>;

        static const size_type npos;

        String();
        String(size_type count, value_type chr);
        template <typename InputIt>
            String(InputIt first, InputIt last);
        String(const_pointer string, size_type count);
        String(const_pointer string);
        String(std::nullptr_t) = delete;
        String(const String& other);
        String(String&& other);
        String(const String& other, size_type index);
        String(String&& other, size_type index);
        String(const String& other, size_type index, size_type count);
        String(String&& other, size_type index, size_type count);
        String(std::initializer_list<value_type> list);
        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other);
        String& operator=(const_pointer string);
        String& operator=(value_type chr);
        String& operator=(std::initializer_list<value_type> list);
        String& operator=(std::nullptr_t) = delete;

        String& assign(const String& other);
        String& assign(String&& other);
        String& assign(size_type count, value_type chr);
        String& assign(const_pointer string, size_type count);
        String& assign(const_pointer string);
        String& assign(const String& other, size_type index, size_type count=npos);
        template <typename InputIt>
            String& assign(InputIt first, InputIt last);
        String& assign(std::initializer_list<value_type> list);

        reference at(size_type index);
        const_reference at(size_type index) const;
        reference operator[](size_type index);
        const_reference operator[](size_type index) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        const_pointer data() const;
        pointer data();
        const_pointer c_str() const;

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator crbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        const_reverse_iterator crend() const;

        bool empty() const;
        size_type size() const;
        size_type length() const;
        size_type max_size() const;
        void reserve(size_type newCapacity);
        size_type capacity() const;
        void shrink_to_fit();

        void clear();
        String& insert(size_type index, size_type count, value_type chr);
        String& insert(size_type index, const_pointer string);
        String& insert(size_type index, const_pointer string, size_type count);
        String& insert(size_type index, const String& other);
        String& insert(size_type index, const String& other, size_type otherIndex, size_type count=npos);
        iterator insert(const_iterator iter, value_type chr);
        iterator insert(const_iterator iter, size_type count, value_type chr);
        template <typename InputIt>
            iterator insert(const_iterator iter, InputIt first, InputIt last);
        iterator insert(const_iterator iter, std::initializer_list<value_type> list);

        String& erase(size_type index=0, size_type count=npos);
        iterator erase(const_iterator iter);
        iterator erase(const_iterator first, const_iterator last);
    private:
        pointer m_string;
        size_type m_capacity;
        size_type m_count;

        void m_chrset(pointer dest, value_type chr, size_type count);
        void m_strcpy(pointer dest, const_pointer src, size_type count);
        size_type m_strlen(const_pointer string);
        size_type m_min(size_type value1, size_type value2);

        void m_reserveEmpty(size_type newCapacity);
        void m_reserveOuter(size_type newCapacity, size_type index, size_type count);

    friend std::ostream& operator<<(std::ostream& ostream, const String& string);
};

template <typename InputIt>
String::String(InputIt first, InputIt last)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    size_type count{ 0 };

    for (InputIt it{ first }; it != last; ++it)
        ++count;

    if (count > m_capacity)
        m_capacity = count;

    m_string = new value_type[m_capacity + 1];
    for (size_type i{ 0 }; i < count; ++i)
        m_string[i] = *(first++);
    m_string[count] = '\0';
    m_count = count;
}

template <typename InputIt>
String& String::assign(InputIt first, InputIt last)
{
    size_type count{ 0 };

    for (InputIt it{ first }; it != last; ++it)
        ++count;

    m_reserveEmpty(count);

    for (size_type i{ 0 }; i < count; ++i)
        m_string[i] = *(first++);
    m_string[count] = '\0';
    m_count = count;

    return *this;
}

template <typename InputIt>
String::iterator String::insert(const_iterator iter, InputIt first, InputIt last)
{
    size_type index{ static_cast<size_type>(iter - cbegin()) };

    if (first == last)
        return begin() + index;

    size_type count{ 0 };

    for (InputIt it{ first }; it != last; ++it)
        ++count;

    size_type totalCount{ m_count + count };

    m_reserveOuter(totalCount, index, count);

    for (size_type i{ 0 }; i < count; ++i)
        m_string[index + i] = *(first++);
    m_string[totalCount] = '\0';
    m_count = totalCount;

    return begin() + index;
}

#endif