#ifndef HTML_NODE_HPP
#define HTML_NODE_HPP

#include <html/html_tag.hpp>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace html
{
    enum class html_node_type : std::size_t
    {
        none,
        node,
        text
    };

    class html_node;

    struct html_node_data
    {
        html_tag tag;
        std::vector<html_node> children;

        html_node_data() = default;
        html_node_data(const html_node_data&) = default;
        html_node_data(html_node_data&&) noexcept = default;

        html_node_data& operator=(const html_node_data&) = default;
        html_node_data& operator=(html_node_data&&) noexcept = default;

        void swap(html_node_data& d2) noexcept
        {
            tag.swap(d2.tag);
            children.swap(d2.children);
        }
    };

    inline bool operator==(const html_node_data& d1, const html_node_data& d2) { return d1.tag == d2.tag && std::equal(d1.children.begin(), d1.children.end(), d2.children.begin(), d2.children.end()); }
    inline bool operator!=(const html_node_data& d1, const html_node_data& d2) { return !(d1 == d2); }

    class html_node_elements_view;
    class html_node_const_elements_view;

    class html_node
    {
    private:
        using child_type = html_node;
        using child_reference = child_type&;
        using child_const_reference = const child_type&;
        using child_seq_type = std::vector<child_type>;
        using child_iterator = typename child_seq_type::iterator;
        using child_const_iterator = typename child_seq_type::const_iterator;
        using child_reverse_iterator = typename child_seq_type::reverse_iterator;
        using child_const_reverse_iterator = typename child_seq_type::const_reverse_iterator;
        using none_type = std::monostate;
        using node_type = html_node_data;
        using text_type = std::string;
        using data_type = std::variant<none_type, node_type, text_type>;

        data_type m_data;

        CPPHTML_API std::ostream& print(std::ostream& stream, std::size_t indent) const;
        std::ostream& print(std::ostream& stream) const { return print(stream, 0); }
        CPPHTML_API std::istream& scan(std::istream& stream);

        friend class html_doc;
        friend class html_node_elements_iterator;
        friend class html_node_const_elements_iterator;

        constexpr node_type& get_node() & { return std::get<node_type>(m_data); }
        constexpr node_type const& get_node() const& { return std::get<node_type>(m_data); }
        constexpr node_type&& get_node() && { return std::get<node_type>(std::move(m_data)); }

    public:
        html_node() : m_data() {}
        html_node(node_type data) : m_data(std::move(data)) {}
        html_node(html_tag tag, std::vector<html_node> children) : m_data(html_node_data{ tag, children }) {}
        html_node(text_type data) : m_data(std::move(data)) {}

        html_node(const html_node&) = default;
        html_node(html_node&&) noexcept = default;

        html_node& operator=(const html_node&) = default;
        html_node& operator=(html_node&&) noexcept = default;

        constexpr html_node_type type() const noexcept { return static_cast<html_node_type>(m_data.index()); }
        void type(html_node_type value)
        {
            if (value != type())
            {
                switch (value)
                {
                case html_node_type::node:
                    m_data = node_type();
                    break;
                case html_node_type::text:
                    m_data = text_type();
                    break;
                default:
                    m_data = none_type();
                    break;
                }
            }
        }

        CPPHTML_PROP_GETSET(tag, html_tag, get_node().tag, get_node().tag)

        CPPHTML_PROP_GETSET(text, text_type, std::get<text_type>(m_data), m_data)

        child_reference operator[](std::size_t index) { return get_node().children[index]; }
        child_const_reference operator[](std::size_t index) const { return get_node().children[index]; }

        bool empty() const { return get_node().children.empty(); }
        std::size_t size() const { return get_node().children.size(); }

        child_reference front() { return get_node().children.front(); }
        child_const_reference front() const { return get_node().children.front(); }
        child_reference back() { return get_node().children.back(); }
        child_const_reference back() const { return get_node().children.back(); }

        child_iterator begin() { return get_node().children.begin(); }
        child_const_iterator begin() const { return get_node().children.begin(); }
        child_const_iterator cbegin() const { return get_node().children.cbegin(); }
        child_iterator end() { return get_node().children.end(); }
        child_const_iterator end() const { return get_node().children.end(); }
        child_const_iterator cend() const { return get_node().children.cend(); }

        child_reverse_iterator rbegin() { return get_node().children.rbegin(); }
        child_const_reverse_iterator rbegin() const { return get_node().children.rbegin(); }
        child_const_reverse_iterator crbegin() const { return get_node().children.crbegin(); }
        child_reverse_iterator rend() { return get_node().children.rend(); }
        child_const_reverse_iterator rend() const { return get_node().children.rend(); }
        child_const_reverse_iterator crend() const { return get_node().children.crend(); }

        void clear() { return get_node().children.clear(); }
        void push_back(const child_type& child) { get_node().children.push_back(child); }
        void push_back(child_type&& child) { get_node().children.push_back(std::move(child)); }
        void pop_back() { get_node().children.pop_back(); }
        child_iterator insert(child_const_iterator pos, const child_type& child) { return get_node().children.insert(pos, child); }
        child_iterator insert(child_const_iterator pos, child_type&& child) { return get_node().children.insert(pos, std::move(child)); }
        child_iterator erase(child_const_iterator pos) { return get_node().children.erase(pos); }
        child_iterator erase(child_const_iterator first, child_const_iterator last) { return get_node().children.erase(first, last); }

        html_node_elements_view operator[](const std::string& name);
        html_node_const_elements_view operator[](const std::string& name) const;

        void swap(html_node& n) noexcept { m_data.swap(n.m_data); }

        CPPHTML_API static html_node parse(std::string_view buffer);

        friend inline std::ostream& operator<<(std::ostream& stream, const html_node& node) { return node.print(stream); }
        friend inline std::istream& operator>>(std::istream& stream, html_node& node) { return node.scan(stream); }

        friend inline bool operator==(const html_node& n1, const html_node& n2) { return n1.m_data == n2.m_data; }
        friend inline bool operator!=(const html_node& n1, const html_node& n2) { return !(n1 == n2); }
    };

    class html_node_const_elements_iterator
    {
    public:
        using value_type = html_node;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        using child_seq_type = typename html_node::child_seq_type;
        using child_const_iterator = typename child_seq_type::const_iterator;

    protected:
        std::string m_name;
        child_const_iterator m_begin, m_end;

    private:
        static inline bool satisfy(child_const_iterator& it, const std::string& name)
        {
            switch (it->type())
            {
            case html_node_type::node:
                return it->tag().name() == name;
            case html_node_type::text:
                return name.empty();
            default:
                return false;
            }
        }

        void move_next()
        {
            while (m_begin != m_end && !satisfy(m_begin, m_name))
                ++m_begin;
        }
        void move_prev()
        {
            while (!satisfy(m_begin, m_name))
                --m_begin;
        }

    public:
        html_node_const_elements_iterator() : m_name(), m_begin(), m_end() {}
        html_node_const_elements_iterator(const std::string& name, child_const_iterator begin, child_const_iterator end) : m_name(name), m_begin(begin), m_end(end) { move_next(); }
        html_node_const_elements_iterator(const html_node_const_elements_iterator& it) : m_name(it.m_name), m_begin(it.m_begin), m_end(it.m_end) {}
        html_node_const_elements_iterator& operator=(const html_node_const_elements_iterator& it)
        {
            m_name = it.m_name;
            m_begin = it.m_begin;
            m_end = it.m_end;
            return *this;
        }

        void swap(html_node_const_elements_iterator& it)
        {
            std::swap(m_name, it.m_name);
            std::swap(m_begin, it.m_begin);
            std::swap(m_end, it.m_end);
        }

        friend inline bool operator==(const html_node_const_elements_iterator& it1, const html_node_const_elements_iterator& it2) { return it1.m_name == it2.m_name && it1.m_begin == it2.m_begin && it1.m_end == it2.m_end; }
        friend inline bool operator!=(const html_node_const_elements_iterator& it1, const html_node_const_elements_iterator& it2) { return !(it1 == it2); }

        reference operator*() { return *m_begin; }
        pointer operator->() { return m_begin.operator->(); }

        html_node_const_elements_iterator& operator++()
        {
            ++m_begin;
            move_next();
            return *this;
        }
        html_node_const_elements_iterator operator++(int)
        {
            html_node_const_elements_iterator it = *this;
            operator++();
            return it;
        }
        html_node_const_elements_iterator& operator--()
        {
            --m_begin;
            move_prev();
            return *this;
        }
        html_node_const_elements_iterator operator--(int)
        {
            html_node_const_elements_iterator it = *this;
            operator--();
            return it;
        }
    };

    inline void swap(html_node_const_elements_iterator& it1, html_node_const_elements_iterator& it2) { it1.swap(it2); }

    class html_node_elements_iterator : public html_node_const_elements_iterator
    {
    public:
        using value_type = html_node;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        using child_seq_type = typename html_node::child_seq_type;
        using child_iterator = typename child_seq_type::iterator;

    public:
        html_node_elements_iterator() : html_node_const_elements_iterator() {}
        html_node_elements_iterator(const std::string& name, child_iterator begin, child_iterator end) : html_node_const_elements_iterator(name, begin, end) {}
        html_node_elements_iterator(const html_node_elements_iterator& it) : html_node_const_elements_iterator(it) {}
        html_node_elements_iterator& operator=(const html_node_elements_iterator& it)
        {
            html_node_const_elements_iterator::operator=(it);
            return *this;
        }

        reference operator*() { return const_cast<reference>(html_node_const_elements_iterator::operator*()); }
        pointer operator->() { return const_cast<pointer>(html_node_const_elements_iterator::operator->()); }

        html_node_elements_iterator& operator++()
        {
            html_node_const_elements_iterator::operator++();
            return *this;
        }
        html_node_elements_iterator operator++(int)
        {
            html_node_elements_iterator it = *this;
            operator++();
            return it;
        }
        html_node_elements_iterator& operator--()
        {
            html_node_const_elements_iterator::operator--();
            return *this;
        }
        html_node_elements_iterator operator--(int)
        {
            html_node_elements_iterator it = *this;
            operator--();
            return it;
        }
    };

    class html_node_const_elements_view
    {
    protected:
        std::string m_name;
        html_node* m_node;

    public:
        html_node_const_elements_view(const std::string& name, const html_node& node) : m_name(name), m_node(const_cast<html_node*>(&node)) {}

        using const_iterator = html_node_const_elements_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using const_reference = typename const_iterator::reference;

        const_iterator begin() const { return const_iterator(m_name, m_node->begin(), m_node->end()); }
        const_iterator cbegin() const { return const_iterator(m_name, m_node->cbegin(), m_node->cend()); }
        const_iterator end() const { return const_iterator(m_name, m_node->end(), m_node->end()); }
        const_iterator cend() const { return const_iterator(m_name, m_node->cend(), m_node->cend()); }

        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

        const_reference front() const { return *begin(); }
        const_reference back() const { return *(rbegin()); }
    };

    class html_node_elements_view : public html_node_const_elements_view
    {
    public:
        html_node_elements_view(const std::string& name, html_node& node) : html_node_const_elements_view(name, node) {}

        using iterator = html_node_elements_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using reference = typename iterator::reference;

        iterator begin() { return iterator(this->m_name, this->m_node->begin(), this->m_node->end()); }
        iterator end() { return iterator(this->m_name, this->m_node->end(), this->m_node->end()); }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }

        reference front() { return *begin(); }
        reference back() { return *(rbegin()); }
    };

    inline html_node_elements_view html_node::operator[](const std::string& name)
    {
        return html_node_elements_view(name, *this);
    }
    inline html_node_const_elements_view html_node::operator[](const std::string& name) const
    {
        return html_node_const_elements_view(name, *this);
    }
} // namespace html

#endif // !HTML_NODE_HPP
