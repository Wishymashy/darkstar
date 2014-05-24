/*
===========================================================================

Copyright (c) 2010-2014 Darkstar Dev Teams

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

This file is part of DarkStar-server source code.

===========================================================================
*/

#include "../../common/showmsg.h"

#include "lua_instance.h"

#include "lua_baseentity.h"


/************************************************************************
*																		*
*  Constructor															*
*																		*
************************************************************************/

CLuaInstance::CLuaInstance(lua_State *L)
{
	if (!lua_isnil(L, -1))
	{
		m_PLuaInstance = (CInstance*)(lua_touserdata(L, -1));
		lua_pop(L, 1);
	}
	else{
		m_PLuaInstance = NULL;
	}
}

/************************************************************************
*																		*
*  Constructor															*
*																		*
************************************************************************/

CLuaInstance::CLuaInstance(CInstance* PBattlefield)
{
	m_PLuaInstance = PBattlefield;
}

inline int32 CLuaInstance::setLevelCap(lua_State* L)
{
	DSP_DEBUG_BREAK_IF(m_PLuaInstance == NULL);
	DSP_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

	m_PLuaInstance->SetLevelCap(lua_tonumber(L, 1));

	return 0;
}

inline int32 CLuaInstance::getChars(lua_State* L)
{
	DSP_DEBUG_BREAK_IF(m_PLuaInstance == NULL);

	lua_createtable(L, m_PLuaInstance->m_charList.size(), 0);
	int8 newTable = lua_gettop(L);
	int i = 1;
	for (auto member : m_PLuaInstance->m_charList)
	{
		lua_getglobal(L, CLuaBaseEntity::className);
		lua_pushstring(L, "new");
		lua_gettable(L, -2);
		lua_insert(L, -2);
		lua_pushlightuserdata(L, (void*)member.second);
		lua_pcall(L, 2, 1, 0);

		lua_rawseti(L, -2, i++);
	}

	return 1;
}

inline int32 CLuaInstance::getTimeLimit(lua_State* L)
{
	DSP_DEBUG_BREAK_IF(m_PLuaInstance == NULL);

	lua_pushinteger(L, m_PLuaInstance->GetTimeLimit());

	return 1;
}

inline int32 CLuaInstance::getEntryPos(lua_State* L)
{
	lua_createtable(L, 4, 0);
	int8 newTable = lua_gettop(L);

	position_t entry = m_PLuaInstance->GetEntryLoc();

	lua_pushnumber(L, entry.x);
	lua_setfield(L, newTable, "x");

	lua_pushnumber(L, entry.y);
	lua_setfield(L, newTable, "y");

	lua_pushnumber(L, entry.z);
	lua_setfield(L, newTable, "z");

	lua_pushnumber(L, entry.rotation);
	lua_setfield(L, newTable, "rot");

	return 1;
}

/************************************************************************
*																		*
*  declare lua function													*
*																		*
************************************************************************/

const int8 CLuaInstance::className[] = "CInstance";
Lunar<CLuaInstance>::Register_t CLuaInstance::methods[] =
{
	LUNAR_DECLARE_METHOD(CLuaInstance, setLevelCap),
	LUNAR_DECLARE_METHOD(CLuaInstance, getChars),
	LUNAR_DECLARE_METHOD(CLuaInstance, getTimeLimit),
	LUNAR_DECLARE_METHOD(CLuaInstance, getEntryPos),
	{ NULL, NULL }
};