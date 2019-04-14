# CppHtml
A modern C++ HTML parser. Still has lots of bugs...

## Define a HTML document
``` cpp
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
                { { "p" },
                  { { "Hello world!" } } } } } } }
    };
    cout << doc << endl;
}
```
Output:
``` html
<!doctype html>
<html>
  <head>
    <title>Title</title>
  </head>
  <body>
    <h1 align='center'>Hello</h1>
    <p>Hello world!</p>
  </body>
</html>
```

## Parse a HTML document
``` cpp
#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc = html_doc::parse(array_view("<!doctype html><html><head><title>Title</title></head><body><h1 align=\"center\">Hello</h1><p>Hello world!</p></body></html>"));
    cout << doc << endl;
}
```
The output is the same as above.
