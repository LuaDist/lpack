# makefile for pack library for Lua

# change this to reflect your installation
LUA=/l/lua
LUAINC= /l/lua
LUALIB= /l/lua
LUA_C= /l/lua/lua.c

# change this to reflect your installation
LUA=/tmp/lua-4.0.1
LUAINC= $(LUA)/include
LUALIB= $(LUA)/lib
LUA_C= $(LUA)/src/lua/lua.c

# no need to change anything below here

CFLAGS= $(INCS) $(DEFS) $(WARN) -O2
WARN= -ansi -pedantic -Wall #-Wmissing-prototypes

INCS= -I$(LUAINC) -I.
LIBS= -L$(LUALIB) -llua -llualib -lm

OBJS= lua.o packlib.o

T=a.out

all:	test

$T:	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

test:	$T
	@$T test.lua
	@#$T test.lua | xxd

lua.c:	$(LUA_C)
	sed '/dblib/s/$$/ lua_packlibopen(L);/' <$? >$@

clean:
	rm -f $(OBJS) $T lua.c core a.out

x:
	@grep '	pos_' packlib.c | cut -f2 | tr -d '{",' | sort | column

# distribution

D=pack
A=$D.tar.gz
TOTAR=Makefile,README,packlib.c,test.lua

tar:	clean
	tar zcvf $A -C .. $D/{$(TOTAR)}

distr:	tar
	mv $A ftp

diff:
	tar zxf ftp/$A
	diff . $D
