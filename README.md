<div align="center">
<p>
    <a href="https://onelang.org/" alt="The One Programming Language">
        <img width="150" src="https://avatars.githubusercontent.com/u/40718659?s=200&v=4" alt="The One Programming Language">
    </a>
</p>
<h1>The One Programming Language</h1>

[Onelang.org](https://onelang.org) |
[Help wanted](https://github.com/One-Language/One/issues/new)

</div>
<div align="center">

<!--
[![Build Status][WorkflowBadge]][WorkflowUrl]
-->
[![Patreon][PatreonBadge]][PatreonUrl]
[![Discord][DiscordBadge]][DiscordUrl]
[![Twitter][TwitterUrl]][TwitterBadge]

</div>
Welcome to [One](https://onelang.org) !</br>
It is an open source, self-hosted, bootstrapping **system programming language**
which is easy to build, reliable, and efficient.
It is being developed by [Max](https://github.com/BaseMax) and other open-source  contributors. 
<!--The project is still in its early stages of development-->
<h3>Compiler for One Language will be released in the next few months</h3>
<!-- WRITE PROJECT MOTIVATION HERE -->
![-----------------------------------------------------](
https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


<!-- TABLE OF CONTENTS -->
<h2 id="table-of-contents"> Table of Contents</h2>

<details open="open">
  <ol>
    <li><a href="#Features-of-one">  Features of One</a></li>
    <li><a href="#RoadMap">  RoadMap</a></li>
    <li><a href="#Code-Example"> Code Example</a></li>
    <li><a href="#Getting-started">Getting Started</a></li>
    <li><a href="#contributors">  Contributors</a></li>
    <li><a href="#sponsors"> Sponsors</a></li>
    <li><a href="#license">  License</a></li>
  </ol>
</details>


![-----------------------------------------------------](
https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 id="Features-of-one"> ➤ Features of One</h2>

- Simplicity
- Compiler
- Ability to create output file (x86_64, i386)
- System programming Language
- Intermediate level
- Supports Web Programming 
-<ul>
  <li> Automatic generation of correspoding code for HTML and CSS Language. So, you won't have to learn so many languages so as to develop a website.
  <li> Uses variables in CSS, so that we can obtain the colors or sizes from the database.
  <li> Auto minify the page results
</ul>
- Network, API programming
- Performance and high speed
- Supports inline Assembly code
- Does not require specific libraries and tools on the user system in normal mode (in the future)
- Does not require external runtime libraries in normal mode (in the future)
- Does not require external compilers to do the compilation (in the future)

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


<h2 id="RoadMap"> ➤ RoadMap</h2>

 Grammar of "One" language is available at [here](grammar.BNF).

- [x] Lexer/Parser (Mostly)
- [ ] Code Generator (we can get help from LLVM-C)
- [ ] Developing runtime library and adding features
- [ ] Design web framework for the language.
- [ ] Rewrite compiler in the One language.

<!-- Include to a section about steps of installation -->

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


<h2 id="Code-Example"> ➤ Code Examples</h2>
<!-- Will have to explain how variable assignment, control flow, function declaration and call etc work in the language-->

```c
main {
   ret 0
}
```

__Convert to C:__

```c
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
   global_argc = argc;
   global_argv = argv;
   return (int) 0;
}
```


```c
i32 main {
   ret 10
}
```

__Convert to C:__

```c
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
   global_argc = argc;
   global_argv = argv;
   return (int) 10;
}
```

```c
main {
   string in = "Hello, World!"
   __ in
   return in.length
}
```

__Convert to C:__

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
   global_argc = argc;
   global_argv = argv;
   char *in = "Hello, World!";
   printf("%s\n", in);
   return (int) strlen(in);
}
```
<hr/>

**Another draft:**

```c
import web
home {
    _ "Hi, Welcome"
}
error {
    headers.add('HTTP-Type: 404')
    headers.add('Content-Type: text/html;charset=utf-8')
    _ "<h1>404></h1>"
}
main {
    if system.args.length === 2 {
        port = system.args[1]
    } else  {

        port=8080;
    }
    web.route.add("/", home)
    web.route.add("*", error)
    web.listen(port)
    return 0
}
```

__Another draft:__

```c
error {
    headers.add('HTTP-Type: 404')
    headers.add('Content-Type: text/html;charset=utf-8')
    _ `<!doctype html><html><head>title>Error 404</title><meta charset="utf-8"></head><body><h1>404></h1></body></html>`
}

vs

error {
    headers.add('HTTP-Type: 404')
    headers.add('Content-Type: text/html;charset=utf-8')
    page {
        title: 'Error 404'
        label {
            type: 'h1'
            _ "Not found!"
        }
    }
}
```

---

<h3> Old CLI Development</h3>

```
main:
   // __ "Hello, World!"
   _ "Hello,"
   io.write(' ')
   io.write("World")
   __ '!'
end
```

```
@start
customName:
   _ "Hello, World!\n"
end
```

```
@start
void app:
   __ "Hello, World!"
end
```

```
@start
int customName:
   _ "Hello, World!\n"
   return 0
end
```
<hr/>
<h3> Old GUI Development (Web, Software)</h3>
This architecture is designed only from websites and native softwares. In the future, it will also be available for mobile apps (native). <br/>
Mobile structures are not yet complete and need more thought and attention.<br/><br/>Example to demonstrate  working of the language :
```css
title "Name - Main"
description "Descriptions"
/*
Keyword tag not used in the software, only on the web.
*/
keyword "keywords"
style {
  * {
    margin 0
    padding 0
  }
  header {
    width "100%"
    height "auto"
  }
  list {
    color "red"
  }
  list item {
    display "inline"
    padding "10px"
    background "yellow"
  }
}
header {
  list {
    item {
      _ "Home"
    }
    item {
      _ "About"
    }
    item {
      _ "Contact Us"
    }
  }
}
```

__Convert to HTML/CSS/JS base:__

```html
<html>
  <head>
    <title>Name - Main</title>
    <meta name="description" content="Descriptions" />
    <meta name="keyword" content="keywords" />
    <style>
      * {
        margin: 0;
        padding: 0;
      }
      header {
        width 100%;
        height: auto;
      }
      ul {
        color: red;
      }
      ul li {
        display: inline;
        padding: 10px;
        background: yellow;
      }
    </style>
  </head>
  <body>
    <header>
      <ul>
        <li>Home</li>
        <li>About</li>
        <li>Contact Us</li>
      </ul>
    </header>
  </body>
</html>
```



![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 id="Getting-started"> ➤ Getting Started</h2>

#### Start activity

- Initial start: 2010 about 9 years ago
- Beginning : 2012 about 7 years ago

Refer to the [Github Wiki](https://github.com/ET-Lang/ET/wiki) to start learning this language.
<!-- Installation Steps -->

<!-- #### Prerequisities -->

#### Supported Environments

- [x] GNU / Linux  <!-- which linux?  -->
- [x] Windows
- [ ] macOS (Not complete)
- [ ] BSD
<!-- Write more about the compiler -->

<!--#### Steps -->
<!-- BUilding One from Source -->
<!-- Hello World in One -->
<!-- Tips to understand the language better -->
<!-- _Filename extensions: `.one`__ -->

![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


<!--Contribute-->
<h2 id="contributors"> ➤ Contributors</h2>
We welcome all kinds of contributions, including bug reports, feature requests, documentation improvements etc.
To ask a question or open a discussion, create an issue or join the [One Discord Server](https://discord.gg/sFCE2HcMCa).

If you would like to contribute in the development of this project, please reach us at : <maxbasecode@gmail.com>


If you are not familiar with how to make a pull request on Github then please read this [guide](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests).

You can also help in the development of One by making some donations - [Patreon](https://www.patreon.com/onelanguage).

Thanks to all the [contributors](https://github.com/One-Language/One/graphs/contributors)!!



![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)


<!-- Sponsors -->
<h2 id="sponsors"> ➤ Sponsors</h2>

Become a sponsor and help ensure the development of the programming language - "One". </br> Your support can help sustain it.

[:heart: Sponsor on Github ](https://github.com/sponsors/BaseMax)


Created By Max Base @ 2019
![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<h2 id="license"> ➤ License</h2>
One is released under the GNU General Public License v3.0. Please refer to the terms in the [license](LICENSE) file included in the repository.

<!--
[![Gitter](https://badges.gitter.im/ET_lang/community.svg)](https://gitter.im/ET_lang/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
-->
<!--
[Official Community for a chat and discuss.](https://spectrum.chat/et?tab=chat)
-->

[DiscordBadge]: https://img.shields.io/discord/592103645835821068?label=Discord&logo=discord&logoColor=white
[PatreonBadge]: https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dvlang%26type%3Dpledges
[SponsorBadge]: https://camo.githubusercontent.com/da8bc40db5ed31e4b12660245535b5db67aa03ce/68747470733a2f2f696d672e736869656c64732e696f2f7374617469632f76313f6c6162656c3d53706f6e736f72266d6573736167653d254532253944254134266c6f676f3d476974487562
[TwitterBadge]: https://twitter.com/MaxProgram
[DiscordUrl]: https://discord.gg/sFCE2HcMCa
[PatreonUrl]: https://patreon.com/onelanguage
[TwitterUrl]: https://img.shields.io/twitter/follow/v_language.svg?style=flatl&label=Follow&logo=twitter&logoColor=white&color=1da1f2
