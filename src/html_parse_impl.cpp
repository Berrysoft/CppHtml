#include <algorithm>
#include <cctype>
#include <html_parse_impl.hpp>
#include <vector>

using namespace std;

namespace html
{
    inline bool char_case_eq(char a, char b) noexcept { return std::tolower(a) == std::tolower(b); }

    inline bool starts_with(std::string_view buffer, std::string_view str)
    {
        if (buffer.size() < str.size()) return false;
        return std::equal(buffer.begin(), buffer.begin() + str.size(), str.begin(), str.end(), char_case_eq);
    }

    inline bool str_case_eq(std::string_view s1, std::string_view s2)
    {
        return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), char_case_eq);
    }

    inline std::string tolower_inplace(std::string&& str) noexcept
    {
        std::transform(str.begin(), str.end(), str.begin(), std::tolower);
        return std::move(str);
    }

    inline void skip_space(std::string_view& buffer)
    {
        bool ctn;
        do
        {
            ctn = false;
            while (!buffer.empty() && buffer.front() >= 1 && isspace(buffer.front()))
            {
                buffer = buffer.substr(1);
                ctn = true;
            }
            if (starts_with(buffer, "<!--"))
            {
                buffer = buffer.substr(4);
                while (!starts_with(buffer, "-->"))
                {
                    buffer = buffer.substr(1);
                    ctn = true;
                }
                buffer = buffer.substr(3);
            }
        } while (ctn);
    }

    html_tag parse_tag(std::string_view& buffer)
    {
        skip_space(buffer);
        if (buffer.empty() || buffer.front() != '<') return {};
        buffer = buffer.substr(1);
        std::size_t pos = buffer.find_first_of(" />");
        if (pos == std::string_view::npos)
        {
            html_tag tag(tolower_inplace(std::string(buffer)));
            buffer = {};
            return tag;
        }
        html_tag tag(tolower_inplace(std::string(buffer.data(), pos)));
        buffer = buffer.substr(pos);
        skip_space(buffer);
        while (!buffer.empty() && buffer.front() != '/' && buffer.front() != '>')
        {
            pos = buffer.find('=');
            if (pos == std::string_view::npos)
            {
                pos = buffer.find_first_of(" />");
                std::string key = tolower_inplace(std::string(buffer.data(), pos));
                tag[key] = key;
                buffer = buffer.substr(pos);
                skip_space(buffer);
                continue;
            }
            std::string key = tolower_inplace(std::string(buffer.data(), pos));
            buffer = buffer.substr(pos + 1);
            skip_space(buffer);
            char qc = buffer.front();
            if (qc != '\'' && qc != '\"')
            {
                pos = buffer.find_first_of(" />");
            }
            else
            {
                buffer = buffer.substr(1);
                pos = buffer.find(qc);
            }
            if (pos == std::string_view::npos)
            {
                tag[key] = std::string(buffer.begin(), buffer.end());
                buffer = {};
            }
            else
            {
                tag[key] = std::string(buffer.data(), pos);
                buffer = buffer.substr(pos + 1);
            }
            skip_space(buffer);
        }
        return tag;
    }

    inline html_node_type parse_node_type(std::string_view& buffer)
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

    inline html_node parse_text_node(std::string_view& buffer)
    {
        skip_space(buffer);
        html_node node;
        node.type(html_node_type::text);
        std::size_t pos = buffer.find('<');
        if (pos == std::string_view::npos)
        {
            node.text(std::string(buffer));
            buffer = {};
        }
        else
        {
            node.text(std::string(buffer.data(), pos));
            buffer = buffer.substr(pos);
        }
        return node;
    }

    html_node parse_node(std::string_view& buffer)
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
                std::size_t pos2 = buffer.find_first_of(" >", pos);
                std::string_view cname(buffer.data() + pos, pos2 - pos);
                pos = buffer.find('>', pos);
                pos++;
                auto it = find_if(p.rbegin(), p.rend(), [cname](html_node* pn) { return str_case_eq(pn->tag().name(), cname); });
                if (it == p.rend())
                {
                    buffer = buffer.substr(pos);
                }
                else
                {
                    if (it == p.rbegin())
                    {
                        buffer = buffer.substr(pos);
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
                if (buffer.empty()) break;
                if (buffer.front() == '>')
                {
                    buffer = buffer.substr(1);
                    if (str_case_eq(newn.tag().name(), "script") || str_case_eq(newn.tag().name(), "style"))
                    {
                        std::size_t pos = 0;
                        while (true)
                        {
                            pos = buffer.find('<', pos);
                            if (pos == std::string_view::npos) break;
                            if (starts_with(buffer.substr(pos), "</"))
                            {
                                auto pos2 = pos + 2;
                                auto pos3 = buffer.find_first_of(" >", pos2);
                                if (str_case_eq(newn.tag().name(), std::string_view(buffer.data() + pos2, pos3 - pos2)))
                                    break;
                            }
                            pos++;
                        }
                        if (pos > 0)
                            newn.push_back(tolower_inplace(std::string(buffer.data(), pos)));
                        buffer = buffer.substr(pos);
                        pos = buffer.find('>');
                        buffer = buffer.substr(pos + 1);
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
                    buffer = buffer.substr(pos + 1);
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

    html_decl parse_decl(std::string_view& buffer)
    {
        skip_space(buffer);
        if (!starts_with(buffer, "<!doctype")) return {};
        buffer = buffer.substr(9);
        skip_space(buffer);
        html_decl decl;
        std::size_t pos = buffer.find('>');
        if (pos == std::string_view::npos)
        {
            decl.type(std::string(buffer));
            buffer = {};
        }
        else
        {
            decl.type(std::string(buffer.data(), pos));
            buffer = buffer.substr(pos + 1);
        }
        return decl;
    }

    html_doc parse_doc(std::string_view& buffer)
    {
        html_doc doc;
        doc.decl(parse_decl(buffer));
        doc.node(parse_node(buffer));
        return doc;
    }
} // namespace html
