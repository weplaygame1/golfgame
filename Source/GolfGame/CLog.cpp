#include "CLog.h"
#include "Engine.h"

CLog::CLog()
{
}

CLog::~CLog()
{
}

void CLog::Log(int val)
{
	GLog->Logf(L"%d", val);
}

void CLog::Log(float val)
{
	GLog->Logf(L"%f", val);
}

void CLog::Log(UINT val)
{
	GLog->Logf(L"%d", val);
}

void CLog::Log(FVector2D val)
{
	GLog->Logf(L"X : %f, Y : %f", val.X, val.Y);
}

void CLog::Log(const FString & val)
{
	GLog->Log(val);
}

void CLog::Log(const TCHAR * val)
{
	GLog->Log(val);
}

void CLog::Log(const FName & val)
{
	GLog->Log(val.ToString());
}

void CLog::LogNull(UObject * object)
{
	GLog->Log(object != NULL ? L"Not Null" : L"Null");
}

void CLog::Print(int val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"%d", val));
}

void CLog::Print(float val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"%f", val));
}

void CLog::Print(UINT val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"%d", val));
}

void CLog::Print(FVector val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"X : %f, Y : %f, Z : %f", val.X, val.Y, val.Z));
}

void CLog::Print(FVector2D val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"X : %f, Y : %f", val.X, val.Y));
}

void CLog::Print(FRotator val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(L"X(Roll) : %f, Y(Pitch) : %f, Z(Yaw) : %f", val.Roll, val.Pitch, val.Yaw));
}

void CLog::Print(const FString & val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, val);
}

void CLog::Print(const TCHAR * val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, val);
}

void CLog::Print(const FName & val, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, val.ToString());
}

void CLog::PrintNull(UObject * object, float time, FColor color, int32 key)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, object != NULL ? "NotNull" : "Null");
}