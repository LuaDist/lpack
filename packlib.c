/*
* packlib.c -- a Lua library for packing and unpacking binary data
*/

#include <string.h>

#include "lua.h"
#include "lauxlib.h"

#ifndef LUA_REGISTRYINDEX
#define lua_Number double
#endif

static void badmode(lua_State *L, int c)
{
 char s[]="bad mode `?'";
 s[sizeof(s)-3]=c;
 luaL_argerror(L,1,s);
}

static int copy(lua_State *L, void *a, size_t n, const char *s, int i, size_t len)
{
 if (i+n>len) luaL_argerror(L,1,"input string too short");
 memcpy(a,s+i,n);
 return i+n;
}

static int l_unpack(lua_State *L) 		/* unpack(s,f,[init]) */
{
 const char *s=luaL_check_string(L,1);
 const char *f=luaL_check_string(L,2);
 int i=luaL_opt_number(L,3,1)-1;
 size_t len=lua_strlen(L,1);
 int n;
 for (n=0; f[n]; n++)
 {
  int c=f[n];
  switch (c)
  {
   case 'S':
   {
    size_t l;
    i=copy(L,&l,sizeof(l),s,i,len);
    if (i+l>len) luaL_argerror(L,1,"input string too short");
    lua_pushlstring(L,s+i,l);
    i+=l;
    break;
   }
   case 'n':
   {
    lua_Number a;
    i=copy(L,&a,sizeof(a),s,i,len);
    lua_pushnumber(L,a);
    break;
   }
   case 'd':
   {
    double a;
    i=copy(L,&a,sizeof(a),s,i,len);
    lua_pushnumber(L,a);
    break;
   }
   case 'f':
   {
    float a;
    i=copy(L,&a,sizeof(a),s,i,len);
    lua_pushnumber(L,a);
    break;
   }
   case 'i':
   {
    int a;
    i=copy(L,&a,sizeof(a),s,i,len);
    lua_pushnumber(L,a);
    break;
   }
   case 'b':
   {
    unsigned char a;
    char b[1];
    i=copy(L,&a,sizeof(a),s,i,len);
    b[0]=(char)a;
    lua_pushlstring(L,b,1);
    break;
   }
   case 'c':
   {
    char a;
    i=copy(L,&a,sizeof(a),s,i,len);
    lua_pushlstring(L,&a,1);
    break;
   }
   default:
    badmode(L,c);
    break;
  }
 }
 lua_pushnumber(L,i+1);
 return n+1;
}

/* s string, d double, f float, i int, w short, b byte, c char */

static int l_pack(lua_State *L) 		/* pack(s,...) */
{
 int i,j;
 const char *s=luaL_check_string(L,1);
 luaL_Buffer b;
 luaL_buffinit(L,&b);
 for (i=0, j=2; s[i]; i++, j++)
 {
  int c=s[i];
  switch (c)
  {
   case 'S':
   {
    size_t l;
    const char *a=luaL_check_lstr(L,j,&l);
    luaL_addlstring(&b,(void*)&l,sizeof(l));
    luaL_addlstring(&b,a,l);
    break;
   }
   case 's':
   {
    size_t l;
    const char *a=luaL_check_lstr(L,j,&l);
    luaL_addlstring(&b,a,l);
    break;
   }
   case 'n':
   {
    lua_Number a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   case 'd':
   {
    double a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   case 'f':
   {
    float a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   case 'i':
   {
    int a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   case 'b':
   {
    unsigned char a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   case 'c':
   {
    char a=luaL_check_number(L,j);
    luaL_addlstring(&b,(void*)&a,sizeof(a));
    break;
   }
   default:
    badmode(L,c);
    break;
  }
 }
 luaL_pushresult(&b);
 return 1;
}

int lua_packlibopen(lua_State *L)
{
 lua_register(L,"bpack",l_pack);
 lua_register(L,"bunpack",l_unpack);
 return 0;
}
