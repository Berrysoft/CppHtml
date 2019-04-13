#ifndef HTML_DOC_HPP
#define HTML_DOC_HPP

#include <html/array_view.hpp>
#include <html/html_decl.hpp>
#include <html/html_node.hpp>

namespace html
{
    class html_doc
    {
    private:
        html_decl m_decl;
        std::shared_ptr<html_node> m_node;

    public:
        constexpr html_decl& decl() noexcept { return m_decl; }
        constexpr const html_decl& decl() const noexcept { return m_decl; }

        constexpr std::shared_ptr<html_node>& node() noexcept { return m_node; }
        constexpr const std::shared_ptr<html_node>& node() const noexcept { return m_node; }

        static html_doc parse(impl::array_view<const char> buffer);
    };
} // namespace html

#endif //!HTML_DOC_HPP
