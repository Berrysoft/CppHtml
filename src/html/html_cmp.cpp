#include <algorithm>
#include <html/html_doc.hpp>

using namespace std;

namespace html
{
    bool operator==(const html_tag& t1, const html_tag& t2)
    {
        return t1.m_name == t2.m_name && equal(t1.m_attrs.begin(), t1.m_attrs.end(), t2.m_attrs.begin(), t2.m_attrs.end());
    }

    bool operator==(const html_decl& d1, const html_decl& d2)
    {
        return d1.m_type == d2.m_type;
    }

    bool operator==(const html_node_data& d1, const html_node_data& d2)
    {
        return d1.tag == d2.tag && equal(d1.children.begin(), d1.children.end(), d2.children.begin(), d2.children.end());
    }

    bool operator==(const html_node& n1, const html_node& n2)
    {
        return n1.m_data == n2.m_data;
    }

    bool operator==(const html_doc& d1, const html_doc& d2)
    {
        return d1.m_decl == d2.m_decl && d1.m_node == d2.m_node;
    }
} // namespace html
