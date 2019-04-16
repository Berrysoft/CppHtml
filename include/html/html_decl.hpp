#ifndef HTML_DECL_HPP
#define HTML_DECL_HPP

#include <html/html_utility.hpp>
#include <sstream>
#include <string>
#include <string_view>

namespace html
{
    class html_decl
    {
    private:
        std::string m_type;

        CPPHTML_API std::ostream& print(std::ostream& stream) const;
        CPPHTML_API std::istream& scan(std::istream& stream);

        friend class html_doc;

    public:
        html_decl(const std::string& type = {}) : m_type(type) {}

        PROP(type, std::string)

        void swap(html_decl& d) { std::swap(m_type, d.m_type); }

        CPPHTML_API static html_decl parse(std::string_view buffer);

        std::string to_string() const
        {
            std::ostringstream stream;
            print(stream);
            return stream.str();
        }

        friend inline std::ostream& operator<<(std::ostream& stream, const html_decl& decl) { return decl.print(stream); }
        friend inline std::istream& operator>>(std::istream& stream, html_decl& decl) { return decl.scan(stream); }

        friend inline bool operator==(const html_decl& d1, const html_decl& d2) { return d1.m_type == d2.m_type; }
        friend inline bool operator!=(const html_decl& d1, const html_decl& d2) { return !(d1 == d2); }
    };

    inline void swap(html_decl& d1, html_decl& d2) { d1.swap(d2); }
} // namespace html

#endif //!HTML_DECL_HPP
