#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TheTree, Log, All);
#define TTLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// �Ϲ� �α�
#define TTLOG_S(Verbosity) UE_LOG(TheTree, Verbosity, TEXT("%s"), *TTLOG_CALLINFO)
// ��� �α� (���ڿ��� *�� �پ���մϴ�.)
#define TTLOG(Verbosity, Format, ...) UE_LOG(TheTree, Verbosity, TEXT("%s %s"), *TTLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
// ������ ���� �ƴѰ�� ���� �α׸� ���� �����մϴ�. ��ȯ ���� �ִ� �Լ������� �ι�°���ڷ� ��ȯ���� �ο��ؾ��մϴ�.
#define TTCHECK(Expr, ...) { if(!(Expr)) { TTLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }