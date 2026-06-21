extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <iostream>

int lua_callable_host_function (lua_State* L)
{
  float a = (float) lua_tonumber (L, 1);
  float b = (float) lua_tonumber (L, 2);
  std::cout << "[C++] lua_callable_host_function (" << a << ", " << b << ")\n";
  float c = a * b;
  lua_pushnumber (L, c);
  return 1; // 1 arg returned
}

int main (int, char**)
{
  lua_State *L = luaL_newstate ();
  luaL_openlibs (L);

  lua_register (L, "host_function", lua_callable_host_function);
  struct thing
  {
    std::string name;
    int size;
  } my_thing;

  std::string cmd = "a = 7 + 11 + math.sin(23.7)";
  //auto rc = luaL_dostring (L, cmd.c_str ());
  auto rc = luaL_dofile (L, "foo.lua");
  if (rc == LUA_OK)
  {
    lua_getglobal (L, "thing");
    if (lua_istable (L, -1))
    {
      lua_pushstring (L, "name");
      lua_gettable (L, -2);
      my_thing.name = lua_tostring (L, -1);
      lua_pop (L, 1);

      lua_pushstring (L, "size");
      lua_gettable (L, -2);
      my_thing.size = lua_tonumber (L, -1);
      lua_pop (L, 1);
      std::cout << "thing.name = " << my_thing.name << '\n'
                << "thing.size = " << my_thing.size << '\n';
    }

    lua_getglobal (L, "AddStuff");
    if (lua_isfunction (L, -1))
    {
      lua_pushnumber (L, 1.23f);
      lua_pushnumber (L, 2.34f);
      if (lua_pcall (L, 2, 1, 0) == LUA_OK)
      {
        float result = (float) lua_tonumber (L, -1);
        lua_pop (L, 1);
        std::cout << "function returns " << result << '\n';
      }
    }

    lua_getglobal (L, "DoAThing");
    if (lua_isfunction (L, -1))
    {
      lua_pushnumber (L, 1.23f);
      lua_pushnumber (L, 2.34f);
      if (lua_pcall (L, 2, 1, 0) == LUA_OK)
      {
        float result = (float) lua_tonumber (L, -1);
        lua_pop (L, 1);
        std::cout << "function returns " << result << '\n';
      }
    }
  }
  else
  {
    std::string error = lua_tostring (L, -1);
    std::cout << error << "\n";
  }

  lua_close (L);
  return 0;
}

