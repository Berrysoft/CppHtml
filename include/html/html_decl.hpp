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

        friend class html_doc;

    public:
        html_decl(const std::string& type = {}) : m_type(type) {}

        PROP(type, std::string)

        CPPHTML_API static html_decl parse(std::string_view buffer);

        std::string to_string() const
        {
            std::ostringstream stream;
            print(stream);
            return stream.str();
        }

        template <typename Char>
        friend constexpr std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& stream, const html_decl& decl)
        {
            if constexpr (std::is_same_v<Char, char>)
            {
                decl.print(stream);
                return stream;
            }
            else
            {
                stream << decl.to_string().c_str();
            }
        }

        friend inline bool operator==(const html_decl& d1, const html_decl& d2) { return d1.m_type == d2.m_type; }
        friend inline bool operator!=(const html_decl& d1, const html_decl& d2) { return !(d1 == d2); }
    };
} // namespace html

#endif //!HTML_DECL_HPP
