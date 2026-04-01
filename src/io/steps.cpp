#include "steps.h"
#include "utils.h"

Steps Steps::loadSteps(const std::string& filename) 
{
	Steps steps;
	std::ifstream steps_file(filename);

	if (!steps_file.good()) 
		printMissingAuxFileMessage("steps", filename);
	else 
	{
		steps_file >> steps.randomSeed;
		size_t size;
		steps_file >> size;
		steps.goodFile = true;

		while (!steps_file.eof() && size-- != 0) 
		{
			size_t iteration;
			char step;
			steps_file >> iteration >> step;
			steps.addStep(iteration, step);
		}

		steps_file.close();
	}

	return steps;
}

void Steps::saveSteps(const std::string& filename) const 
{
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();

	for (const auto& step : steps)
	{
		steps_file << '\n' << step.first << ' ' << step.second;
	}

	steps_file.close();
}

char Steps::popStep() 
{
	char step = steps.front().second;
	steps.pop_front();

	return step;
}

void Steps::clearStepsList() 
{
	steps.clear();
	goodFile = false;
}
