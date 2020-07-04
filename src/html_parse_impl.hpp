#ifndef HTML_PARSE_IMPL_HPP
#define HTML_PARSE_IMPL_HPP

#include <html/html_doc.hpp>
#include <string_view>

namespace html
{
    html_tag parse_tag(std::string_view& buffer);
    html_node parse_node(std::string_view& buffer);
    html_decl parse_decl(std::string_view& buffer);
    html_doc parse_doc(std::string_view& buffer);
} // namespace html

#endif // !HTML_PARSE_IMPL_HPP
