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
        tbl(table) : 0x7ffaa940a1f0
        num(number) : 1
        str(string) : fdsa
        v_bool(boolean) : true
        v_nil(nil) : nil
[1]test.lua:16: in function bar
        func(function) : 0x7ffaa9409a80 defined@test.lua:10
        s(string) : hello world!
[2]test.lua:19: in main chunk
[3][C]: ?
        (*temporary)(userdata) : 0x7fff548063d8
        (*temporary)(function) : 0x7ffaa9409020 defined@C
0   lcoredump.so                        0x000000010b45acf6 signal_handler + 1926
1   libsystem_platform.dylib            0x00007fffd0e0cbba _sigtramp + 26
2   ???                                 0x000000011718609c 0x0 + 4682440860
3   libsystem_c.dylib                   0x00007fffd0c78797 __srefill1 + 24
4   libsystem_c.dylib                   0x00007fffd0c7205a fgets + 103
5   lua                                 0x000000010b414483 read_line + 83
6   lua                                 0x000000010b414569 g_read + 57
7   lua                                 0x000000010b4022ac luaD_precall + 588
8   lua                                 0x000000010b40eea0 luaV_execute + 2720
9   lua                                 0x000000010b4029b2 luaD_call + 98
10  lua                                 0x000000010b401d95 luaD_rawrunprotected + 85
11  lua                                 0x000000010b402cca luaD_pcall + 58
12  lua                                 0x000000010b3fd82a lua_pcall + 282
13  lua                                 0x000000010b3fa68d docall + 93
14  lua                                 0x000000010b3fa0cf pmain + 1567
15  lua                                 0x000000010b4022ac luaD_precall + 588
16  lua                                 0x000000010b4029a1 luaD_call + 81
17  lua                                 0x000000010b401d95 luaD_rawrunprotected + 85
18  lua                                 0x000000010b402cca luaD_pcall + 58
19  lua                                 0x000000010b3fd89b lua_cpcall + 43
20  lua                                 0x000000010b3f99b1 main + 65
21  libdyld.dylib                       0x00007fffd0bff255 start + 1
22  ???                                 0x0000000000000002 0x0 + 2
```
