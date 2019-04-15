#ifndef HTML_NODE_HPP
#define HTML_NODE_HPP

#include <html/array_view.hpp>
#include <html/html_error.hpp>
#include <html/html_tag.hpp>
#include <sstream>
#include <string>
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
    };

    class html_node
    {
    private:
        using child_type = html_node;
        using child_reference = child_type&;
        using child_const_reference = const child_type&;
        using child_seq_type = std::vector<child_type>;
        using child_iterator = typename child_seq_type::iterator;
        using child_const_iterator = typename child_seq_type::const_iterator;
        using none_type = std::monostate;
        using node_type = html_node_data;
        using text_type = std::string;
        using data_type = std::variant<none_type, node_type, text_type>;

        data_type m_data;

        std::ostream& print(std::ostream& stream, std::size_t indent) const;
        std::ostream& print(std::ostream& stream) const { return print(stream, 0); }

        friend class html_doc;

    public:
        html_node() : m_data() {}
        html_node(node_type data) : m_data(std::move(data)) {}
        html_node(html_tag tag, std::vector<html_node> children) : m_data(html_node_data{ tag, children }) {}
        html_node(text_type data) : m_data(std::move(data)) {}

        constexpr html_node_type type() const noexcept { return static_cast<html_node_type>(m_data.index()); }
        void type(html_node_type value)
        {
            if (value != type())
            {
                switch (value)
                {
                case html_node_type::none:
                    m_data = none_type();
                    break;
                case html_node_type::node:
                    m_data = node_type();
                    break;
                case html_node_type::text:
                    m_data = text_type();
                    break;
                default:
                    throw html_node_type_error();
                }
            }
        }

        PROP_GETSET(tag, html_tag, std::get<node_type>(m_data).tag, std::get<node_type>(m_data).tag)

        PROP_GETSET(text, text_type, std::get<text_type>(m_data), m_data)

        child_reference operator[](std::size_t index) { return std::get<node_type>(m_data).children[index]; }
        child_const_reference operator[](std::size_t index) const { return std::get<node_type>(m_data).children[index]; }

        bool empty() const noexcept { return std::get<node_type>(m_data).children.empty(); }
        std::size_t size() const noexcept { return std::get<node_type>(m_data).children.size(); }

        child_reference front() { return std::get<node_type>(m_data).children.front(); }
        child_const_reference front() const { return std::get<node_type>(m_data).children.front(); }
        child_reference back() { return std::get<node_type>(m_data).children.back(); }
        child_const_reference back() const { return std::get<node_type>(m_data).children.back(); }

        child_iterator begin() noexcept { return std::get<node_type>(m_data).children.begin(); }
        child_const_iterator begin() const noexcept { return std::get<node_type>(m_data).children.begin(); }
        child_const_iterator cbegin() const noexcept { return std::get<node_type>(m_data).children.cbegin(); }
        child_iterator end() noexcept { return std::get<node_type>(m_data).children.end(); }
        child_const_iterator end() const noexcept { return std::get<node_type>(m_data).children.end(); }
        child_const_iterator cend() const noexcept { return std::get<node_type>(m_data).children.cend(); }

        void clear() noexcept { return std::get<node_type>(m_data).children.clear(); }
        void push_back(const child_type& child) { std::get<node_type>(m_data).children.push_back(child); }
        void push_back(child_type&& child) { std::get<node_type>(m_data).children.push_back(std::move(child)); }
        void pop_back() { std::get<node_type>(m_data).children.pop_back(); }
        child_iterator insert(child_const_iterator pos, const child_type& child) { return std::get<node_type>(m_data).children.insert(pos, child); }
        child_iterator insert(child_const_iterator pos, child_type&& child) { return std::get<node_type>(m_data).children.insert(pos, std::move(child)); }
        child_iterator erase(child_const_iterator pos) { return std::get<node_type>(m_data).children.erase(pos); }
        child_iterator erase(child_const_iterator first, child_const_iterator last) { return std::get<node_type>(m_data).children.erase(first, last); }

        static html_node parse(impl::array_view<const char> buffer);

        std::string to_string() const
        {
            std::ostringstream stream;
            print(stream);
            return stream.str();
        }

        template <typename Char>
        friend constexpr std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& stream, const html_node& node)
        {
            if constexpr (std::is_same_v<Char, char>)
            {
                node.print(stream);
                return stream;
            }
            else
            {
                stream << node.to_string().c_str();
            }
        }
    };
} // namespace html

#endif // !HTML_NODE_HPP
