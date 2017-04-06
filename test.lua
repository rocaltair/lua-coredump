lcoredump = require "lcoredump"
lcoredump.register(2)
lcoredump.register(3)
lcoredump.register(15)

-- if freopen invoked, traceback would be write to file traceback.log
-- lcoredump.freopen("traceback.log")

function foo()
	print("press Ctrl-C(SIGINT) or Ctrl-\\(SIGTERM) please, traceback would be dumped to stderr")
	io.read()
end

function bar()
	foo()
end

bar()
