#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

/*for dump_c_traceback, *nix only */
#include <execinfo.h>
#include <unistd.h>

#if LUA_VERSION_NUM < 502
# ifndef luaL_newlib
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
# endif
#endif


#define pushfstring(buf, e, fmt, ...) do{if(sz-e > 0) e+=snprintf(buf+e, sz-e, fmt, ##__VA_ARGS__);}while(0)
#define LEVELS1	12
#define LEVELS2	10

#define MAX_BUF_SZ (256 * 1024)
#define MAX_FRAME_SZ 128

/**
 * signal(SIGSEGV, crash_signal_handler);
 */

lua_State *GlobalL = NULL;

static size_t dump_lua_traceback(lua_State *L, char *buf, size_t sz)
{
	size_t e = 0;
	int level = 1;
	int firstpart = 1;
	lua_Debug ar;

	pushfstring(buf, e, "traceback:");
	while (lua_getstack(L, level++, &ar)) {
		if (level > LEVELS1 && firstpart) {
			if (!lua_getstack(L, level+LEVELS2, &ar))
				level--;
			else {
				pushfstring(buf, e, "%s", "\n\t...");
				while (lua_getstack(L, level+LEVELS2, &ar))
					level++;
			}
			firstpart = 0;
			continue;
		}
		pushfstring(buf, e, "\n\t");
		lua_getinfo(L, "Snl", &ar);
		pushfstring(buf, e, "%s:", ar.short_src);
		if (ar.currentline > 0)
			pushfstring(buf, e, "%d:", ar.currentline);
		if (*ar.namewhat != '\0')
			pushfstring(buf, e, " in function %s", ar.name);
		else {
			if (*ar.what == 'm')
				pushfstring(buf, e, " in main chunk");
			else if (*ar.what == 'C' || *ar.what == 't')
				pushfstring(buf, e, " ?");
			else
				pushfstring(buf, e, " in function <%s:%d>",
					    ar.short_src, ar.linedefined);
		}
	}
	pushfstring(buf, e, "\n");
	return e;
}

/* *nix only */
void dump_c_traceback()
{
	void *frames[MAX_FRAME_SZ];
	size_t frame_size;
	frame_size = backtrace(frames, sizeof(frames) / sizeof(frames[0]));
	/*you can also use backtrace_symbols here */
	backtrace_symbols_fd(frames, frame_size, STDERR_FILENO);
}

static void signal_handler(int signum)
{
	lua_State *L = GlobalL;
	char *buf;
	signal(signum, SIG_DFL);

	buf = malloc(MAX_BUF_SZ);
	dump_lua_traceback(L, buf, MAX_BUF_SZ);
	fprintf(stderr, "%s", buf);
	free(buf);

	/* *nix only */
	dump_c_traceback();
}

static int lua__register(lua_State *L)
{
	int signum = luaL_checkinteger(L, 1);
	GlobalL = L;
 	signal(signum, signal_handler);
	return 0;
}

int luaopen_lcoredump(lua_State* L)
{
	luaL_Reg lfuncs[] = {
		{"register", lua__register},
		{NULL, NULL},
	};
	luaL_newlib(L, lfuncs);
	return 1;
}
