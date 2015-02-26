#include <lua.h>
#include <lauxlib.h>
#include <enscrypt.h>
#include <string.h>

#define LUAENSCRYPT_VERSION   			"lua-enscrypt 0.1"
#define LUAENSCRYPT_COPYRIGHT 			"Copyright (C) 2014, lua-enscrypt authors"
#define LUAENSCRYPT_DESCRIPTION  		"Binding for the enscrypt library."

#define N_FACTOR 9

uint8_t *salt = NULL;

const char hexTable[33] = "0123456789abcdef0123456789ABCDEF";

// luaL_setfuncs() from lua 5.2
static void setfuncs (lua_State *L, const luaL_reg*l, int nup)
{
	luaL_checkstack(L, nup, "Too many upvalues.");
	for(; l && l->name; l++) {
		int i;
		for (i = 0; i < nup; i++)
			lua_pushvalue(L, -nup);
		lua_pushcclosure(L, l->func, nup);
		lua_setfield(L, -(nup + 2), l->name);
	}
	lua_pop(L, nup);
}


// from enscrypt_bin.c
void hexify( char *out, const uint8_t *in )
{
	int i;
	for( i = 0; i < 32; i++ ) {
		out[i*2] = (char)hexTable[in[i]>>4];
		out[i*2+1] = (char)hexTable[in[i]&0x0F];
	}
	out[i*2] = 0;
}

void unhexify( uint8_t *out, const char *in )
{
	int i, a;
	char *p;
	memset( out, 0, 32 );
	for( i = 0; i < 64; i++ ) {
		p = strchr( hexTable, in[i] );
		if( p ) {
			a = (p - hexTable) % 16;
			out[i/2] |= (i%2) ? a : a<<4;
		}
	}
}

static int lenscrypt_hash(lua_State * L)
{
	const char * password = luaL_checkstring(L, 1);
	const char * salthex = luaL_checkstring(L, 2);
	int iterations = lua_tointeger(L, 3);
	uint8_t buf[32];
	char str[65];
	int retVal;
	double startTime, endTime, elapsed;
	size_t password_len = 0;

	int len = strlen(salthex);
	if(len == 64 && strspn(salthex, hexTable) == 64) {
		if(salt) free(salt);
		salt = malloc(32);
		unhexify(salt, salthex);
	} else {
		return luaL_error(L, "Salt should be a 64 character hex string.");
	}

	password_len = strlen(password);

	startTime = enscrypt_get_real_time();

	if(iterations) {
		retVal = enscrypt(buf, password, password_len, salt, 32, iterations, N_FACTOR, NULL, NULL);
	} else {
		iterations = 1;
		retVal = enscrypt(buf, password, password_len, salt, 32, iterations, N_FACTOR, NULL, NULL);
	}

	endTime = enscrypt_get_real_time();
	elapsed = endTime - startTime;

	if(retVal > -1) {
		hexify(str, buf);
		lua_pushstring(L, str);
		lua_pushnumber(L, elapsed);
		return 2;
	} else {
		return luaL_error(L, "Error running enscrypt.");
	}
}

static const struct luaL_reg E[] =
{
	{ NULL, NULL }
};

static const struct luaL_reg R[] =
{
	{ "hash", lenscrypt_hash },

	{ NULL, NULL }
};


LUALIB_API int luaopen_enscrypt(lua_State * L)
{
	luaL_register(L, "enscrypt", E);

	lua_pushliteral(L, LUAENSCRYPT_VERSION);
	lua_setfield(L, -2, "_VERSION");

	lua_pushliteral(L, LUAENSCRYPT_COPYRIGHT);
	lua_setfield(L, -2, "_COPYRIGHT");

	lua_pushliteral(L, LUAENSCRYPT_DESCRIPTION);
	lua_setfield(L, -2, "_DESCRIPTION");

	lua_pushvalue(L, -1);
	setfuncs(L, R, 1);

	return 1;
}
