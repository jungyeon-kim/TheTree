#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TheTree, Log, All);
#define TTLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// 일반 로그
#define TTLOG_S(Verbosity) UE_LOG(TheTree, Verbosity, TEXT("%s"), *TTLOG_CALLINFO)
// 출력 로그 (문자열은 *이 붙어야합니다.)
#define TTLOG(Verbosity, Format, ...) UE_LOG(TheTree, Verbosity, TEXT("%s %s"), *TTLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
// 조건이 참이 아닌경우 붉은 로그를 띄우고 리턴합니다. 반환 값이 있는 함수에서는 두번째인자로 반환값을 부여해야합니다.
#define TTCHECK(Expr, ...) { if(!(Expr)) { TTLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }