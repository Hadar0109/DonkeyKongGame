#include "results.h"
#include "utils.h"

#include <string>

Results Results::loadResults(const std::string& filename) 
{
	Results results;
	std::ifstream results_file(filename);

	if (!results_file.good()) 
		printMissingAuxFileMessage("results", filename);
	else 
	{
		size_t colorMode;
		results_file >> colorMode;
		size_t size;
		results_file >> size;
		results.goodFile = true;

		while (!results_file.eof() && size-- != 0) 
		{
			size_t iteration;
			int result;
			results_file >> iteration >> result;
			results.addResult(iteration, static_cast<ResultValue>(result));
		}
	}

	return results;
}

void Results::saveResults(const std::string& filename, int score, bool colorModeEnabled) const 
{
	std::ofstream results_file(filename);
	results_file << colorModeEnabled << '\n';
	results_file << results.size();

	for (const auto& result : results) 
	{
		results_file << '\n' << result.first << ' ' << (int)result.second;
	}

	results_file << '\n' << score;
	results_file.close();
}

size_t Results::getNextBombIteration() const 
{
	if (!results.empty() && results.front().second == ResultValue::DISQUALIFIED) 
		return results.front().first;
	else 
		return (std::numeric_limits<size_t>::max)(); // a big number we will never reach
}

size_t Results::getNextPaulineIteration() const 
{
	if (!results.empty() && results.front().second == ResultValue::FINISHED) 
		return results.front().first;
	else 
		return (std::numeric_limits<size_t>::max)(); // a big number we will never reach
}

std::string Results::buildResultErrorText(const std::string& messageToScreen,
	const std::string& message,
	const std::string& filename,
	size_t iteration,
	int score) 
{
	std::string text;
	text += "Screen " + filename + '\n';
	text += messageToScreen + '\n';
	text += message + '\n';
	text += "Iteration: " + std::to_string(iteration) + '\n';
	text += "You scored: " + std::to_string(score) + "\n\n";
	text += "Press any key to continue to next screens (if any)\n";

	return text;
}

int Results::getColorMode(const std::string& filename) 
{
	std::ifstream results_file(filename);
	int colorModeEnabled;
	results_file >> colorModeEnabled;

	return colorModeEnabled;
}

void Results::clearResultsList() 
{
	results.clear();
	goodFile = false;
}

std::pair<size_t, ResultValue> Results::popResult() 
{
	if (results.empty())
		return { 0, ResultValue::NO_RESULT };

	auto result = results.front();
	results.pop_front();

	return result;
}

bool Results::isReachedPauline() 
{
	
	if (!results.empty() && results.back().second == ResultValue::FINISHED)
		return true;
	else 
		return false;
}

