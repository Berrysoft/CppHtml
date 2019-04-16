#ifndef HTML_PARSE_IMPL_HPP
#define HTML_PARSE_IMPL_HPP

#include <html/html_doc.hpp>
#include <string_view>
#include <vector>

namespace html
{
    inline bool char_case_eq(char a, char b) { return std::tolower(a) == std::tolower(b); }

    template <typename View>
    inline bool starts_with(View& buffer, std::string_view str)
    {
        if (str.size() > buffer.size() && !buffer.enlarge(str.size() - buffer.size())) return false;
        return std::equal(buffer.begin(), buffer.begin() + str.size(), str.begin(), str.end(), char_case_eq);
    }

    template <typename View>
    inline bool starts_with(View& buffer, std::size_t off, std::string_view str)
    {
        if ((str.size() + off > buffer.size()) && !buffer.enlarge(str.size() + off - buffer.size())) return false;
        return std::equal(buffer.begin() + off, buffer.begin() + str.size() + off, str.begin(), str.end(), char_case_eq);
    }

    inline bool str_case_eq(std::string_view s1, std::string_view s2)
    {
        return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), char_case_eq);
    }

    template <typename View>
    inline void skip_space(View& buffer)
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
        if (buffer.empty()) buffer.enlarge(1);
    }

    template <typename View>
    inline html_tag parse_tag(View& buffer)
    {
        skip_space(buffer);
        if (buffer.front() != '<') return {};
        ++buffer;
        std::size_t pos = buffer.find({ ' ', '/', '>' });
        if (pos == View::npos)
        {
            html_tag tag(std::string(buffer.begin(), buffer.end()));
            buffer += View::npos;
            return tag;
        }
        html_tag tag(std::string(buffer.data(), pos));
        buffer += pos;
        skip_space(buffer);
        while (!buffer.empty() && buffer.front() != '/' && buffer.front() != '>')
        {
            pos = buffer.find('=');
            if (pos == View::npos)
            {
                pos = buffer.find({ ' ', '/', '>' });
                std::string key(buffer.data(), pos);
                tag[key] = key;
                buffer += pos;
                skip_space(buffer);
                continue;
            }
            std::string key(buffer.data(), pos);
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
            if (pos == View::npos)
            {
                tag[key] = std::string(buffer.begin(), buffer.end());
                buffer += View::npos;
            }
            else
            {
                tag[key] = std::string(buffer.data(), pos);
                buffer += pos + 1;
            }
            skip_space(buffer);
        }
        return tag;
    }

    template <typename View>
    inline html_node_type parse_node_type(View& buffer)
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

    template <typename View>
    inline html_node parse_text_node(View& buffer)
    {
        skip_space(buffer);
        html_node node;
        node.type(html_node_type::text);
        std::size_t pos = buffer.find('<');
        if (pos == View::npos)
        {
            node.text(std::string(buffer.begin(), buffer.end()));
            buffer += pos;
        }
        else
        {
            node.text(std::string(buffer.data(), pos));
            buffer += pos;
        }
        return node;
    }

    template <typename View>
    inline html_node parse_node(View& buffer)
    {
        html_node root;
        root.type(html_node_type::node);
        std::vector<html_node*> p = { &root };
        std::vector<html_node*> np;
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
                std::size_t pos = buffer.find('/');
                pos++;
                std::size_t pos2 = buffer.find({ ' ', '>' }, pos);
                std::string_view cname(buffer.data() + pos, pos2 - pos);
                pos = buffer.find('>', pos);
                pos++;
                auto it = find_if(p.rbegin(), p.rend(), [cname](html_node* pn) { return str_case_eq(pn->tag().name(), cname); });
                if (it == p.rend())
                {
                    buffer += pos;
                }
                else
                {
                    if (it == p.rbegin())
                    {
                        buffer += pos;
                    }
                    else
                    {
                        if (p.size() > 1)
                        {
                            html_node* parent = p[p.size() - 2];
                            std::vector<html_node> children;
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
                                parent->push_back(std::move(node));
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
                        std::size_t pos = 0;
                        while (true)
                        {
                            pos = buffer.find('<', pos);
                            if (pos == View::npos) break;
                            if (starts_with(buffer, pos, "</"))
                            {
                                auto pos2 = pos + 2;
                                auto pos3 = buffer.find({ ' ', '>' }, pos2);
                                if (str_case_eq(newn.tag().name(), std::string_view(buffer.data() + pos2, pos3 - pos2)))
                                    break;
                            }
                            pos++;
                        }
                        if (pos > 0)
                            newn.push_back({ std::string(buffer.data(), pos) });
                        buffer += pos;
                        pos = buffer.find('>');
                        buffer += pos + 1;
                        current->push_back(std::move(newn));
                    }
                    else
                    {
                        current->push_back(std::move(newn));
                        p.push_back(&current->back());
                    }
                }
                else
                {
                    std::size_t pos = buffer.find('>');
                    buffer += pos + 1;
                    current->push_back(std::move(newn));
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
        return root.empty() ? html_node() : root.front();
    }

    template <typename View>
    inline html_decl parse_decl(View& buffer)
    {
        skip_space(buffer);
        if (!starts_with(buffer, "<!doctype")) return {};
        buffer += 9;
        skip_space(buffer);
        html_decl decl;
        std::size_t pos = buffer.find('>');
        if (pos == View::npos)
        {
            decl.type(std::string(buffer.begin(), buffer.end()));
            buffer += View::npos;
        }
        else
        {
            decl.type(std::string(buffer.data(), pos));
            buffer += pos + 1;
        }
        return decl;
    }

    template <typename View>
    inline html_doc parse_doc(View& buffer)
    {
        html_doc doc;
        doc.decl(parse_decl(buffer));
        doc.node(parse_node(buffer));
        return doc;
    }
} // namespace html

#endif // !HTML_PARSE_IMPL_HPP
