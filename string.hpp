#ifndef STRING_HPP
#define STRING_HPP

#include <iterator>
#include <initializer_list>
#include <iostream>

class String
{
    struct DefContructor {};

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
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static const size_type npos;

        String(DefContructor);
        String();
        String(size_type count, value_type ch);
        String(const_iterator first, const_iterator end);
        String(const_pointer s, size_type count);
        String(std::nullptr_t) = delete;
        String(const String& other);
        String(String&& other);
        String(const String& other, size_type pos);
        String(String&& other, size_type pos);
        String(const String& other, size_type pos, size_type count);
        String(String&& other, size_type pos, size_type count);
        String(std::initializer_list<value_type> ilist);
        ~String();

    private:
        pointer m_string;
        size_type m_capacity;
        size_type m_count;

    friend std::ostream& operator<<(std::ostream& ostream, const String& str);

    friend void printString(const String& string);
};

#endif