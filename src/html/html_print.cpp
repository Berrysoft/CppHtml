#include <html/html_doc.hpp>
#include <map>
#include <string_view>

using namespace std;

namespace html
{
    const map<char, string> ascii_ent = { { '\"', "quot" }, { '\'', "apos" }, { '&', "amp" }, { '<', "lt" }, { '>', "gt" } };
    constexpr string_view ascii_ent_fs = "\"\'&<>";
    const map<char, string> ascii_ent_nq = { { '&', "amp" }, { '<', "lt" }, { '>', "gt" } };
    constexpr string_view ascii_ent_nq_fs = "&<>";

    ostream& print_ent(ostream& stream, const string& str, bool noquotes)
    {
        const map<char, string>& m = noquotes ? ascii_ent_nq : ascii_ent;
        string_view fs = noquotes ? ascii_ent_nq_fs : ascii_ent_fs;
        size_t off = 0;
        size_t pos;
        while ((pos = str.find_first_of(fs, off)) != string::npos)
        {
            stream.write(str.data() + off, pos - off);
            stream << '&' << m.at(str[pos]) << ';';
            off = pos + 1;
        }
        stream.write(str.data() + off, str.size() - off);
        return stream;
    }

    ostream& html_decl::print(ostream& stream) const
    {
        return stream << "<!doctype " << m_type << ">";
    }

    ostream& html_tag::print_attrs(ostream& stream) const
    {
        for (auto pair : m_attrs)
        {
            stream << ' ' << pair.first;
            if (pair.first != pair.second)
            {
                if (pair.second.find('\"') != string::npos)
                {
                    if (pair.second.find('\'') != string::npos)
                    {
                        stream << "=\"";
                        print_ent(stream, pair.second, false) << "\"";
                    }
                    else
                    {
                        stream << "=\'";
                        print_ent(stream, pair.second, true) << "\'";
                    }
                }
                else
                {
                    stream << "=\"";
                    print_ent(stream, pair.second, true) << "\"";
                }
            }
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
            stream << in;
            print_ent(stream, get<text_type>(m_data), false);
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
