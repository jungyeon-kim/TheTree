#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TheTree, Log, All);

#define TTLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// General log
#define TTLOG_S(Verbosity) UE_LOG(TheTree, Verbosity, TEXT("%s"), *TTLOG_CALLINFO)

// Output log (string must be followed by *)
#define TTLOG(Verbosity, Format, ...) UE_LOG(TheTree, Verbosity, TEXT("%s %s"), *TTLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// If the condition is not true, red log is returned.In a function with a return value, the return value must be given as the second argument.
#define TTCHECK(Expr, ...) { if(!(Expr)) { TTLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }

UENUM()
enum class ECharacterState
{
	INIT,
	LOADING,
	READY,
	NOBATTLE,
	BATTLE,
	DEAD
};

UENUM()
enum class EMontageType
{
	ATTACK, ATTACK_CHARGE, ATTACK_QUAKE, ATTACK_JUMP, 
	ATTACK_SUMMON, ATTACK_DRAIN,
	DEFENSE,
	DODGE, INWEAPON, OUTWEAPON, TELEPORT,
	KNOCKBACK, DEATH
};

// Struct containing instances for global access.
struct FTTWorld
{ 
private:
	FTTWorld() = default;
public:
	static float TimeDilation;
	static bool bIsDebugging;
public:
	// Function to convert string to hashcode
	static constexpr uint32 HashCode(const TCHAR* Str)
	{
		uint32 Hash{};
		while (*Str)
		{
			Hash = 65599 * Hash + Str[0];
			++Str;
		}
		return Hash ^ (Hash >> 16);
	}
};

// Variable for GlobalTimeDilation.
float FTTWorld::TimeDilation{};
// Flag to turn debug mode ON / OFF.
bool FTTWorld::bIsDebugging{};