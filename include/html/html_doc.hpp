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
        html_doc(const html_doc&) = default;
        html_doc(html_doc&&) noexcept = default;

        html_doc& operator=(const html_doc&) = default;
        html_doc& operator=(html_doc&&) noexcept = default;

        CPPHTML_PROP(decl, html_decl)

        CPPHTML_PROP(node, html_node)

        void swap(html_doc& d) noexcept
        {
            m_decl.swap(d.m_decl);
            m_node.swap(d.m_node);
        }

        CPPHTML_API static html_doc parse(std::string_view buffer);

        friend inline std::ostream& operator<<(std::ostream& stream, const html_doc& doc) { return doc.print(stream); }
        friend inline std::istream& operator>>(std::istream& stream, html_doc& doc) { return doc.scan(stream); }

        friend inline bool operator==(const html_doc& d1, const html_doc& d2) { return d1.m_decl == d2.m_decl && d1.m_node == d2.m_node; }
        friend inline bool operator!=(const html_doc& d1, const html_doc& d2) { return !(d1 == d2); }
    };
} // namespace html

#endif //!HTML_DOC_HPP
