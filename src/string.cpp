#include "string.hpp"
#include <stdexcept>
#include <utility>

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
    : String(other, 0, npos)
{
}

String::String(String&& other)
    : String(std::move(other), 0, npos)
{
}

String::String(const String& other, size_type index)
    : String(other, index, npos)
{
}

String::String(String&& other, size_type index)
    : String(std::move(other), index, npos)
{
}

String::String(const String& other, size_type index, size_type count)
    : m_string{ nullptr }
    , m_capacity{ 15 }
    , m_count{ 0 }
{
    if (index > other.m_count)
        throw std::out_of_range{ "out of range" };
    if (index == other.m_count)
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
    : String()
{
    if (index > other.m_count)
        throw std::out_of_range{ "out of range" };
    if (index == other.m_count)
    {
        m_string = new value_type[m_capacity + 1];
        m_string[0] = '\0';
    }
    else
    if (index < other.m_count)
    {
        std::swap(m_string, other.m_string);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_count, other.m_count);

        count = m_min(count, (other.m_count - index));

        if (index > 0 || count < other.m_count)
        {
            if (index == 0)
            {
                m_string[count] = '\0';
                m_count = count;
            }
            else
            if (index > 0)
            {
                m_strcpy(m_string, (m_string + index), count);
                m_string[count] = '\0';
                m_count = count;
            }
        }
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
    return assign(string);
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

    return assign(other, 0, npos);
}

String& String::assign(String&& other)
{
    if (this == &other)
        return *this;

    std::swap(m_string, other.m_string);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_count, other.m_count);

    other.clear();

    return *this;
}

String& String::assign(size_type count, value_type chr)
{
    m_reserveEmpty(count);

    m_chrset(m_string, chr, count);
    m_string[count] = '\0';
    m_count = count;

    return *this;
}

String& String::assign(const_pointer string, size_type count)
{
    m_reserveEmpty(count);

    m_strcpy(m_string, string, count);
    m_string[count] = '\0';
    m_count = count;

    return *this;
}

String& String::assign(const_pointer string)
{
    return assign(string, m_strlen(string));
}

String& String::assign(const String& other, size_type index, size_type count)
{
    if (index > other.m_count)
        throw std::out_of_range{ "out of range" };
    if (index == other.m_count)
    {
        m_string[0] = '\0';
        m_count = 0;
    }
    else
    if (index < other.m_count)
    {
        count = m_min(count, (other.m_count - index));

        m_reserveEmpty(count);

        m_strcpy(m_string, (other.m_string + index), count);
        m_string[count] = '\0';
        m_count = count;
    }

    return *this;
}

String& String::assign(std::initializer_list<value_type> list)
{
    return assign(list.begin(), list.size());
}

String::reference String::at(size_type index)
{
    if (index >= m_count)
        throw std::out_of_range{ "out of range" };

    return m_string[index];
}

String::const_reference String::at(size_type index) const
{
    if (index >= m_count)
        throw std::out_of_range{ "out of range" };

    return m_string[index];
}

String::reference String::operator[](size_type index)
{
    return m_string[index];
}

String::const_reference String::operator[](size_type index) const
{
    return m_string[index];
}

String::reference String::front()
{
    return m_string[0];
}

String::const_reference String::front() const
{
    return m_string[0];
}

String::reference String::back()
{
    return m_string[m_count - 1];
}

String::const_reference String::back() const
{
    return m_string[m_count - 1];
}

String::const_pointer String::data() const
{
    return m_string;
}

String::pointer String::data()
{
    return m_string;
}

String::const_pointer String::c_str() const
{
    return m_string;
}

String::iterator String::begin()
{
    return m_string;
}

String::const_iterator String::begin() const
{
    return m_string;
}

String::const_iterator String::cbegin() const
{
    return m_string;
}

String::iterator String::end()
{
    return m_string + m_count;
}

String::const_iterator String::end() const
{
    return m_string + m_count;
}

String::const_iterator String::cend() const
{
    return m_string + m_count;
}

String::reverse_iterator String::rbegin()
{
    return reverse_iterator{ end() };
}

String::const_reverse_iterator String::rbegin() const
{
    return const_reverse_iterator{ end() };
}

String::const_reverse_iterator String::crbegin() const
{
    return const_reverse_iterator{ end() };
}

String::reverse_iterator String::rend()
{
    return reverse_iterator{ begin() };
}

String::const_reverse_iterator String::rend() const
{
    return const_reverse_iterator{ begin() };
}

String::const_reverse_iterator String::crend() const
{
    return const_reverse_iterator{ begin() };
}

bool String::empty() const
{
    return m_count == 0;
}

String::size_type String::size() const
{
    return m_count;
}

String::size_type String::length() const
{
    return m_count;
}

String::size_type String::max_size() const
{
    return 0x7FFFFFFFFFFFFFFFu;
}

