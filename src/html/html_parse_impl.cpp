#include "html_parse_impl.hpp"
#include <algorithm>
#include <cctype>
#include <optional>
#include <vector>

using namespace std;

namespace html
{
    bool starts_with(const char_array_view& buffer, string_view str)
    {
        if (buffer.size() < str.size()) return false;
        return equal(buffer.begin(), buffer.begin() + str.size(), str.begin(), str.end(), [](char a, char b) { return tolower(a) == tolower(b); });
    }

    bool str_case_eq(string_view s1, string_view s2)
    {
        return equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char a, char b) { return tolower(a) == tolower(b); });
    }

    void skip_space(char_array_view& buffer)
    {
        bool ctn;
        do
        {
            ctn = false;
            while (!buffer.empty() && buffer.front() >= 1 && isspace(buffer.front()))
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

    html_tag parse_tag(char_array_view& buffer)
    {
        skip_space(buffer);
        if (buffer.front() != '<') return {};
        ++buffer;
        size_t pos = buffer.find({ ' ', '/', '>' });
        if (pos == char_array_view::npos)
        {
            html_tag tag(string(buffer.begin(), buffer.end()));
            buffer = {};
            return tag;
        }
        html_tag tag(string(buffer.data(), pos));
        buffer += pos;
        skip_space(buffer);
        while (!buffer.empty() && buffer.front() != '/' && buffer.front() != '>')
        {
            pos = buffer.find('=');
            if (pos == char_array_view::npos)
            {
                pos = buffer.find({ ' ', '/', '>' });
                string key(buffer.data(), pos);
                tag[key] = key;
                buffer += pos;
                skip_space(buffer);
                continue;
            }
            string key(buffer.data(), pos);
            buffer += pos + 1;
            skip_space(buffer);
            char qc = buffer.front();
            if (qc != '\'' && qc != '\"')
            {
                pos = buffer.find({ ' ', '/', '>' });
            }
            else
            {
                ++buffer;
                pos = buffer.find(qc);
            }
            if (pos == char_array_view::npos)
            {
                tag[key] = string(buffer.begin(), buffer.end());
                buffer = {};
            }
            else
            {
                tag[key] = string(buffer.data(), pos);
                buffer += pos + 1;
            }
            skip_space(buffer);
        }
        return tag;
    }

    html_node_type parse_node_type(char_array_view& buffer)
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

    html_node parse_text_node(char_array_view& buffer)
    {
        skip_space(buffer);
        html_node node;
        node.type(html_node_type::text);
        std::size_t pos = buffer.find('<');
        if (pos == char_array_view::npos)
        {
            node.text(string(buffer.begin(), buffer.end()));
            buffer += pos;
        }
        else
        {
            node.text(string(buffer.data(), pos));
            buffer += pos;
        }
        return node;
    }

    html_node parse_node(char_array_view& buffer)
    {
        html_node root;
        root.type(html_node_type::node);
        vector<html_node*> p = { &root };
        vector<html_node*> np;
        while (true)
        {
            if (p.empty()) break;
            html_node* current = p.back();
            html_node_type type = parse_node_type(buffer);
            if (buffer.empty()) break;
            switch (type)
            {
            case html_node_type::none:
            {
                size_t pos = buffer.find('/');
                auto tb = buffer + pos + 1;
                pos = tb.find({ ' ', '>' });
                string_view cname(tb.data(), pos);
                pos = tb.find('>');
                tb += pos + 1;
                auto it = find_if(p.rbegin(), p.rend(), [cname](html_node* pn) { return str_case_eq(pn->tag().name(), cname); });
                if (it == p.rend())
                {
                    buffer = tb;
                }
                else
                {
                    if (it == p.rbegin())
                    {
                        buffer = tb;
                    }
                    else
                    {
                        if (p.size() > 1)
                        {
                            html_node* parent = p[p.size() - 2];
                            vector<html_node> children;
                            if (!current->empty() && current->front().type() == html_node_type::text)
                            {
                                children.assign(current->begin() + 1, current->end());
                                current->erase(current->begin() + 1, current->end());
                            }
                            else
                            {
                                children.assign(current->begin(), current->end());
                                current->clear();
                            }
                            for (html_node& node : children)
                            {
                                parent->push_back(move(node));
                            }
                        }
                    }
                    p.pop_back();
                }
                break;
            }
            case html_node_type::node:
            {
                html_node newn(parse_tag(buffer), {});
                if (buffer.front() == '>')
                {
                    ++buffer;
                    if (str_case_eq(newn.tag().name(), "script") || str_case_eq(newn.tag().name(), "style"))
                    {
                        std::size_t pos;
                        auto tb = buffer;
                        while (true)
                        {
                            pos = tb.find('<');
                            tb += pos;
                            if (pos == char_array_view::npos) break;
                            if (starts_with(tb, "</"))
                            {
                                auto ttb = tb + 2;
                                skip_space(ttb);
                                pos = ttb.find({ ' ', '>' });
                                if (str_case_eq(newn.tag().name(), string_view(ttb.data(), pos)))
                                    break;
                            }
                            ++tb;
                        }
                        if (tb.data() - buffer.data() > 0)
                            newn.push_back({ string(buffer.data(), tb.data()) });
                        buffer = tb;
                        pos = buffer.find('>');
                        buffer += pos + 1;
                    }
                    current->push_back(move(newn));
                    p.push_back(&current->back());
                }
                else
                {
                    size_t pos = buffer.find('>');
                    buffer += pos + 1;
                    current->push_back(move(newn));
                }
                break;
            }
            case html_node_type::text:
            {
                current->push_back(parse_text_node(buffer));
                break;
            }
            }
        }
        return root.front();
    }

    html_decl parse_decl(char_array_view& buffer)
    {
        skip_space(buffer);
        if (!starts_with(buffer, "<!doctype")) return {};
        buffer += 9;
        skip_space(buffer);
        html_decl decl;
        size_t pos = buffer.find('>');
        if (pos == char_array_view::npos)
        {
            decl.type(string(buffer.begin(), buffer.end()));
            buffer = {};
        }
        else
        {
            decl.type(string(buffer.data(), pos));
            buffer += pos + 1;
        }
        return decl;
    }

    html_doc parse_doc(char_array_view& buffer)
    {
        html_doc doc;
        doc.decl(parse_decl(buffer));
        doc.node(parse_node(buffer));
        return doc;
    }
} // namespace html
