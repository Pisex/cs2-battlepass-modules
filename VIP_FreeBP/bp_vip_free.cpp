#include <stdio.h>
#include "bp_vip_free.h"
#include "metamod_oslink.h"
#include "schemasystem/schemasystem.h"

bp_vip_free g_bp_vip_free;
PLUGIN_EXPOSE(bp_vip_free, g_bp_vip_free);
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars *gpGlobals = nullptr;

IUtilsApi* g_pUtils;
IPlayersApi* g_pPlayers;
IBattlePassApi* g_pBPApi;
IVIPApi* g_pVIPApi;

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

bool bp_vip_free::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool bp_vip_free::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	
	return true;
}

void bp_vip_free::AllPluginsLoaded()
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
	g_pVIPApi = (IVIPApi *)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing VIP system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pVIPApi->VIP_OnClientLoaded([](int iSlot, bool bIsVIP) {
		if (bIsVIP && g_pVIPApi->VIP_GetClientFeatureBool(iSlot, "bp_pass"))
			g_pBPApi->GiveUserPass(iSlot, false);
	});
}

///////////////////////////////////////
const char* bp_vip_free::GetLicense()
{
	return "GPL";
}

const char* bp_vip_free::GetVersion()
{
	return "1.0";
}

const char* bp_vip_free::GetDate()
{
	return __DATE__;
}

const char *bp_vip_free::GetLogTag()
{
	return "bp_vip_free";
}

const char* bp_vip_free::GetAuthor()
{
	return "Pisex";
}

const char* bp_vip_free::GetDescription()
{
	return "bp_vip_free";
}

const char* bp_vip_free::GetName()
{
	return "[BP][VIP] Free PASS";
}

const char* bp_vip_free::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
