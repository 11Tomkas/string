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
        template <typename InputIterator>
            String(InputIterator first, InputIterator last);
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
        template <typename InputIterator>
            String& assign(InputIterator first, InputIterator last);
        String& assign(std::initializer_list<value_type> list);
    private:
        pointer m_string;
        size_type m_capacity;
        size_type m_count;

        void m_chrset(pointer dest, value_type chr, size_type count);
        void m_strcpy(pointer dest, const_pointer src, size_type count);
        size_type m_strlen(const_pointer string);
        size_type m_min(size_type value1, size_type value2);

    friend std::ostream& operator<<(std::ostream& ostream, const String& string);
};

template <typename InputIterator>
String::String(InputIterator first, InputIterator last)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    size_type count{ 0 };

    for (InputIterator it{ first }; it != last; ++it)
        ++count;

    if (count > m_capacity)
        m_capacity = count;

    m_string = new char[m_capacity + 1];
    for (size_type i{ 0 }; i < count; ++i)
        m_string[i] = *(first++);
    m_string[count] = '\0';
    m_count = count;
}

template <typename InputIterator>
String& String::assign(InputIterator first, InputIterator last)
{
    size_type count{ 0 };

    for (InputIterator it{ first }; it != last; ++it)
        ++count;

    if (count <= m_capacity)
    {
        for (size_type i{ 0 }; i < count; ++i)
            m_string[i] = *(first++);
        m_string[count] = '\0';
        m_count = count;
    }
    else
    if (count > m_capacity)
    {
        if (count <= m_capacity * 2)
            m_capacity *= 2;
        else
        if (count > m_capacity * 2)
            m_capacity = count;

        delete[] m_string;
        m_string = new char[m_capacity + 1];
        for (size_type i{ 0 }; i < count; ++i)
            m_string[i] = *(first++);
        m_string[count] = '\0';
        m_count = count;
    }

    return *this;
}

#endif