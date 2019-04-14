#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc;
    html_decl decl;
    decl.type("html");
    doc.decl(decl);
    html_node html;
    html.type(html_node_type::node);
    html.tag().name("html");
    html_node head;
    head.type(html_node_type::node);
    head.tag().name("head");
    html_node title;
    title.type(html_node_type::node);
    title.tag().name("title");
    html_node titlet;
    titlet.type(html_node_type::text);
    titlet.text("Title");
    title.push_back(move(titlet));
    head.push_back(move(title));
    html.push_back(move(head));
    html_node body;
    body.type(html_node_type::node);
    body.tag().name("body");
    html_node p1;
    p1.type(html_node_type::node);
    p1.tag().name("p");
    html_node p1t;
    p1t.type(html_node_type::text);
    p1t.text("Hello world!");
    p1.push_back(move(p1t));
    body.push_back(move(p1));
    html.push_back(move(body));
    doc.node(move(html));
    cout << doc;
}
