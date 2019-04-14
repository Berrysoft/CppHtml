#include <html/html_doc.hpp>

using namespace std;

namespace html
{
    ostream& html_decl::print(ostream& stream) const
    {
        return stream << "<!doctype " << m_type << ">";
    }

    ostream& html_tag::print_attrs(ostream& stream) const
    {
        for (auto pair : m_attrs)
        {
            stream << ' ' << pair.first << "=\'" << pair.second << '\'';
        }
        return stream;
    }

    ostream& html_tag::print_open(ostream& stream) const
    {
        stream << '<' << m_name;
        print_attrs(stream);
        stream << '>';
        return stream;
    }

    ostream& html_tag::print_close(std::ostream& stream) const
    {
        return stream << "</" << m_name << '>';
    }

    ostream& html_tag::print_single(std::ostream& stream) const
    {
        stream << '<' << m_name;
        print_attrs(stream);
        stream << "/>";
        return stream;
    }

    ostream& html_node::print(ostream& stream, size_t indent) const
    {
        string in(indent * 2, ' ');
        switch (type())
        {
        case html_node_type::node:
        {
            const node_type& data = get<node_type>(m_data);
            stream << in;
            if (!data.children.empty())
            {
                if (data.children.size() == 1 && data.children.front().type() == html_node_type::text)
                {
                    data.tag.print_open(stream);
                    data.children.front().print(stream, 0);
                    data.tag.print_close(stream);
                }
                else
                {
                    data.tag.print_open(stream) << '\n';
                    for (auto& child : data.children)
                    {
                        child.print(stream, indent + 1) << '\n';
                    }
                    stream << in;
                    data.tag.print_close(stream);
                }
            }
            else
            {
                data.tag.print_single(stream);
            }
            break;
        }
        case html_node_type::text:
            stream << in << get<text_type>(m_data);
            break;
        }
        return stream;
    }

    ostream& html_doc::print(ostream& stream) const
    {
        m_decl.print(stream) << '\n';
        m_node.print(stream);
        return stream;
    }
} // namespace html
