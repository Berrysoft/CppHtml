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
    };
} // namespace html

#endif //!HTML_DOC_HPP
