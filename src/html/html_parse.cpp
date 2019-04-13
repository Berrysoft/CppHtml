#include <html/html_doc.hpp>
#include <memory>

using namespace std;

namespace html
{
    namespace impl
    {
        html_tag parse_tag(array_view<const char>& buffer)
        {
        }

        html_node parse_node(array_view<const char>& buffer)
        {
        }

        html_text_node parse_text_node(array_view<const char>& buffer)
        {
        }

        unique_ptr<html_node_base> parse_node_base(array_view<const char>& buffer)
        {
        }

        html_decl parse_decl(array_view<const char>& buffer)
        {
        }

        html_doc parse_doc(array_view<const char>& buffer)
        {
            html_doc doc;
            doc.decl() = parse_decl(buffer);
            doc.node() = parse_node(buffer);
            return doc;
        }
    } // namespace impl

    html_tag html_tag::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_tag(buffer);
    }

    html_node html_node::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_node(buffer);
    }

    html_text_node html_text_node::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_text_node(buffer);
    }

    unique_ptr<html_node_base> html_node_base::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_node_base(buffer);
    }

    html_decl html_decl::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_decl(buffer);
    }

    html_doc html_doc::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_doc(buffer);
    }
} // namespace html
