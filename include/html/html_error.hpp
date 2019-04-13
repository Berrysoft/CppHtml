#ifndef HTML_ERROR_HPP
#define HTML_ERROR_HPP

#include <stdexcept>

namespace html
{
    class html_error : public std::logic_error
    {
        using std::logic_error::logic_error;
    };
} // namespace html

#endif // !HTML_ERROR_HPP
