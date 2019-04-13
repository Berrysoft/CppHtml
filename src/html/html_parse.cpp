#include <html/html_doc.hpp>
#include <html/html_error.hpp>
#include <memory>

using namespace std;

namespace html
{
    namespace impl
    {
        void skip_space(array_view<const char>& buffer)
        {
            while (!buffer.empty() && isspace(buffer.front()))
                buffer += 1;
        }

        html_tag parse_tag(array_view<const char>& buffer)
        {
            skip_space(buffer);
            // TODO: tag
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

        shared_ptr<html_node> parse_node(array_view<const char>& buffer)
        {
            skip_space(buffer);
            shared_ptr<html_node> node = make_shared<html_node>();
            node->tag() = parse_tag(buffer);
            shared_ptr<html_node_base> child = nullptr;
            while (true)
            {
                skip_space(buffer);
                child = parse_node_base(buffer);
                if (!child) break;
                node->push_back(child);
            }
            buffer += 2;
            std::size_t pos = buffer.find('>');
            if (pos != array_view<const char>::npos && node->tag().name() == string_view(buffer.data(), pos))
            {
                return node;
            }
            else
            {
                // TODO: reduce error.
                throw html_error("Node pair doesn't match.");
            }
        }

        shared_ptr<html_text_node> parse_text_node(array_view<const char>& buffer)
        {
            skip_space(buffer);
            shared_ptr<html_text_node> node = make_shared<html_text_node>();
            std::size_t pos = buffer.find('<');
            if (pos == array_view<const char>::npos)
            {
                node->text() = string(buffer.begin(), buffer.end());
                buffer += pos;
            }
            else
            {
                node->text() = string(buffer.begin(), buffer.begin() + pos);
                buffer += pos;
            }
            return node;
        }

        shared_ptr<html_node_base> parse_node_base(array_view<const char>& buffer)
        {
            skip_space(buffer);
            if (buffer.empty()) return nullptr;
            html_node_type type = parse_node_type(buffer);
            switch (type)
            {
            case html_node_type::node:
                return parse_node(buffer);
            case html_node_type::text:
                return parse_text_node(buffer);
            default:
                return nullptr;
            }
        }

        html_decl parse_decl(array_view<const char>& buffer)
        {
            skip_space(buffer);
            if (buffer.empty()) throw html_error("No declaration found.");
            // TODO: type
        }

        html_doc parse_doc(array_view<const char>& buffer)
        {
            html_doc doc;
            doc.decl() = parse_decl(buffer);
            doc.node() = parse_node(buffer);
            return doc;
        }
    } // namespace impl

    html_tag html_tag::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_tag(buffer);
    }

    shared_ptr<html_node> html_node::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_node(buffer);
    }

    shared_ptr<html_text_node> html_text_node::parse(impl::array_view<const char> buffer)
    {
        return impl::parse_text_node(buffer);
    }

    shared_ptr<html_node_base> html_node_base::parse(impl::array_view<const char> buffer)
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
