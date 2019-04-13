#ifndef HTML_DECL_HPP
#define HTML_DECL_HPP

#include <string>

namespace html
{
    class html_decl
    {
    private:
        std::string m_type;

    public:
        constexpr std::string& type() noexcept { return m_type; }
        constexpr const std::string& type() const noexcept { return m_type; }
    };
} // namespace html

#endif //!HTML_DECL_HPP
