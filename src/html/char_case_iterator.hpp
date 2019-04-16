#ifndef CHAR_CASE_ITERATOR_HPP
#define CHAR_CASE_ITERATOR_HPP

#include <cctype>
#include <iterator>

namespace html
{
    template <typename It>
    class char_case_iterator
    {
    private:
        It pc;
        char c;

        void setc() { c = (char)std::tolower(*pc); }

    public:
        using value_type = char;
        using reference = const char&;
        using pointer = const char*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        char_case_iterator() : pc(), c() {}
        char_case_iterator(It p) : pc(p), c((char)std::tolower(*pc)) {}
        char_case_iterator(const char_case_iterator& it) : pc(it.pc), c(it.c) {}
        char_case_iterator& operator=(const char_case_iterator& it)
        {
            pc = it.pc;
            c = it.c;
            return *this;
        }

        reference operator*() const noexcept { return c; }
        pointer operator->() const noexcept { return &c; }
        value_type operator[](difference_type n) { return std::tolower(pc[n]); }

        char_case_iterator& operator++()
        {
            pc++;
            setc();
            return *this;
        }
        char_case_iterator operator++(int)
        {
            char_case_iterator result = *this;
            operator++();
            return result;
        }
        char_case_iterator& operator--()
        {
            pc--;
            setc();
            return *this;
        }
        char_case_iterator operator--(int)
        {
            char_case_iterator result = *this;
            operator--();
            return result;
        }

        char_case_iterator& operator+=(difference_type n)
        {
            pc += n;
            setc();
            return *this;
        }
        friend char_case_iterator operator+(const char_case_iterator& it, difference_type n) { return char_case_iterator(it.pc + n); }
        friend char_case_iterator operator+(difference_type n, const char_case_iterator& it) { return it + n; }

        char_case_iterator& operator-=(difference_type n)
        {
            pc -= n;
            setc();
            return *this;
        }
        friend char_case_iterator operator-(const char_case_iterator& it, difference_type n) { return char_case_iterator(it.pc - n); }
        friend char_case_iterator operator-(difference_type n, const char_case_iterator& it) { return it + n; }

        friend difference_type operator-(const char_case_iterator& it1, const char_case_iterator& it2) { return it1.pc - it2.pc; }

        friend bool operator==(const char_case_iterator& it1, const char_case_iterator& it2) { return it1.pc == it2.pc; }
        friend bool operator!=(const char_case_iterator& it1, const char_case_iterator& it2) { return !(it1 == it2); }
        friend bool operator<(const char_case_iterator& it1, const char_case_iterator& it2) { return it1.pc < it2.pc; }
        friend bool operator>(const char_case_iterator& it1, const char_case_iterator& it2) { return it2 < it1; }
        friend bool operator<=(const char_case_iterator& it1, const char_case_iterator& it2) { return !(it2 < it1); }
        friend bool operator>=(const char_case_iterator& it1, const char_case_iterator& it2) { return !(it1 < it2); }
    };
} // namespace html

#endif // !CHAR_CASE_ITERATOR_HPP
