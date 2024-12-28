#include <stdio.h>
#include "bp_shop_rcon.h"
#include "metamod_oslink.h"
#include "schemasystem/schemasystem.h"

bp_shop_rcon g_bp_shop_rcon;
PLUGIN_EXPOSE(bp_shop_rcon, g_bp_shop_rcon);
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars *gpGlobals = nullptr;

IUtilsApi* g_pUtils;
IPlayersApi* g_pPlayers;
IBattlePassApi* g_pBPApi;

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
}

void StartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
	gpGlobals = g_pUtils->GetCGlobalVars();
}

bool bp_shop_rcon::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool bp_shop_rcon::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	
	return true;
}

void ReplaceString(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void OnSendShopType(int iSlot, const char* szType, std::map<std::string, std::string> vecParams)
{
	if(!strcmp(szType, "rcon"))
	{
		std::string szCommand = vecParams["command"];
		ReplaceString(szCommand, "{userid}", std::to_string(iSlot));
		char szSteamID[64];
		g_SMAPI->Format(szSteamID, sizeof(szSteamID), "%lld", g_pPlayers->GetSteamID64(iSlot));
		ReplaceString(szCommand, "{steamid64}", szSteamID);
		engine->ServerCommand(szCommand.c_str());
	}
}

void bp_shop_rcon::AllPluginsLoaded()
{
	char error[64];
	int ret;
	g_pUtils = (IUtilsApi *)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing Utils system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pPlayers = (IPlayersApi *)g_SMAPI->MetaFactory(PLAYERS_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing Players system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pBPApi = (IBattlePassApi *)g_SMAPI->MetaFactory(BP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing BattlePass system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pBPApi->HookSendShopType(g_PLID, OnSendShopType);
}

///////////////////////////////////////
const char* bp_shop_rcon::GetLicense()
{
	return "GPL";
}

const char* bp_shop_rcon::GetVersion()
{
	return "1.0";
}

const char* bp_shop_rcon::GetDate()
{
	return __DATE__;
}

const char *bp_shop_rcon::GetLogTag()
{
	return "bp_shop_rcon";
}

const char* bp_shop_rcon::GetAuthor()
{
	return "Pisex";
}

const char* bp_shop_rcon::GetDescription()
{
	return "bp_shop_rcon";
}

const char* bp_shop_rcon::GetName()
{
	return "[BP][SHOP] RCON";
}

const char* bp_shop_rcon::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
