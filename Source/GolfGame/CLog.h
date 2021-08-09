#pragma once
#include "CoreMinimal.h"

/** ����ڿ��� �ʿ��� ������ ����Ѵ�. */
#define CLOG_FUNC					UE_LOG(LogTemp, Warning, TEXT("Called : %s(%d)"), TEXT(__FUNCTION__), __LINE__);
#define CLOG_FUNC_TEXT(s)			UE_LOG(LogTemp, Warning, TEXT("Called : %s(%d) : %s"), TEXT(__FUNCTION__), __LINE__, *FString(s));
#define CLOG_ERROR_FUNC				UE_LOG(LogTemp, Error, TEXT("Called : %s(%d)"), TEXT(__FUNCTION__), __LINE__);
#define CLOG_ERROR_FUNC_TEXT(s)		UE_LOG(LogTemp, Error, TEXT("Called : %s(%d) : %s"), TEXT(__FUNCTION__), __LINE__, *FString(s));

/** ġ������ ���� �˻� �� ����Ѵ�. */
#define CLOG_ERROR_CHECK(p)\
{\
	if (p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT("NULL")\
	}\
}
#define CLOG_ERROR_CHECK_TEXT(p, s)\
{\
	if (p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT(s)\
	}\
}

/** ������ nullptr �˻� �� ����Ѵ�. */
#define CLOG_CHECK(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT("NULL");\
	}\
}
#define CLOG_CHECK_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT(s);\
	}\
}

/**
 * ������ nullptr �˻� �� ���, ���������� �Լ��� ���� ��Ų��.
 * ���Ǵ� �Լ����� ��ȯ �ڷ����� �����ϸ�, ����� �� ����.
 * ��ȯ���� void�� �Լ������� ����� �� �ִ�.
 */
#define CLOG_CHECK_RETURN(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT("NULL");\
		return;\
	}\
}
#define CLOG_CHECK_RETURN_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT(s);\
		return;\
	}\
}
#define CLOG_ERROR_CHECK_RETURN(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT("NULL");\
		return;\
	}\
}
#define CLOG_ERROR_CHECK_RETURN_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT(s);\
		return;\
	}\
}

class GOLFGAME_API CLog
{
public:
	CLog();
	~CLog();

	static void Log(int val);
	static void Log(float val);
	static void Log(UINT val);
	static void Log(FVector2D val);
	static void Log(const FString& val);
	static void Log(const TCHAR* val);
	static void Log(const FName& val);
	static void LogNull(UObject* object);

	static void Print(int val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(float val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(UINT val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FVector val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FVector2D val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FRotator val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FString& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const TCHAR* val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FName& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void PrintNull(UObject* object, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
};
