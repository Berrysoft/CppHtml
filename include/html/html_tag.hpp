#ifndef HTML_TAG_HPP
#define HTML_TAG_HPP

#include <html/array_view.hpp>
#include <html/html_utility.hpp>
#include <map>
#include <string>

namespace html
{
    class html_tag
    {
    private:
        using attr_seq_type = std::map<std::string, std::string>;
        using attr_iterator = typename attr_seq_type::iterator;
        using attr_const_iterator = typename attr_seq_type::const_iterator;

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

        void clear() noexcept { m_attrs.clear(); }
        bool contains(const std::string& key) const { return m_attrs.find(key) != m_attrs.end(); }

        attr_iterator erase(attr_const_iterator pos) { return m_attrs.erase(pos); }
        attr_iterator erase(attr_const_iterator first, attr_const_iterator last) { return m_attrs.erase(first, last); }
        std::size_t erase(const std::string& key) { return m_attrs.erase(key); }

        static html_tag parse(impl::array_view<const char> buffer);
    };
} // namespace html

#endif //!HTML_TAG_HPP