// Copyright EVRStudio. All Rights Reserved.

#include "NeuralNetworkActor.h"
#include <cmath>

DEFINE_LOG_CATEGORY(LogNeuralNetwork);

ANeuralNetworkActor::ANeuralNetworkActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	HiddenLayerNum = 5;
	LearningRate = 0.01f;
	LearningNum = 10000;
	CurLearningCount = 0;
	bStartLearning = false;
}

void ANeuralNetworkActor::BeginPlay()
{
	Super::BeginPlay();

	if (TargetSeq)
	{
		TargetSeqLength = TargetSeq->GetPlayLength();
		SegSize = int(TargetSeqLength / 0.1f);
		SeqData = new Example[SegSize];

		for (int i = 0; i < SegSize; ++i)
		{
			SeqData[i].X.initialize(3);
			SeqData[i].Y.initialize(1);
		}

		NN.initialize(3, 1, HiddenLayerNum);
		NN.alpha_ = LearningRate;
	}
}

void ANeuralNetworkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartLearning)
	{
		for (int ii = 0; ii < 10; ++ii)
		{
			if (CurLearningCount >= LearningNum)
			{
				bStartLearning = false;
				return;
			}

			float CurTime = 0.f;

			for (int i = 0; i < SegSize; ++i)
			{
				FVector Location = TargetSeq->ExtractRootMotion(0.f, CurTime, false).GetTranslation();
				VectorND<D> y_temp(2);

				SeqData[i].X[0] = Location.X;
				SeqData[i].X[1] = Location.Y;
				SeqData[i].X[2] = Location.Z;
				SeqData[i].Y[0] = CurTime * 10;
				CurTime += 0.1f;

				NN.setInputVector(SeqData[i].X);
				NN.propForward();

				NN.copyOutputVector(y_temp);

				int y = int(round(y_temp[0]));
				int y_ = int(SeqData[i].Y[0]);

				UE_LOG(LogNeuralNetwork, Log, TEXT("%d %d"), y_, y);
				NN.propBackward(SeqData[i].Y);
			}

			UE_LOG(LogNeuralNetwork, Log, TEXT("%d / %d"), CurLearningCount, LearningNum);
			++CurLearningCount;
		}
	}
}

void ANeuralNetworkActor::StartLearning()
{
	bStartLearning = true;
	CurLearningCount = 0;
	NN.alpha_ = LearningRate;
}

void ANeuralNetworkActor::StopLearning()
{
	bStartLearning = false;
}

FVector ANeuralNetworkActor::GetRootMotion(float Time)
{
	return TargetSeq->ExtractRootMotion(0.f, Time, false).GetTranslation();
}

int ANeuralNetworkActor::SearchAnimIndex(FVector AnimLocation)
{
	VectorND<D> LocationInput;
	VectorND<D> Output;

	LocationInput.initialize(3);
	Output.initialize(1);

	LocationInput[0] = AnimLocation.X;
	LocationInput[1] = AnimLocation.Y;
	LocationInput[2] = AnimLocation.Z;

	NN.setInputVector(LocationInput);
	NN.propForward();
	NN.copyOutputVector(Output);

	return int(round(Output[0]));
}