#include <html_parse_impl.hpp>
#include <string_view>

using namespace std;

namespace html
{
    html_tag html_tag::parse(string_view buffer)
    {
        return parse_tag(buffer);
    }

    html_node html_node::parse(string_view buffer)
    {
        return parse_node(buffer);
    }

    html_decl html_decl::parse(string_view buffer)
    {
        return parse_decl(buffer);
    }

    html_doc html_doc::parse(string_view buffer)
    {
        return parse_doc(buffer);
    }
} // namespace html
