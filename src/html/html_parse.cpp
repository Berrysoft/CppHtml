#include <algorithm>
#include <cctype>
#include <html/html_doc.hpp>
#include <optional>
#include <vector>

using namespace std;

namespace html
{
    namespace impl
    {
        bool starts_with(const array_view<const char>& buffer, string_view str)
        {
            if (buffer.size() < str.size()) return false;
            return equal(buffer.begin(), buffer.begin() + str.size(), str.begin(), str.end(), [](char a, char b) { return tolower(a) == tolower(b); });
        }

        void skip_space(array_view<const char>& buffer)
        {
            bool ctn;
            do
            {
                ctn = false;
                while (!buffer.empty() && buffer.front() >= 1 && buffer.front() <= 255 && isspace(buffer.front()))
                {
                    ++buffer;
                    ctn = true;
                }
                if (starts_with(buffer, "<!--"))
                {
                    buffer += 4;
                    while (!starts_with(buffer, "-->"))
                    {
                        ++buffer;
                        ctn = true;
                    }
                    buffer += 3;
                }
            } while (ctn);
        }

        html_tag parse_tag(array_view<const char>& buffer)
        {
            skip_space(buffer);
            if (buffer.front() != '<') throw html_error("Not a tag.");
            ++buffer;
            size_t pos = buffer.find({ ' ', '/', '>' });
            if (pos == array_view<const char>::npos) throw html_error("The tag isn't complete.");
            html_tag tag(string(buffer.data(), pos));
            buffer += pos;
            skip_space(buffer);
            while (buffer.front() != '/' && buffer.front() != '>')
            {
                pos = buffer.find('=');
                if (pos == array_view<const char>::npos) throw html_error("The property isn't complete.");
                string key(buffer.data(), pos);
                buffer += pos + 1;
                skip_space(buffer);
                char qc = buffer.front();
                if (qc != '\'' && qc != '\"') throw html_error("The property value isn't surrounded with quotes.");
                ++buffer;
                pos = buffer.find(qc);
                if (pos == array_view<const char>::npos) throw html_error("The property value isn't surrounded with quotes.");
                tag[key] = string(buffer.data(), pos);
                buffer += pos + 1;
                skip_space(buffer);
            }
            return tag;
        }

        html_node_type parse_node_type(array_view<const char>& buffer)
        {
            skip_space(buffer);
            if (buffer.empty())
                return html_node_type::none;
            else if (buffer.front() == '<')
            {
                if (buffer[1] == '/')
                    return html_node_type::none;
                else
                    return html_node_type::node;
            }
            else
                return html_node_type::text;
        }

        vector<html_node> parse_node(array_view<const char>& buffer, vector<html_node*>& parent);
        vector<html_node> parse_text_node(array_view<const char>& buffer, vector<html_node*>& parent);

        vector<html_node> parse_node_base_impl(array_view<const char>& buffer, vector<html_node*>& parent)
        {
            skip_space(buffer);
            if (buffer.empty()) return {};
            html_node_type type = parse_node_type(buffer);
            switch (type)
            {
            case html_node_type::node:
                return parse_node(buffer, parent);
            case html_node_type::text:
                return parse_text_node(buffer, parent);
            default:
                return {};
            }
        }

        vector<html_node> parse_node(array_view<const char>& buffer, vector<html_node*>& parent)
        {
            skip_space(buffer);
            html_node node;
            parent.push_back(&node);
            node.type(html_node_type::node);
            node.tag(parse_tag(buffer));
            if (buffer.front() == '>')
            {
                ++buffer;
                if (node.tag().name() == "script" || node.tag().name() == "style")
                {
                    std::size_t pos;
                    auto tb = buffer;
                    while (true)
                    {
                        pos = tb.find('<');
                        tb += pos;
                        if (pos == array_view<const char>::npos || starts_with(tb, "</" + node.tag().name() + ">"))
                            break;
                        ++tb;
                    }
                    if (tb.data() - buffer.data() > 0)
                        node.push_back({ string(buffer.data(), tb.data()) });
                    buffer = tb;
                }
                else
                {
                    while (true)
                    {
                        skip_space(buffer);
                        auto child = parse_node_base_impl(buffer, parent);
                        if (child.empty())
                        {
                            std::size_t pos = buffer.find('/');
                            auto tb = buffer + pos + 1;
                            pos = tb.find('>');
                            string_view cname(tb.data(), pos);
                            auto it = find_if(parent.rbegin(), parent.rend(), [cname](html_node* pn) { return pn->tag().name() == cname; });
                            if (it != parent.rend()) break;
                            tb += pos + 1;
                            buffer = tb;
                        }
                        else
                        {
                            parent.pop_back();
                            for (auto& cc : child)
                            {
                                node.push_back(move(cc));
                            }
                        }
                    }
                }
                std::size_t pos = buffer.find('/');
                auto tb = buffer + pos + 1;
                pos = tb.find('>');
                string_view cname(tb.data(), pos);
                if (pos != array_view<const char>::npos && node.tag().name() == cname)
                {
                    tb += pos + 1;
                    buffer = tb;
                    return { node };
                }
                else
                {
                    vector<html_node> result = { node };
                    string_view tn = node.tag().name();
                    if (tn != "p" && tn != "title" & tn != "div")
                    {
                        if (node.type() == html_node_type::node)
                        {
                            for (auto& child : node)
                            {
                                result.push_back(move(child));
                            }
                        }
                        result[0].clear();
                    }
                    return result;
                }
            }
            else
            {
                size_t pos = buffer.find('>');
                if (pos == array_view<const char>::npos) throw html_error("The tag isn't complete.");
                buffer += pos + 1;
                return { node };
            }
        }

        vector<html_node> parse_text_node(array_view<const char>& buffer, vector<html_node*>& parent)
        {
            skip_space(buffer);
            html_node node;
            parent.push_back(&node);
            node.type(html_node_type::text);
            std::size_t pos = buffer.find('<');
            if (pos == array_view<const char>::npos)
            {
                node.text(string(buffer.begin(), buffer.end()));
                buffer += pos;
            }
            else
            {
                node.text(string(buffer.data(), pos));
                buffer += pos;
            }
            return { node };
        }

        html_node parse_node_base(array_view<const char>& buffer)
        {
            vector<html_node*> p;
            auto result = parse_node_base_impl(buffer, p);
            if (result.empty()) throw html_error("No node found.");
            return result[0];
        }

        html_decl parse_decl(array_view<const char>& buffer)
        {
            skip_space(buffer);
            if (!starts_with(buffer, "<!doctype")) throw html_error("No declaration found.");
            buffer += 9;
            skip_space(buffer);
            html_decl decl;
            size_t pos = buffer.find('>');
            if (pos == array_view<const char>::npos) throw html_error("Declaration not complete.");
            decl.type(string(buffer.data(), pos));
            buffer += pos + 1;
            return decl;
        }

        html_doc parse_doc(array_view<const char>& buffer)
        {
            html_doc doc;
            doc.decl(parse_decl(buffer));
            doc.node(parse_node_base(buffer));
            return doc;
        }
    } // namespace impl

    html_tag html_tag::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_tag(buffer);
    }

    html_node html_node::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_node_base(buffer);
    }

    html_decl html_decl::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_decl(buffer);
    }

    html_doc html_doc::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_doc(buffer);
    }
} // namespace html
