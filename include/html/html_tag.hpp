#ifndef HTML_TAG_HPP
#define HTML_TAG_HPP

#include <algorithm>
#include <html/html_utility.hpp>
#include <map>
#include <string>
#include <string_view>

namespace html
{
    class html_tag
    {
    private:
        using attr_seq_type = std::map<std::string, std::string>;
        using attr_iterator = typename attr_seq_type::iterator;
        using attr_const_iterator = typename attr_seq_type::const_iterator;
        using attr_reverse_iterator = typename attr_seq_type::reverse_iterator;
        using attr_const_reverse_iterator = typename attr_seq_type::const_reverse_iterator;

        std::string m_name;
        attr_seq_type m_attrs;

        std::ostream& print_attrs(std::ostream& stream) const;
        std::ostream& print_open(std::ostream& stream) const;
        std::ostream& print_close(std::ostream& stream) const;
        std::ostream& print_single(std::ostream& stream) const;

        friend class html_node;

    public:
        html_tag(const std::string& name = {}) : m_name(name) {}
        html_tag(const std::string& name, const attr_seq_type& attrs) : m_name(name), m_attrs(std::move(attrs)) {}

        PROP(name, std::string)

        std::string& operator[](const std::string& key) { return m_attrs[key]; }
        std::string& operator[](std::string&& key) { return m_attrs[std::move(key)]; }
        std::string& at(const std::string& key) { return m_attrs.at(key); }
        const std::string& at(const std::string& key) const { return m_attrs.at(key); }

        bool empty() const noexcept { return m_attrs.empty(); }
        std::size_t size() const noexcept { return m_attrs.size(); }

        attr_iterator begin() noexcept { return m_attrs.begin(); }
        attr_const_iterator begin() const noexcept { return m_attrs.begin(); }
        attr_const_iterator cbegin() const noexcept { return m_attrs.cbegin(); }
        attr_iterator end() noexcept { return m_attrs.end(); }
        attr_const_iterator end() const noexcept { return m_attrs.end(); }
        attr_const_iterator cend() const noexcept { return m_attrs.cend(); }

        attr_reverse_iterator rbegin() noexcept { return m_attrs.rbegin(); }
        attr_const_reverse_iterator rbegin() const noexcept { return m_attrs.rbegin(); }
        attr_const_reverse_iterator crbegin() const noexcept { return m_attrs.crbegin(); }
        attr_reverse_iterator rend() noexcept { return m_attrs.rend(); }
        attr_const_reverse_iterator rend() const noexcept { return m_attrs.rend(); }
        attr_const_reverse_iterator crend() const noexcept { return m_attrs.crend(); }

        void clear() noexcept { m_attrs.clear(); }
        bool contains(const std::string& key) const { return m_attrs.find(key) != m_attrs.end(); }

        attr_iterator erase(attr_const_iterator pos) { return m_attrs.erase(pos); }
        attr_iterator erase(attr_const_iterator first, attr_const_iterator last) { return m_attrs.erase(first, last); }
        std::size_t erase(const std::string& key) { return m_attrs.erase(key); }

        static html_tag parse(std::string_view buffer);

        friend inline bool operator==(const html_tag& t1, const html_tag& t2) { return t1.m_name == t2.m_name && std::equal(t1.m_attrs.begin(), t1.m_attrs.end(), t2.m_attrs.begin(), t2.m_attrs.end()); }
        friend inline bool operator!=(const html_tag& t1, const html_tag& t2) { return !(t1 == t2); }
    };
} // namespace html

#endif //!HTML_TAG_HPP