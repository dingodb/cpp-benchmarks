#include <stdio.h>

#include <luajit/lauxlib.h>
#include <luajit/lua.h>
#include <luajit/lualib.h>

extern void my_func(void)
{
    printf("f\n");
}

const char *lua = "local ffi = require(\"ffi\")\n"
          "ffi.cdef[[\n"
          "void my_func(void);\n"
          "]]\n"
          "ffi.C.my_func()\n";

int main(int argc, char **argv)
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    if (luaL_dostring(L, lua)) {
        printf("err: %s\n", lua_tostring(L, -1));
    }
    lua_close(L);

    return 0;
}
