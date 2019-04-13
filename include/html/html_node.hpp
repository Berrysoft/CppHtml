#ifndef HTML_NODE_HPP
#define HTML_NODE_HPP

#include <html_tag.hpp>
#include <memory>
#include <string>
#include <vector>

namespace html
{
    enum class html_node_type
    {
        node,
        text,
        style,
        script
    };

    class html_node_base
    {
    protected:
        html_tag m_tag;

    public:
        virtual html_node_type type() const noexcept = 0;

        constexpr html_tag& tag() noexcept { return m_tag; }
        constexpr const html_tag& tag() const noexcept { return m_tag; }
    };

    class html_node : public html_node_base
    {
    private:
        using child_type = std::unique_ptr<html_node_base>;
        using child_reference = child_type&;
        using child_const_reference = const child_type&;
        using child_seq_type = std::vector<child_type>;
        using child_iterator = typename child_seq_type::iterator;
        using child_const_iterator = typename child_seq_type::const_iterator;

        child_seq_type m_nodes;

    public:
        html_node_type type() const noexcept override { return html_node_type::node; }

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
        void push_back(child_type&& child) { m_nodes.push_back(std::move(child)); }
        child_iterator insert(child_const_iterator pos, child_type&& child) { return m_nodes.insert(pos, std::move(child)); }
        child_iterator erase(child_const_iterator pos) { return m_nodes.erase(pos); }
        child_iterator erase(child_const_iterator first, child_const_iterator last) { return m_nodes.erase(first, last); }
    };

    class html_text_node : public html_node_base
    {
    protected:
        std::string m_text;

    public:
        html_node_type type() const noexcept override { return html_node_type::text; }

        constexpr std::string& text() noexcept { return m_text; }
        constexpr const std::string& text() const noexcept { return m_text; }
    };

    class html_style_node : public html_text_node
    {
    public:
        html_node_type type() const noexcept override { return html_node_type::style; }
    };

    class html_script_node : public html_text_node
    {
    public:
        html_node_type type() const noexcept override { return html_node_type::script; }
    };
} // namespace html

#endif // !HTML_NODE_HPP
