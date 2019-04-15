#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc = html_doc::parse("<!doctype html><html><head><title>Title<!--</title>--></head></title><body><h1 align=\"center\">Hello</h1><p>Hello <p>world!<br/></body></html>");
    cout << doc << endl;
    for (auto& p : doc.node()["body"].back()["p"])
    {
        cout << p.front().text();
    }
}