void String::reserve(size_type newCapacity)
{
    if (newCapacity > m_capacity)
    {
        if (newCapacity <= m_capacity * 2)
            m_capacity *= 2;
        else
        if (newCapacity > m_capacity * 2)
            m_capacity = newCapacity;

        pointer newString{ new value_type[m_capacity + 1] };

        m_strcpy(newString, m_string, m_count);
        delete[] m_string;
        m_string = newString;
        m_string[m_count] = '\0';
    }
}

String::size_type String::capacity() const
{
    return m_capacity;
}

void String::shrink_to_fit()
{
    if (m_capacity > 15)
    {
        if (m_count < m_capacity)
        {
            if (m_count <= 15)
                m_capacity = 15;
            else
            if (m_count > 15)
                m_capacity = m_count;

            pointer newString{ new value_type[m_capacity + 1] };

            m_strcpy(newString, m_string, m_count);
            delete[] m_string;
            m_string = newString;
            m_string[m_count] = '\0';
        }
    }
}

void String::clear()
{
    m_string[0] = '\0';
    m_count = 0;
}

String& String::insert(size_type index, size_type count, value_type chr)
{
    if (index > m_count)
        throw std::out_of_range{ "out of range" };
    else
    if (index <= m_count)
    {
        size_type totalCount{ m_count + count };

        m_reserveOuter(totalCount, index, count);

        m_chrset((m_string + index), chr, count);
        m_string[totalCount] = '\0';
        m_count = totalCount;
    }

    return *this;
}

String& String::insert(size_type index, const_pointer string)
{
    return insert(index, string, m_strlen(string));
}

String& String::insert(size_type index, const_pointer string, size_type count)
{
    if (index > m_count)
        throw std::out_of_range{ "out of range" };
    else
    if (index <= m_count)
    {
        size_type totalCount{ m_count + count };

        m_reserveOuter(totalCount, index, count);

        m_strcpy((m_string + index), string, count);
        m_string[totalCount] = '\0';
        m_count = totalCount;
    }

    return *this;
}

String& String::insert(size_type index, const String& other)
{
    return insert(index, other, 0, npos);
}

String& String::insert(size_type index, const String& other, size_type otherIndex, size_type count)
{
    if (index > m_count)
        throw std::out_of_range{ "out of range" };
    else
    if (index <= m_count)
    {
        if (otherIndex > other.m_count)
            throw std::out_of_range{ "out of range" };
        else
        if (otherIndex < other.m_count)
        {
            count = m_min(count, other.m_count - otherIndex);

            size_type totalCount{ m_count + count };

            m_reserveOuter(totalCount, index, count);

            m_strcpy((m_string + index), (other.m_string + otherIndex), count);
            m_string[totalCount] = '\0';
            m_count = totalCount;
        }
    }

    return *this;
}

String::iterator String::insert(const_iterator iter, value_type chr)
{
    return insert(iter, 1, chr);
}

String::iterator String::insert(const_iterator iter, size_type count, value_type chr)
{
    size_type index{ static_cast<size_type>(iter - cbegin()) };

    if (count == 0)
        return begin() + index;

    size_type totalCount{ m_count + count };

    m_reserveOuter(totalCount, index, count);

    m_chrset((m_string + index), chr, count);
    m_string[totalCount] = '\0';
    m_count = totalCount;

    return begin() + index;
}

String::iterator String::insert(const_iterator iter, std::initializer_list<value_type> list)
{
    size_type index{ static_cast<size_type>(iter - cbegin()) };

    if (list.size() == 0)
        return begin() + index;

    size_type count{ list.size() };
    size_type totalCount{ m_count + count };

    m_reserveOuter(totalCount, index, count);

    m_strcpy((m_string + index), list.begin(), count);
    m_string[totalCount] = '\0';
    m_count = totalCount;

    return begin() + index;
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

void String::m_reserveEmpty(size_type newCapacity)
{
    if (newCapacity > m_capacity)
    {
        if (newCapacity <= m_capacity * 2)
            m_capacity *= 2;
        else
        if (newCapacity > m_capacity * 2)
            m_capacity = newCapacity;

        pointer newString{ new value_type[m_capacity + 1] };

        delete[] m_string;
        m_string = newString;
    }
}

void String::m_reserveOuter(size_type newCapacity, size_type index, size_type count)
{
    if (newCapacity <= m_capacity)
        m_strcpy((m_string + index + count), (m_string + index), (m_count - index));
    if (newCapacity > m_capacity)
    {
        if (newCapacity <= m_capacity * 2)
            m_capacity *= 2;
        else
        if (newCapacity > m_capacity * 2)
            m_capacity = newCapacity;

        pointer newString{ new value_type[m_capacity + 1] };

        m_strcpy(newString, m_string, index);
        m_strcpy((newString + index + count), (m_string + index), (m_count - index));
        delete[] m_string;
        m_string = newString;
    }
}

std::ostream& operator<<(std::ostream& ostream, const String& string)
{
    for (String::size_type i{ 0 }; i < string.m_count; ++i)
        ostream << string.m_string[i];

    return ostream;
}