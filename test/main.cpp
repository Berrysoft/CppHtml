#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc{
        { "html" },
        { { "html" },
          { { { "head" },
              { { { "title" },
                  { { "Title" } } } } },
            { { "body" },
              { { { "h1", { { "align", "center" } } },
                  { { "Hello" } } },
                { { "p", { { "custom", "\"\'a&b<c>" } } },
                  { { "Hello world!" } } } } } } }
    };
    cout << doc << endl;
}
