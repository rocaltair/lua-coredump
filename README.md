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

you can call ```lcoredump.freopen``` to redirect stderr to a log file, see [test.lua](test.lua)

before coredump file created, traceback for lua & c would be print into stderr.

## NOTICE!!!
lcoredump.set_show_var(flag) if flag is true, every variable in each stack frame would be shown, but easy to crash here again.

## Example
see [test.lua](test.lua)
```
press Ctrl-C(SIGINT) or Ctrl-\(SIGTERM) please, traceback would be dumped to stderr
^Cfull traceback:
[0]test.lua:12: in function foo
        tbl(table) : (null)
        num(number) : 1
        str(string) : fdsa
[1]test.lua:16: in function bar
        func(function) : defined@test.lua:10
        s(string) : hello world!
[2]test.lua:19: in main chunk
[3][C]: ?
        (*temporary)(userdata) : (null)
        (*temporary)(function) : defined@C
0   lcoredump.so                        0x00000001030bfd53 signal_handler + 1555
1   libsystem_platform.dylib            0x00007fffd0e0cbba _sigtramp + 26
2   ???                                 0x0000000104e7609c 0x0 + 4377239708
3   libsystem_c.dylib                   0x00007fffd0c78797 __srefill1 + 24
4   libsystem_c.dylib                   0x00007fffd0c7205a fgets + 103
5   lua                                 0x0000000103079483 read_line + 83
6   lua                                 0x0000000103079569 g_read + 57
7   lua                                 0x00000001030672ac luaD_precall + 588
8   lua                                 0x0000000103073ea0 luaV_execute + 2720
9   lua                                 0x00000001030679b2 luaD_call + 98
10  lua                                 0x0000000103066d95 luaD_rawrunprotected + 85
11  lua                                 0x0000000103067cca luaD_pcall + 58
12  lua                                 0x000000010306282a lua_pcall + 282
13  lua                                 0x000000010305f68d docall + 93
14  lua                                 0x000000010305f0cf pmain + 1567
15  lua                                 0x00000001030672ac luaD_precall + 588
16  lua                                 0x00000001030679a1 luaD_call + 81
17  lua                                 0x0000000103066d95 luaD_rawrunprotected + 85
18  lua                                 0x0000000103067cca luaD_pcall + 58
19  lua                                 0x000000010306289b lua_cpcall + 43
20  lua                                 0x000000010305e9b1 main + 65
21  libdyld.dylib                       0x00007fffd0bff255 start + 1
22  ???                                 0x0000000000000002 0x0 + 2
```
