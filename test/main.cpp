#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc = html_doc::parse(array_view("<!doctype html><html><head><title>Title</title></head><body><h1 align=\"center\">Hello</h1><p>Hello world!</p></body></html>"));
    cout << doc << endl;
}
