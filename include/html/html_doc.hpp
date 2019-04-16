#ifndef HTML_DOC_HPP
#define HTML_DOC_HPP

#include <html/html_decl.hpp>
#include <html/html_node.hpp>
#include <sstream>
#include <string_view>

namespace html
{
    class html_doc
    {
    private:
        html_decl m_decl;
        html_node m_node;

        CPPHTML_API std::ostream& print(std::ostream& stream) const;
        CPPHTML_API std::istream& scan(std::istream& stream);

    public:
        html_doc(const html_decl& decl = {}, const html_node& node = {}) : m_decl(std::move(decl)), m_node(std::move(node)) {}

        PROP(decl, html_decl)

        PROP(node, html_node)

        void swap(html_doc& d)
        {
            std::swap(m_decl, d.m_decl);
            std::swap(m_node, d.m_node);
        }

        CPPHTML_API static html_doc parse(std::string_view buffer);

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
                return doc.print(stream);
            }
            else
            {
                return stream << doc.to_string().c_str();
            }
        }
        friend inline std::istream& operator>>(std::istream& stream, html_doc& doc) { return doc.scan(stream); }

        friend inline bool operator==(const html_doc& d1, const html_doc& d2) { return d1.m_decl == d2.m_decl && d1.m_node == d2.m_node; }
        friend inline bool operator!=(const html_doc& d1, const html_doc& d2) { return !(d1 == d2); }
    };

    inline void swap(html_doc& d1, html_doc& d2) { d1.swap(d2); }
} // namespace html

#endif //!HTML_DOC_HPP
