# CppHtml
A modern C++ HTML parser. Still has lots of bugs...

## Define
``` cpp
using namespace std;
using namespace html;
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
```
Output:
``` html
<!doctype html>
<html>
  <head>
    <title>Title</title>
  </head>
  <body>
    <h1 align="center">Hello</h1>
    <p>Hello world!</p>
  </body>
</html>
```

## Parse
``` cpp
using namespace std;
using namespace html;
html_doc doc = html_doc::parse("<!doctype html><html><head><title>Title</title></head><body><h1 align=\"center\">Hello</h1><p>Hello world!</p></body></html>");
cout << doc << endl;
```
The output is the same as above.

## Query
``` cpp
using namespace std;
using namespace html;
html_doc doc = html_doc::parse("<!doctype html><html><head><title>Title<!--</title>--></head></title><body><h1 align=\"center\">Hello</h1><p>Hello <p>world!<br/></body></html>");
for (auto& p : doc.node()["body"].back()["p"])
{
    cout << p.front().text();
}
```
Output:
```
Hello world!
```
