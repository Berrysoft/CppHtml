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
        *this = parse_decl(view);
        return stream;
    }

    istream& html_tag::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        *this = parse_tag(view);
        return stream;
    }

    istream& html_node::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        *this = parse_node(view);
        return stream;
    }

    istream& html_doc::scan(istream& stream)
    {
        string buffer(istreambuf_iterator<char>{ stream }, istreambuf_iterator<char>{});
        string_view view = buffer;
        *this = parse_doc(view);
        return stream;
    }
} // namespace html
