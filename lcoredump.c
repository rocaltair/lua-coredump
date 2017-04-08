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

#define MAX_BUF_SZ (1024 * 1024)
#define MAX_FRAME_SZ 128

/**
 * signal(SIGSEGV, crash_signal_handler);
 */

lua_State *GlobalL = NULL;
static int is_full_stack = 0;

static size_t dump_lua_traceback(lua_State *L, char *buf, size_t sz, int is_show_var)
{
	size_t e = 0;
	int level = 1;
	int firstpart = 1;
	lua_Debug ar;
	int i = 0;

	pushfstring(buf, e, "%s traceback:", is_show_var ? "full" : "");
	while (lua_getstack(L, level++, &ar)) {
		if (level > LEVELS1 && firstpart) {
			if (!lua_getstack(L, level+LEVELS2, &ar))
				level--;
			else {
				pushfstring(buf, e, "%s", "\n...");
				while (lua_getstack(L, level+LEVELS2, &ar))
					level++;
			}
			firstpart = 0;
			continue;
		}
		pushfstring(buf, e, "\n");
		lua_getinfo(L, "Snl", &ar);
		pushfstring(buf, e, "[%d]%s:", level - 2, ar.short_src);
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

		if (!lua_checkstack(L, 1)) /* to call lua_getlocal*/
			continue;

		i = 1;
		while(is_show_var) {
			const void *pointer;
			int type;
			lua_Debug arf;
			const char *name = lua_getlocal(L, &ar, i++);
			const char *typename;
			if (name == NULL)
				break;
			type = lua_type(L, -1);
			typename = lua_typename(L, type);
			pushfstring(buf, e, "\n\t%s(%s) : ", name, typename);
			switch(type) {
			case LUA_TFUNCTION:
				pointer = lua_topointer(L, -1);
				lua_getinfo(L, ">Snl", &arf);
				if (*arf.what == 'C' || *arf.what == 't')
					pushfstring(buf, e, "%p %s@C",
						    pointer,
						    (arf.name != NULL ? arf.name : "defined"));
				else
					pushfstring(buf, e, "%p %s@%s:%d",
						    pointer,
						    (arf.name != NULL ? arf.name : "defined"),
						    arf.short_src, arf.linedefined);
				break;
			case LUA_TBOOLEAN:
				pushfstring(buf, e, "%s", lua_toboolean(L, 1) ? "true" : "false");
				lua_pop(L, 1);
				break;
			case LUA_TNIL:
				pushfstring(buf, e, "nil");
				lua_pop(L, 1);
				break;
			case LUA_TNUMBER:
			case LUA_TSTRING:
				pushfstring(buf, e, "%s", lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			default:
				pushfstring(buf, e, "%p", lua_topointer(L, -1));
				lua_pop(L, 1);
				break;
			}
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
	dump_lua_traceback(L, buf, MAX_BUF_SZ, is_full_stack);
	fprintf(stderr, "%s", buf);
	fflush(stderr);
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

static int lua__freopen(lua_State *L)
{
	const char * filepath = luaL_checkstring(L, 1);
	freopen(filepath, "a", stderr);
	return 0;
}

static int lua__set_show_var(lua_State *L)
{
	is_full_stack = lua_toboolean(L, 1);
	return 0;
}

int luaopen_lcoredump(lua_State* L)
{
	luaL_Reg lfuncs[] = {
		{"register", lua__register},
		{"freopen", lua__freopen},
		{"set_show_var", lua__set_show_var},
		{NULL, NULL},
	};
	luaL_newlib(L, lfuncs);
	return 1;
}
