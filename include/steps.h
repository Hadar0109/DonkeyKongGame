#pragma once
#include <list>
#include <fstream>
#include <iostream>

class Steps 
{
	long randomSeed = 0;
	std::list<std::pair<size_t, char>> steps; // (iteration, key)
	bool goodFile = false;
public:
	// File format:
	// randomSeed
	// count
	// <iteration> <key>
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const { return randomSeed; }
	void setRandomSeed(long seed) { randomSeed = seed; }
	void addStep(size_t iteration, char step) { steps.push_back({ iteration, step }); }
	char popStep();
	bool isNextStepOnIteration(size_t iteration) const {

		return !steps.empty() && steps.front().first == iteration;
	}
	void clearStepsList();
	bool getGoodFile() const { return goodFile; }
};

