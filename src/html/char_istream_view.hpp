#ifndef CHAR_ISTREAM_VIEW_HPP
#define CHAR_ISTREAM_VIEW_HPP

#include <istream>
#include <vector>

namespace html
{
    class char_istream_view
    {
    public:
        using value_type = char;
        using buffer_type = std::vector<char>;
        using const_reference = typename buffer_type::const_reference;
        using const_pointer = typename buffer_type::const_pointer;
        using const_iterator = typename buffer_type::const_iterator;

    private:
        std::istream& m_stream;
        buffer_type m_buffer;

    public:
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);

        char_istream_view(std::istream& stream) : m_stream(stream), m_buffer()
        {
            if (m_stream)
            {
                m_buffer.push_back((char)m_stream.get());
            }
        }
        char_istream_view(const char_istream_view&) = delete;
        char_istream_view& operator=(const char_istream_view&) = delete;

        bool empty() const { return !m_stream && m_buffer.empty(); }
        bool enlarge(std::size_t s)
        {
            while (m_stream && s--)
            {
                m_buffer.push_back((char)m_stream.get());
            }
            return !(s + 1);
        }

        const_pointer data() const { return m_buffer.data(); }
        const_reference operator[](std::size_t index) const { return m_buffer[index]; }

        const_reference front() const { return m_buffer.front(); }
        const_reference back() const { return m_buffer.back(); }

        const_iterator begin() const { return m_buffer.begin(); }
        const_iterator end() const { return m_buffer.end(); }

        template <typename Pred>
        std::size_t find_if(Pred&& pred, std::size_t off = 0)
        {
            std::size_t i = off;
            if (i < m_buffer.size())
            {
                for (; i < m_buffer.size(); i++)
                {
                    if (pred(m_buffer[i])) return i;
                }
            }
            else
            {
                off -= m_buffer.size();
                while (m_stream && off--)
                    m_buffer.push_back((char)m_stream.get());
            }
            while (m_stream)
            {
                m_buffer.push_back((char)m_stream.get());
                if (pred(m_buffer.back())) return i;
                i++;
            }
            return npos;
        }

        std::size_t find(const char& value, std::size_t off = 0)
        {
            return find_if([&value](char c) { return c == value; }, off);
        }
        std::size_t find(std::initializer_list<char> values, std::size_t off = 0)
        {
            return find_if(
                [values](char c) {
                    for (const char value : values)
                    {
                        if (value == c)
                            return true;
                    }
                    return false;
                },
                off);
        }

        char_istream_view& operator+=(std::size_t n)
        {
            if (n < m_buffer.size())
            {
                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + n);
            }
            else
            {
                n -= m_buffer.size();
                m_buffer.clear();
                if (m_stream.seekg(n, std::ios_base::cur))
                {
                    m_buffer.push_back((char)m_stream.get());
                }
            }
            return *this;
        }
        char_istream_view& operator++()
        {
            if (m_buffer.size() <= 1)
            {
                m_buffer.push_back((char)m_stream.get());
            }
            m_buffer.erase(m_buffer.begin());
            return *this;
        }
    };
} // namespace html

#endif // !CHAR_ISTREAM_VIEW_HPP
