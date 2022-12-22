// Copyright EVRStudio. All Rights Reserved.

#include "NeuralNetworkActor.h"

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
		SegSize = int(TargetSeqLength / 0.2f) + 1;
		SeqData = new Example[SegSize];

		for (int i = 0; i < SegSize; ++i)
		{
			SeqData[i].X.initialize(3);
			SeqData[i].Y.initialize(1);
		}

		NN.initialize(3, 1, HiddenLayerNum);
		NN.alpha_ = LearningRate;

		//for (int c = 0; c < LearningNum; ++c)
		//{
		//	float CurTime = 0.f;

		//	for (int i = 0; i < SegSize; ++i)
		//	{
		//		FVector Location = TargetSeq->ExtractRootMotion(0.f, CurTime, false).GetTranslation();
		//		VectorND<D> y_temp(2);
		//		SeqData[i].X[0] = Location.X;
		//		SeqData[i].X[1] = Location.Y;
		//		SeqData[i].X[2] = Location.Z;
		//		SeqData[i].Y[0] = CurTime;
		//		CurTime += 0.2f;

		//		NN.setInputVector(SeqData[i].X);
		//		NN.propForward();

		//		NN.copyOutputVector(y_temp);

		//		double y = y_temp[0];
		//		double y_ = SeqData[i].Y[0];

		//		UE_LOG(LogNeuralNetwork, Log, TEXT("%lf %lf"), y_, y);
		//		NN.propBackward(SeqData[i].Y);
		//	}

		//	UE_LOG(LogNeuralNetwork, Log, TEXT(" "));
		//}
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
				SeqData[i].Y[0] = CurTime;
				CurTime += 0.2f;

				NN.setInputVector(SeqData[i].X);
				NN.propForward();

				NN.copyOutputVector(y_temp);

				double y = y_temp[0];
				double y_ = SeqData[i].Y[0];

				UE_LOG(LogNeuralNetwork, Log, TEXT("%lf %lf"), y_, y);
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