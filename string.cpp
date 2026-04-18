#include "string.hpp"

const String::size_type String::npos{ static_cast<size_type>(-1) };

String::String()
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    m_string = new value_type[m_capacity + 1];
    m_string[0] = '\0';
}

String::String(size_type count, value_type chr)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    if (count > m_capacity)
        m_capacity = count;

    m_string = new value_type[m_capacity + 1];
    m_chrset(m_string, chr, count);
    m_string[count] = '\0';
    m_count = count;
}

String::String(const_pointer string, size_type count)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    if (count > m_capacity)
        m_capacity = count;

    m_string = new value_type[m_capacity + 1];
    m_strcpy(m_string, string, count);
    m_string[count] = '\0';
    m_count = count;
}

String::String(const_pointer string)
    : String(string, m_strlen(string))
{
}

String::String(const String& other)
    : String(other.m_string, other.m_count)
{
}

String::String(String&& other)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    m_string = other.m_string;
    m_capacity = other.m_capacity;
    m_count = other.m_count;

    other.m_capacity = 15;
    other.m_string = new value_type[other.m_capacity + 1];
    other.m_string[0] = '\0';
    other.m_count = 0;
}

String::String(const String& other, size_type index)
    : String(other, index, (other.m_count - index))
{
}

String::String(String&& other, size_type index)
    : String(std::move(other), index, (other.m_count - index))
{
}

String::String(const String& other, size_type index, size_type count)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    if (index >= other.m_count)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (index < other.m_count)
    {
        count = m_min(count, (other.m_count - index));
        if (count > m_capacity)
            m_capacity = count;

        m_string = new value_type[m_capacity + 1];
        m_strcpy(m_string, (other.m_string + index), count);
        m_string[count] = '\0';
        m_count = count;
    }
}

String::String(String&& other, size_type index, size_type count)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    if (index >= other.m_count)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (index < other.m_count)
    {
        m_string = other.m_string;
        m_capacity = other.m_capacity;
        m_count = other.m_count;
        count = m_min(count, (m_count - index));
        m_strcpy(m_string, (m_string + index), count);
        m_string[count] = '\0';
        m_count = count;

        other.m_capacity = 15;
        other.m_string = new value_type[other.m_capacity + 1];
        other.m_string[0] = '\0';
        other.m_count = 0;
    }
}

String::String(std::initializer_list<value_type> list)
    : String(list.begin(), list.size())
{
}

String::~String() {
    delete[] m_string;
}

String& String::operator=(const String& other)
{
    return assign(other);
}

String& String::operator=(String&& other)
{
    return assign(std::move(other));
}

String& String::operator=(const_pointer string)
{
    return assign(string, m_strlen(string));
}

String& String::operator=(value_type chr)
{
    return assign(1, chr);
}

String& String::operator=(std::initializer_list<value_type> list)
{
    return assign(list.begin(), list.size());
}

String& String::assign(const String& other)
{
    if (this == &other)
        return *this;

    return assign(other.m_string, other.m_count);
}

String& String::assign(String&& other)
{
    if (this == &other)
        return *this;

    delete[] m_string;
    m_string = other.m_string;
    m_capacity = other.m_capacity;
    m_count = other.m_count;

    other.m_capacity = 15;
    other.m_string = new value_type[other.m_capacity + 1];
    other.m_string[0] = '\0';
    other.m_count = 0;

    return *this;
}

String& String::assign(size_type count, value_type chr)
{
    if (count <= m_capacity)
    {
        m_chrset(m_string, chr, count);
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
        m_string = new value_type[m_capacity + 1];
        m_chrset(m_string, chr, count);
        m_string[count] = '\0';
        m_count = count;
    }

    return *this;
}

String& String::assign(const_pointer string, size_type count)
{
    if (count <= m_capacity)
    {
        m_strcpy(m_string, string, count);
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
        m_string = new value_type[m_capacity + 1];
        m_strcpy(m_string, string, count);
        m_string[count] = '\0';
        m_count = count;
    }

    return *this;
}

String& String::assign(const_pointer string)
{
    return assign(string, m_strlen(string));
}

String& String::assign(const String& other, size_type index, size_type count)
{
    if (index >= other.m_count)
    {
        m_string[0] = '\0';
        m_count = 0;
    }
    else
    if (index < other.m_count)
    {
        count = m_min(count, (other.m_count - index));

        if (count <= m_capacity)
        {
            m_strcpy(m_string, (other.m_string + index), count);
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
            m_string = new value_type[m_capacity + 1];
            m_strcpy(m_string, (other.m_string + index), count);
            m_string[count] = '\0';
            m_count = count;
        }
    }

    return *this;
}

String& String::assign(std::initializer_list<value_type> list)
{
    size_type count{ list.size() };

    if (count <= m_capacity)
    {
        m_strcpy(m_string, list.begin(), count);
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
        m_string = new value_type[m_capacity + 1];
        m_strcpy(m_string, list.begin(), count);
        m_string[count] = '\0';
        m_count = count;
    }

    return *this;
}

void String::m_chrset(pointer dest, value_type chr, size_type count)
{
    for (size_type i{ 0 }; i < count; ++i)
        dest[i] = chr;
}

void String::m_strcpy(pointer dest, const_pointer src, size_type count)
{
    for (size_type i{ 0 }; i < count; ++i)
        dest[i] = src[i];
}

String::size_type String::m_strlen(const_pointer string)
{
    size_type count{ 0 };

    for (; *string != '\0'; ++string)
        ++count;

    return count;
}

String::size_type String::m_min(size_type value1, size_type value2) {
    return (value1 < value2) ? value1 : value2;
}

std::ostream& operator<<(std::ostream& ostream, const String& string)
{
    for (String::size_type i{ 0 }; i < string.m_count; ++i)
        ostream << string.m_string[i];

    return ostream;
}