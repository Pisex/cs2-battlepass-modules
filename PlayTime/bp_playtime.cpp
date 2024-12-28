#include <stdio.h>
#include "bp_playtime.h"
#include "metamod_oslink.h"
#include "schemasystem/schemasystem.h"

BP_PlayTime g_BP_PlayTime;
PLUGIN_EXPOSE(BP_PlayTime, g_BP_PlayTime);
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

bool BP_PlayTime::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool BP_PlayTime::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	
	return true;
}

void BP_PlayTime::AllPluginsLoaded()
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
	g_pUtils->CreateTimer(60.0f, [](){
		for (int i = 0; i < 64; i++)
		{
			if(g_pPlayers->IsFakeClient(i)) continue;
			CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
			if(!pPlayer) continue;
			CCSPlayerPawn* pPlayerPawn = pPlayer->GetPlayerPawn();
			if(!pPlayerPawn) continue;
			int iTeam = pPlayerPawn->GetTeam();
			if(iTeam == 0) continue;
			g_pBPApi->SendCustomEvent(i, "playtime", {}, {{"team", std::to_string(iTeam)}});
		}
		return 60.0f;
	});
}

///////////////////////////////////////
const char* BP_PlayTime::GetLicense()
{
	return "GPL";
}

const char* BP_PlayTime::GetVersion()
{
	return "1.0";
}

const char* BP_PlayTime::GetDate()
{
	return __DATE__;
}

const char *BP_PlayTime::GetLogTag()
{
	return "BP_PlayTime";
}

const char* BP_PlayTime::GetAuthor()
{
	return "Pisex";
}

const char* BP_PlayTime::GetDescription()
{
	return "BP_PlayTime";
}

const char* BP_PlayTime::GetName()
{
	return "[BP] PlayTime";
}

const char* BP_PlayTime::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
