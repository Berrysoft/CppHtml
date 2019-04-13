#ifndef HTML_DECL_HPP
#define HTML_DECL_HPP

#include <html/array_view.hpp>
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

        static html_decl parse(impl::array_view<const char> buffer);
    };
} // namespace html

#endif //!HTML_DECL_HPP
