#include "Scheduler.h"

int main() {
	Scheduler* scheduler = new Scheduler();
	scheduler->readInputFile("input.txt");
	scheduler->simulate();
	//delete scheduler;
	return 0;
}