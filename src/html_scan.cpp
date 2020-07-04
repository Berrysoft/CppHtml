#include <html_parse_impl.hpp>
#include <istream>
#include <iterator>
#include <string>

using namespace std;

namespace html
{
    istream& html_decl::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        html_decl decl = parse_decl(view);
        swap(decl);
        return stream;
    }

    istream& html_tag::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        html_tag tag = parse_tag(view);
        swap(tag);
        return stream;
    }

    istream& html_node::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        html_node node = parse_node(view);
        swap(node);
        return stream;
    }

    istream& html_doc::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        html_doc doc = parse_doc(view);
        swap(doc);
        return stream;
    }
} // namespace html
