#ifndef HTML_TAG_HPP
#define HTML_TAG_HPP

#include <string>
#include <vector>

namespace html
{
    struct html_attribute
    {
        std::string name;
        std::string value;
    };

    class html_tag
    {
    private:
        using attr_type = html_attribute;
        using attr_reference = attr_type&;
        using attr_const_reference = const attr_type&;
        using attr_seq_type = std::vector<attr_type>;
        using attr_iterator = typename attr_seq_type::iterator;
        using attr_const_iterator = typename attr_seq_type::const_iterator;

        std::string m_name;
        attr_seq_type m_attrs;

    public:
        constexpr std::string& name() noexcept { return m_name; }
        constexpr const std::string& name() const noexcept { return m_name; }

        bool empty() const noexcept { return m_attrs.empty(); }
        std::size_t size() const noexcept { return m_attrs.size(); }

        attr_reference front() { return m_attrs.front(); }
        attr_const_reference front() const { return m_attrs.front(); }
        attr_reference back() { return m_attrs.back(); }
        attr_const_reference back() const { return m_attrs.back(); }

        attr_iterator begin() noexcept { return m_attrs.begin(); }
        attr_const_iterator begin() const noexcept { return m_attrs.begin(); }
        attr_const_iterator cbegin() const noexcept { return m_attrs.cbegin(); }
        attr_iterator end() noexcept { return m_attrs.end(); }
        attr_const_iterator end() const noexcept { return m_attrs.end(); }
        attr_const_iterator cend() const noexcept { return m_attrs.cend(); }

        void clear() noexcept { m_attrs.clear(); }
        void push_back(const attr_type& attr) { m_attrs.push_back(attr); }
        void push_back(attr_type&& attr) { m_attrs.push_back(std::move(attr)); }
        attr_iterator insert(attr_const_iterator pos, const attr_type& attr) { return m_attrs.insert(pos, attr); }
        attr_iterator insert(attr_const_iterator pos, attr_type&& attr) { return m_attrs.insert(pos, std::move(attr)); }
        attr_iterator erase(attr_const_iterator pos) { return m_attrs.erase(pos); }
        attr_iterator erase(attr_const_iterator first, attr_const_iterator last) { return m_attrs.erase(first, last); }
    };
} // namespace html

#endif //!HTML_TAG_HPP