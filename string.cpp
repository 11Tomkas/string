#include <cstring>
#include <algorithm>
#include "string.hpp"

const String::size_type String::npos{ static_cast<size_type>(-1) };

String::String(DefContructor)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
}

String::String()
    : String(DefContructor{})
{
    m_string = new value_type[m_capacity + 1];
    m_string[0] = '\0';
}

String::String(size_type count, value_type ch)
    : String(DefContructor{})
{
    if (count == 0)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (count > 0)
    {
        if (count > m_capacity)
            m_capacity = count;

        m_string = new value_type[m_capacity + 1];
        std::memset(m_string, ch, count);
        m_string[count] = '\0';
        m_count = count;
    }
}

String::String(const_iterator first, const_iterator end)
    : String(first, static_cast<size_type>(end - first))
{
}

String::String(const_pointer s, size_type count)
    : String(DefContructor{})
{
    if (count == 0)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (count > 0)
    {
        if (count > m_capacity)
            m_capacity = count;

        m_string = new value_type[m_capacity + 1];
        std::memcpy(m_string, s, count);
        m_string[count] = '\0';
        m_count = count;
    }
}

String::String(const String& other)
    : String(DefContructor{})
{
    if (other.m_count == 0)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (other.m_count > 0)
    {
        m_string = new value_type[other.m_capacity + 1];
        std::memcpy(m_string, other.m_string, other.m_count);
        m_string[other.m_count] = '\0';
        m_capacity = other.m_capacity;
        m_count = other.m_count;
    }
}

String::String(String&& other)
    : String(DefContructor{})
{
    m_string = other.m_string;
    m_capacity = other.m_capacity;
    m_count = other.m_count;

    other.m_capacity = 15;
    other.m_string = new value_type[other.m_capacity + 1];
    other.m_string[0] = '\0';
    other.m_count = 0;
}

String::String(const String& other, size_type pos)
    : String(other, pos, (other.m_count - pos))
{
}

String::String(String&& other, size_type pos)
    : String(other, pos, (other.m_count - pos))
{
}

String::String(const String& other, size_type pos, size_type count)
    : String(DefContructor{})
{
    if (pos >= other.m_count)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (pos < other.m_count)
    {
        if (count == 0)
        {
            m_string = new value_type[m_capacity + 1];
            m_string[0] = '\0';
        }
        else
        if (count > 0)
        {
            count = std::min(count, (other.m_count - pos));
            if (count > m_capacity)
                m_capacity = count;

            m_string = new value_type[m_capacity + 1];
            std::memcpy(m_string, (other.m_string + pos), count);
            m_string[count] = '\0';
            m_count = count;
        }
    }
}

String::String(String&& other, size_type pos, size_type count)
    : String(other, pos, count)
{
}

String::String(std::initializer_list<value_type> ilist)
    : String(ilist.begin(), ilist.size())
{
}

String::~String() {
    delete[] m_string;
}

std::ostream& operator<<(std::ostream& ostream, const String& str)
{
    String::size_type index;

    for (index = 0; index < str.m_count; ++index)
        ostream << str.m_string[index];

    return ostream;
}