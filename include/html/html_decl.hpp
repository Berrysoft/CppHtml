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
        html_decl(const html_decl&) = default;
        html_decl(html_decl&&) noexcept = default;

        html_decl& operator=(const html_decl&) = default;
        html_decl& operator=(html_decl&&) noexcept = default;

        CPPHTML_PROP(type, std::string)

        void swap(html_decl& d) noexcept { m_type.swap(d.m_type); }

        CPPHTML_API static html_decl parse(std::string_view buffer);

        friend inline std::ostream& operator<<(std::ostream& stream, const html_decl& decl) { return decl.print(stream); }
        friend inline std::istream& operator>>(std::istream& stream, html_decl& decl) { return decl.scan(stream); }

        friend inline bool operator==(const html_decl& d1, const html_decl& d2) { return d1.m_type == d2.m_type; }
        friend inline bool operator!=(const html_decl& d1, const html_decl& d2) { return !(d1 == d2); }
    };
} // namespace html

#endif //!HTML_DECL_HPP
