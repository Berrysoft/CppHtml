#ifndef HTML_DOC_HPP
#define HTML_DOC_HPP

#include <html/array_view.hpp>
#include <html/html_decl.hpp>
#include <html/html_node.hpp>
#include <sstream>

namespace html
{
    class html_doc
    {
    private:
        html_decl m_decl;
        html_node m_node;

        std::ostream& print(std::ostream& stream) const;

    public:
        html_doc(const html_decl& decl = {}, const html_node& node = {}) : m_decl(std::move(decl)), m_node(std::move(node)) {}

        PROP(decl, html_decl)

        PROP(node, html_node)

        static html_doc parse(impl::array_view<const char> buffer);

        std::string to_string() const
        {
            std::ostringstream stream;
            print(stream);
            return stream.str();
        }

        template <typename Char>
        friend constexpr std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& stream, const html_doc& doc)
        {
            if constexpr (std::is_same_v<Char, char>)
            {
                doc.print(stream);
                return stream;
            }
            else
            {
                stream << doc.to_string().c_str();
            }
        }

        friend inline bool operator==(const html_doc& d1, const html_doc& d2) { return d1.m_decl == d2.m_decl && d1.m_node == d2.m_node; }
        friend inline bool operator!=(const html_doc& d1, const html_doc& d2) { return !(d1 == d2); }
    };
} // namespace html

#endif //!HTML_DOC_HPP
