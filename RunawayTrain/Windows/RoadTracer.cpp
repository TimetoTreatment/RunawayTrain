#include "RoadTracer.h"

RoadTracer* RoadTracer::sInstance = nullptr;


RoadTracer* RoadTracer::Instance()
{
	if (sInstance == nullptr)
		sInstance = new RoadTracer;

	return sInstance;
}

void RoadTracer::Release()
{
	delete sInstance;
	sInstance = nullptr;
}