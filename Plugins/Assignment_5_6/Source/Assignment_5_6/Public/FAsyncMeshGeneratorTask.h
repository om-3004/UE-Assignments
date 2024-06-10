// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

class AMeshGenerator;

class ASSIGNMENT_5_6_API FAsyncMeshGeneratorTask : public FNonAbandonableTask
{
public:
	FAsyncMeshGeneratorTask(AMeshGenerator* InMeshGenerator);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(AsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	AMeshGenerator* MeshGenerator;

};
