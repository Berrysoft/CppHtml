#include "char_istream_view.hpp"
#include "html_parse_impl.hpp"
#include <istream>

using namespace std;

namespace html
{
    istream& html_decl::scan(istream& stream)
    {
        char_istream_view view(stream);
        html_decl decl = parse_decl(view);
        swap(decl);
        return stream;
    }

    istream& html_tag::scan(istream& stream)
    {
        char_istream_view view(stream);
        html_tag tag = parse_tag(view);
        swap(tag);
        return stream;
    }

    istream& html_node::scan(istream& stream)
    {
        char_istream_view view(stream);
        html_node node = parse_node(view);
        swap(node);
        return stream;
    }

    istream& html_doc::scan(istream& stream)
    {
        char_istream_view view(stream);
        html_doc doc = parse_doc(view);
        swap(doc);
        return stream;
    }
} // namespace html
