#include "pch.h"
#include "Time.h"

Dynamo::Time* Dynamo::Time::myInstance = nullptr;

void Dynamo::Time::Create()
{
	myInstance = new Time;
	myInstance->CreateInternal();
}

void Dynamo::Time::CreateInternal()
{
	myTimer = std::make_unique<CU::Timer>();
}

void Dynamo::Time::Destroy()
{
	delete myInstance;
	myInstance = nullptr;
}

void Dynamo::Time::Update()
{
	myInstance->myTimer->Update();
}

float Dynamo::Time::GetDeltaTime()
{
	return myInstance->myTimer->GetDeltaTime();
}

float Dynamo::Time::GetUnscaledDeltaTime()
{
	return myInstance->myTimer->GetUnscaledDeltaTime();
}

float Dynamo::Time::GetTotalTime()
{
	return myInstance->myTimer->GetTotalTime();
}

void Dynamo::Time::StartCallbackEvent(const float aTimer, const std::function<void(void)> aCallbackFunction)
{
	myInstance->myTimer->SetTimer(aTimer, aCallbackFunction, false);
}

void Dynamo::Time::StartUpdateCallbackEvent(const float aTimer, const std::function<void(void)> aCallbackFunction)
{
	myInstance->myTimer->SetUpdateTimer(aTimer, aCallbackFunction, false);
}

void Dynamo::Time::SetTimeScale(const float aTimeScale)
{
	myInstance->myTimer->SetTimeScale(aTimeScale);
}

float Dynamo::Time::GetTimeScale()
{
	return myInstance->myTimer->GetTimeScale();
}
