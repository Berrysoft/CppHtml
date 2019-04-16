#include <html/html_doc.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace html;

int main()
{
    istringstream stream("<!doctype html><html><HEAD><title>Title<!--</title>--></head></title><body><h1 align=\"center\">Hello</H1><P>Hello <p>world!<br/></body></html>");
    html_doc doc;
    stream >> doc;
    cout << doc << endl;
    for (auto& p : doc.node()["body"].back()["p"])
    {
        cout << p.front().text();
    }
}
