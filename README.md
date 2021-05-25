<div align="center">
<p>
    <a href="https://onelang.org/" alt="The One Programming Language">
        <img width="80" src="https://avatars.githubusercontent.com/u/40718659?s=200&v=4" alt="The One Programming Language">
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

One is an open source, self-hosted, bootstrapping **system programming language**. It is easy to build, reliable, and efficient performant software.

The compiler will be released in next few months.

Discord server: https://discord.gg/sFCE2HcMCa

# Objectives and Features

- Easy
- Compiler
- Ability to create output file (x86_64, i386)
- System programming
- Intermediate level
- Web Programming
  - Automatic generation of HTML and CSS code. So you not need to learn so many language to develop a website
  - Use variables in CSS, so we can easily get colors or size from database or etc.
  - Auto minify page result
- Network, API programming
- Performance and high speed
- Supports inline Assembly code
- Does not require specific libraries and tools on the user system in normal mode (in the future)
- Does not require external runtime libraries in normal mode (in the future)
- Does not require external compilers for compile steps (in the future)

## Stages

The grammar of "One" language available at [here](grammar.BNF).

- [x] Lexer/Parser (Mostly)
- [ ] Code Generator (we can get help from LLVM-C)
- [ ] Developing runtime library and adding features
- [ ] Design web framework for this language (Developers can design/create website without using HTML,CSS language)
- [ ] Rewrite compiler in own language (Bootstrapping)

### Development help is wanted

Please contact one of the authors by email. See email addresses elsewhere.

> maxbasecode [ @ ] gmail { . DOT . } com

## Concept

```c
main {
   ret 0
}
```

Convert to C:

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

Convert to C:

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

Convert to C:

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

Another draft:

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

Another draft:

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

## Old CLI Development

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

## Old GUI Development (Web, Software)

This architecture is only designed for website and software (native). In the
future, it will also be available for mobile apps (native).

Mobile structures are not yet complete and need more thought and attention.

An example of the language at work:

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

Convert to HTML/CSS/JS base:

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

The end result:

<p align="center">
 <img alt="Screen Of Render" src="https://github.com/ET-Lang/ET/raw/master/tests/42-architecture-sample.png" width="80%">
</p>

# Learning

Refer to the [Wiki](https://github.com/ET-Lang/ET/wiki) to start learning this language.

# Introduction

#### Start activity

- Initial start: 2010 about 9 years ago
- Beginning : 2012 about 7 years ago

#### OS

- [x] GNU / Linux
- [x] Windows
- [ ] macOS (Not complete)
- [ ] BSD

#### Filename extensions: `.one`

Send your message for sponsorship and to support us. (MaxBaseCode {[@]} Gmail {[DOT]} Com)

Created By Max Base @ 2019

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
