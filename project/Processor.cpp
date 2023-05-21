#include "Processor.h"

Processor::Processor()
{
	//RUN_Process = new Process;
	Pstate = run;

}

Processor::~Processor()
{
}

void Processor::ScheduleAlgo()
{
}





int Processor::getstoptime()
{
	return Stoptime;
}

void Processor::setstoptime(int t)
{
	Stoptime = t;
}

bool Processor::processor_overheated()
{
	static std::mt19937 gen(time(nullptr)); // Initialize random number generator
	std::uniform_real_distribution<> dis(0.0, 1.0);

	double random_number = dis(gen);
	return random_number <= OVERHEAT_PROBABILITY;
}

void Processor::setprocessorstate(ProcessorState p)
{
	Pstate = p;
}

ProcessorState Processor::getprocessorstate()
{
	return Pstate;
}
