#include "html_parse_impl.hpp"

using namespace std;

namespace html
{
    html_tag html_tag::parse(string_view buffer)
    {
        char_array_view arr(buffer.data(), buffer.size());
        return parse_tag(arr);
    }

    html_node html_node::parse(string_view buffer)
    {
        char_array_view arr(buffer.data(), buffer.size());
        return parse_node(arr);
    }

    html_decl html_decl::parse(string_view buffer)
    {
        char_array_view arr(buffer.data(), buffer.size());
        return parse_decl(arr);
    }

    html_doc html_doc::parse(string_view buffer)
    {
        char_array_view arr(buffer.data(), buffer.size());
        return parse_doc(arr);
    }
} // namespace html
