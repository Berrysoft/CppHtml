#ifndef HTML_NODE_HPP
#define HTML_NODE_HPP

#include <html/array_view.hpp>
#include <html/html_tag.hpp>
#include <memory>
#include <string>
#include <vector>

namespace html
{
    enum class html_node_type
    {
        none,
        node,
        text
    };

    class html_node_base
    {
    public:
        virtual ~html_node_base() {}
        virtual html_node_type type() const noexcept = 0;

        static std::shared_ptr<html_node_base> parse(impl::array_view<const char> buffer);
    };

    class html_node : public html_node_base
    {
    private:
        using child_type = std::shared_ptr<html_node_base>;
        using child_reference = child_type&;
        using child_const_reference = const child_type&;
        using child_seq_type = std::vector<child_type>;
        using child_iterator = typename child_seq_type::iterator;
        using child_const_iterator = typename child_seq_type::const_iterator;

        html_tag m_tag;
        child_seq_type m_nodes;

    public:
        ~html_node() override {}
        html_node_type type() const noexcept override { return html_node_type::node; }

        constexpr html_tag& tag() noexcept { return m_tag; }
        constexpr const html_tag& tag() const noexcept { return m_tag; }

        bool empty() const noexcept { return m_nodes.empty(); }
        std::size_t size() const noexcept { return m_nodes.size(); }

        child_reference front() { return m_nodes.front(); }
        child_const_reference front() const { return m_nodes.front(); }
        child_reference back() { return m_nodes.back(); }
        child_const_reference back() const { return m_nodes.back(); }

        child_iterator begin() noexcept { return m_nodes.begin(); }
        child_const_iterator begin() const noexcept { return m_nodes.begin(); }
        child_const_iterator cbegin() const noexcept { return m_nodes.cbegin(); }
        child_iterator end() noexcept { return m_nodes.end(); }
        child_const_iterator end() const noexcept { return m_nodes.end(); }
        child_const_iterator cend() const noexcept { return m_nodes.cend(); }

        void clear() noexcept { m_nodes.clear(); }
        void push_back(const child_type& child) { m_nodes.push_back(child); }
        void push_back(child_type&& child) { m_nodes.push_back(std::move(child)); }
        child_iterator insert(child_const_iterator pos, const child_type& child) { return m_nodes.insert(pos, child); }
        child_iterator insert(child_const_iterator pos, child_type&& child) { return m_nodes.insert(pos, std::move(child)); }
        child_iterator erase(child_const_iterator pos) { return m_nodes.erase(pos); }
        child_iterator erase(child_const_iterator first, child_const_iterator last) { return m_nodes.erase(first, last); }

        static std::shared_ptr<html_node> parse(impl::array_view<const char> buffer);
    };

    class html_text_node : public html_node_base
    {
    protected:
        std::string m_text;

    public:
        ~html_text_node() override {}
        html_node_type type() const noexcept override { return html_node_type::text; }

        constexpr std::string& text() noexcept { return m_text; }
        constexpr const std::string& text() const noexcept { return m_text; }

        static std::shared_ptr<html_text_node> parse(impl::array_view<const char> buffer);
    };
} // namespace html

#endif // !HTML_NODE_HPP
