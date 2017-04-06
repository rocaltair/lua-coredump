l = require "lcoredump"
l.register(2)
l.register(3)
l.register(15)
print("press Ctrl-C(SIGINT) or Ctrl-\\(SIGTERM) please, traceback would be dumped to stderr")
io.read()
