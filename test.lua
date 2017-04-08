lcoredump = require "lcoredump"
lcoredump.register(2)
lcoredump.register(3)
lcoredump.register(15)
lcoredump.set_show_var(true)

-- if freopen invoked, traceback would be write to file traceback.log
-- lcoredump.freopen("traceback.log")

function foo(tbl, num, str, v_bool, v_nil)
	print("press Ctrl-C(SIGINT) or Ctrl-\\(SIGTERM) please, traceback would be dumped to stderr")
	io.read()
end

function bar(func, s)
	foo({}, 1, "fdsa", true, nil)
end

bar(foo, "hello world!")
