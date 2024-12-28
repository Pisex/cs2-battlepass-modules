#include <stdio.h>
#include "bp_lr_progress.h"
#include "metamod_oslink.h"
#include "schemasystem/schemasystem.h"

bp_lr_progress g_bp_lr_progress;
PLUGIN_EXPOSE(bp_lr_progress, g_bp_lr_progress);
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars *gpGlobals = nullptr;

ILRApi* g_pLRApi;
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

bool bp_lr_progress::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool bp_lr_progress::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	
	return true;
}

void bp_lr_progress::AllPluginsLoaded()
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
	g_pLRApi = (ILRApi *)g_SMAPI->MetaFactory(LR_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing LR system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pLRApi->HookOnExpChangedPre(g_PLID, [](int iSlot, int &iGiveExp)
	{
		g_pBPApi->SendCustomEvent(iSlot, "lr_exp", {}, {{"count", std::to_string(iGiveExp)}});
	});
	g_pUtils->StartupServer(g_PLID, StartupServer);
}

///////////////////////////////////////
const char* bp_lr_progress::GetLicense()
{
	return "GPL";
}

const char* bp_lr_progress::GetVersion()
{
	return "1.0";
}

const char* bp_lr_progress::GetDate()
{
	return __DATE__;
}

const char *bp_lr_progress::GetLogTag()
{
	return "bp_lr_progress";
}

const char* bp_lr_progress::GetAuthor()
{
	return "Pisex";
}

const char* bp_lr_progress::GetDescription()
{
	return "bp_lr_progress";
}

const char* bp_lr_progress::GetName()
{
	return "[BP][LR] Progress";
}

const char* bp_lr_progress::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
