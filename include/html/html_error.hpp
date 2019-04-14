#ifndef HTML_ERROR_HPP
#define HTML_ERROR_HPP

#include <stdexcept>

namespace html
{
    struct html_error : std::logic_error
    {
        using std::logic_error::logic_error;
        ~html_error() override {}
    };

    struct html_node_type_error : html_error
    {
        html_node_type_error() : html_error("Wrong html_node_type.") {}
        ~html_node_type_error() override {}
    };
} // namespace html

#endif // !HTML_ERROR_HPP
