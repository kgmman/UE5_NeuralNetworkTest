// Copyright EVRStudio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkActor.generated.h"

class AGunWeaponItem;

DECLARE_LOG_CATEGORY_EXTERN(LogNeuralNetwork, Log, All);

class Example
{
public:
	VectorND<D> X;
	VectorND<D> Y;
};

UCLASS(BlueprintType)
class NEURALNETWORKTEST_API ANeuralNetworkActor : public AActor
{
	GENERATED_BODY()
public:	
	ANeuralNetworkActor();
public:
	UFUNCTION(BlueprintCallable)
	int SearchAnimIndex(FVector AnimLocation);

	UFUNCTION(BlueprintCallable)
	FVector GetRootMotion(float Time);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION(CallInEditor)
	void StartLearning();

	UFUNCTION(CallInEditor)
	void StopLearning();
protected:
	UPROPERTY(EditAnywhere)
	UAnimSequence* TargetSeq;

	UPROPERTY(VisibleAnywhere)
	float TargetSeqLength;

	UPROPERTY(EditAnywhere)
	int HiddenLayerNum;

	UPROPERTY(EditAnywhere)
	int LearningNum;

	UPROPERTY(EditAnywhere)
	float LearningRate;
protected:
	int CurLearningCount;
	int SegSize;

	bool bStartLearning;

	NeuralNetwork NN;

	Example* SeqData;
};