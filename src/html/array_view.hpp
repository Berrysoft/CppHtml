#ifndef ARRAY_VIEW_HPP
#define ARRAY_VIEW_HPP

#include <algorithm>
#include <iterator>

namespace html
{
    template <typename T>
    class array_view
    {
    private:
        T* m_data;
        std::size_t m_size;

    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = T*;
        using const_pointer = const T*;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static constexpr std::size_t npos = static_cast<std::size_t>(-1);

        constexpr array_view() noexcept : array_view(nullptr, 0) {}
        constexpr array_view(T* data, std::size_t size) noexcept : m_data(data), m_size(size) {}

        constexpr bool empty() const noexcept { return !m_size; }
        constexpr std::size_t size() const noexcept { return m_size; }

        constexpr pointer data() noexcept { return m_data; }
        constexpr const_pointer data() const noexcept { return m_data; }
        constexpr reference operator[](std::size_t index) { return m_data[index]; }
        constexpr const_reference operator[](std::size_t index) const { return m_data[index]; }

        constexpr reference front() { return *m_data; }
        constexpr const_reference front() const { return *m_data; }
        constexpr reference back() { return m_data[m_size - 1]; }
        constexpr const_reference back() const { return m_data[m_size - 1]; }

        constexpr iterator begin() noexcept { return m_data; }
        constexpr const_iterator begin() const noexcept { return m_data; }
        constexpr const_iterator cbegin() const noexcept { return m_data; }
        constexpr iterator end() noexcept { return m_data + m_size; }
        constexpr const_iterator end() const noexcept { return m_data + m_size; }
        constexpr const_iterator cend() const noexcept { return m_data + m_size; }

        constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return reverse_iterator(cend()); }
        constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return reverse_iterator(cbegin()); }

        constexpr std::size_t find(const T& value) const noexcept
        {
            for (std::size_t i = 0; i < m_size; i++)
            {
                if (m_data[i] == value) return i;
            }
            return npos;
        }
        constexpr std::size_t find(std::initializer_list<T> values) const noexcept
        {
            for (std::size_t i = 0; i < m_size; i++)
            {
                for (const T value : values)
                {
                    if (m_data[i] == value) return i;
                }
            }
            return npos;
        }

        constexpr array_view& operator+=(std::size_t n)
        {
            n = std::min(n, m_size);
            m_size -= n;
            m_data += n;
            return *this;
        }
        constexpr array_view& operator++()
        {
            if (m_size > 0)
            {
                m_data++;
                m_size--;
            }
            return *this;
        }
        constexpr array_view operator++(int)
        {
            array_view result = *this;
            operator++();
            return result;
        }

        friend constexpr array_view operator+(array_view b, std::size_t n)
        {
            n = std::min(n, b.m_size);
            return array_view{ b.m_data + n, b.m_size - n };
        }
        friend constexpr array_view operator+(std::size_t n, array_view b) { return b + n; }
    };
} // namespace html

#endif //!ARRAY_VIEW_HPP
