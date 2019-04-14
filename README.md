# CppHtml
[WIP]A modern C++ HTML parser.

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
              { { { "p" },
                  { { "Hello world!" } } } } } } }
    };
    cout << doc;
}
```
Output:
``` html
<!doctype html>
<html>
  <head>
    <title>
      Title
    </title>
  </head>
  <body>
    <p>
      Hello world!
    </p>
  </body>
</html>
```

## Parse a HTML document
Implementing...
