#ifndef HTML_PARSE_IMPL_HPP
#define HTML_PARSE_IMPL_HPP

#include "char_array_view.hpp"
#include <html/html_doc.hpp>
#include <string_view>

namespace html
{
    bool starts_with(const char_array_view& buffer, std::string_view str);
    bool str_case_eq(std::string_view s1, std::string_view s2);
    void skip_space(char_array_view& buffer);
    html_tag parse_tag(char_array_view& buffer);
    html_node_type parse_node_type(char_array_view& buffer);
    html_node parse_text_node(char_array_view& buffer);
    html_node parse_node(char_array_view& buffer);
    html_decl parse_decl(char_array_view& buffer);
    html_doc parse_doc(char_array_view& buffer);
} // namespace html

#endif // !HTML_PARSE_IMPL_HPP
