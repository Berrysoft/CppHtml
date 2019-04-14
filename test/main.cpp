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
              { { { "p" },
                  { { "Hello world!" } } } } } } }
    };
    cout << doc;
}
