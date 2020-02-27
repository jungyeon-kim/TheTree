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

//Struct containing instances for debugging
struct FTTDebug 
{ 
	static bool bIsDebugging; 
};

// Flag to turn debug mode ON / OFF.
bool FTTDebug::bIsDebugging{};

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