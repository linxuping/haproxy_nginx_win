#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
#include <windows.h>
#include "iOSLayerHeader.h"
#pragma comment(lib, "iHelper.lib")

int CALLBACK DeviceCall(LPVOID pDevice, int nFlag, LPVOID pExtra)
{
	return 0;
}

void ch2wchstr(const char* buffer, wchar_t** wBuf)
{
    size_t len = strlen(buffer)+1;
    size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
    *wBuf = new wchar_t[wlen + 1];
    MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), *wBuf, int(wlen));
}

DWORD WINAPI OnTimeOut(LPVOID lpParam)
{
	int seconds = *(int*)lpParam;
	Sleep(seconds*1000);
	exit(0);
	return 0;
}

extern "C" int xige(lua_State* L)
{
	size_t len;
    const char* username = luaL_checklstring(L,1, &len);
    const char* passwd = luaL_checklstring(L,2, &len);
	int itemid = luaL_checknumber(L,3);
	int seconds = luaL_checknumber(L,4);

	DWORD tid;
	CloseHandle(CreateThread(NULL, 0, OnTimeOut, LPVOID(&seconds), 0, &tid));
    
	errcode err = StartiOSLayer(DeviceCall);
	char *recv_plist = NULL;
	UINT64 userid = 0;
	wchar_t *wusername=L"",*wpasswd=L"";
	ch2wchstr(username, &wusername);
	ch2wchstr(passwd, &wpasswd);
	err = XiGe(wusername, wpasswd, itemid, NULL, NULL, NULL, NULL, &recv_plist, &userid);
	//lua_pushnumber(L,userid);
	lua_pushstring(L, recv_plist);
	lua_pushnumber(L, err);
	delete wusername;
	delete wpasswd;
    return 2;
}

extern "C" int xige2(lua_State* L)
{
	size_t len;
    
	errcode err = StartiOSLayer(DeviceCall);
	char *recv_plist = NULL;
	UINT64 userid = 0;
	//err = XiGe(L"", L"", 1009227147, NULL, NULL, NULL, NULL, &recv_plist, &userid);
	//lua_pushnumber(L,userid);
	//lua_pushstring(L, recv_plist);
	lua_pushstring(L, "teststr");
	lua_pushnumber(L, 88);

    return 2;
}

static luaL_Reg mylibs[] = { 
	{"xige", xige},
	{"xige2", xige2},
    {NULL, NULL} 
}; 

//该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
//1. 我们可以将该函数简单的理解为模块的工厂函数。
//2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
//3. 在luaL_register的调用中，其第一个字符串参数为模块名"xxx"，第二个参数为待注册函数的数组。
//4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
//   否则将无法调用。
extern "C" __declspec(dllexport)
int luaopen_xigelua(lua_State* L) 
{

	const char* libName = "xigelua";
    luaL_register(L,libName,mylibs);
    return 1;
}
