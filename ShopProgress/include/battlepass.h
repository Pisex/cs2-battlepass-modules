#pragma once

#include <functional>
#include <string>

class IMySQLConnection;

struct Task
{
	const char* szIdentifier;
	const char* szName;
	const char* szDescription;
	const char* szEvent;
	std::map<std::string, std::string> vecParamsString;
	std::map<std::string, std::string> vecParamsInt;
	std::string szCount;
	int iNeeded;
	int iEXP;
	bool bPaid;
};

struct CategoryTask
{
	std::string szName;
	int iTime;
	int iCountGive;
	std::map<std::string, Task> vecTasks;
};

struct Progress
{
	int iID;
	std::string sMission;
	int iProgress;
	int iNeeded;
	int iExpires;
};

struct UserData
{
	int iID;
	int iLevel;
	int iEXP;
	bool bPaid;
	std::map<std::string, Progress> vecProgress;
};

struct Season
{
	int iID;
	std::string sName;
	int iStartDate;
	int iEndDate;
};

typedef std::function<void()> OnCoreLoadCallback;

#define BP_INTERFACE "IBattlePassApi"
class IBattlePassApi
{
public:
	virtual IMySQLConnection* GetConnection() = 0;
    virtual std::map<int, int> GetLevels() = 0;

    virtual Season GetSeason() = 0;
    virtual std::vector<CategoryTask> GetTasks() = 0;
    virtual Task GetTask(const char* szIdentifier) = 0;
    virtual UserData GetUser(int iSlot) = 0;
    virtual void SendCustomEvent(int iSlot, const char* szEvent, std::map<std::string, std::string> vecParamsString, std::map<std::string, std::string> vecParamsInt) = 0;

	virtual bool IsCoreLoaded() = 0;
	virtual void HookOnCoreLoad(SourceMM::PluginId id, OnCoreLoadCallback callback) = 0;
};