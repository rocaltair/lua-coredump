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

you can call lcoredump.freopen to redirect stderr to a log file, see [test.lua](test.lua)

before coredump file created, traceback for lua & c would be print into stderr.

see [test.lua](test.lua)
```
Ctrl-C(SIGINT) or Ctrl-\(SIGTERM) please, traceback would be dumped to stderr
^\traceback:
	test.lua:8: in function foo
	test.lua:12: in function bar
	test.lua:15: in main chunk
	[C]: ?
0   lcoredump.so                        0x000000010d225e2d signal_handler + 861
1   libsystem_platform.dylib            0x00007fffd0e0cbba _sigtramp + 26
2   ???                                 0x0000000000000001 0x0 + 1
3   libsystem_c.dylib                   0x00007fffd0c78797 __srefill1 + 24
4   libsystem_c.dylib                   0x00007fffd0c7205a fgets + 103
5   lua                                 0x000000010d1e0483 read_line + 83
6   lua                                 0x000000010d1e0569 g_read + 57
7   lua                                 0x000000010d1ce2ac luaD_precall + 588
8   lua                                 0x000000010d1daea0 luaV_execute + 2720
9   lua                                 0x000000010d1ce9b2 luaD_call + 98
10  lua                                 0x000000010d1cdd95 luaD_rawrunprotected + 85
11  lua                                 0x000000010d1cecca luaD_pcall + 58
12  lua                                 0x000000010d1c982a lua_pcall + 282
13  lua                                 0x000000010d1c668d docall + 93
14  lua                                 0x000000010d1c60cf pmain + 1567
15  lua                                 0x000000010d1ce2ac luaD_precall + 588
16  lua                                 0x000000010d1ce9a1 luaD_call + 81
17  lua                                 0x000000010d1cdd95 luaD_rawrunprotected + 85
18  lua                                 0x000000010d1cecca luaD_pcall + 58
19  lua                                 0x000000010d1c989b lua_cpcall + 43
20  lua                                 0x000000010d1c59b1 main + 65
21  libdyld.dylib                       0x00007fffd0bff255 start + 1
22  ???                                 0x0000000000000002 0x0 + 2
```
