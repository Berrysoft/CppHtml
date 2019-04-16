#include "html_parse_impl.hpp"

using namespace std;

namespace html
{
    // TODO: implement it.

    std::istream& html_decl::scan(std::istream& stream)
    {
        return stream;
    }

    std::istream& html_tag::scan(std::istream& stream)
    {
        return stream;
    }

    std::istream& html_node::scan(std::istream& stream)
    {
        return stream;
    }

    std::istream& html_doc::scan(std::istream& stream)
    {
        return stream;
    }
} // namespace html
