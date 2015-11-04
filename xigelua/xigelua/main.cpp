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

//��C���Ψһ��ں������亯��ǩ����ͬ�������ע�ắ���������¼���˵����
//1. ���ǿ��Խ��ú����򵥵����Ϊģ��Ĺ���������
//2. �亯��������Ϊluaopen_xxx������xxx��ʾlibrary���ơ�Lua����require "xxx"��Ҫ��֮��Ӧ��
//3. ��luaL_register�ĵ����У����һ���ַ�������Ϊģ����"xxx"���ڶ�������Ϊ��ע�ắ�������顣
//4. ��Ҫǿ�����ǣ�������Ҫ�õ�"xxx"�Ĵ��룬����C����Lua�������뱣��һ�£�����Lua��Լ����
//   �����޷����á�
extern "C" __declspec(dllexport)
int luaopen_xigelua(lua_State* L) 
{

	const char* libName = "xigelua";
    luaL_register(L,libName,mylibs);
    return 1;
}
