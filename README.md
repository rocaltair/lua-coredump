# lua-coredump

how to locate coredump while lua invoking C?

this lib gives you a choice.

for example.
if you get a core file, get signal num, such as SIGSEGV

first, get SIGSEGV num in your shell, run ```kill -l SEGV```, may be 11 in zsh

in lua:
```
lcoredump = require "lcoredump"
lcoredump.register(11)
```

before coredump file created, traceback for lua & c would be print into stderr.

see test.lua

use Ctrl-C or Ctrl-\, you would get some trace like following:

