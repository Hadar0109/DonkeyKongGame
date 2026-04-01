#include "game.h"
#include "gameModes.h"

#include <string>

int main(int argc, char** argv) {

	bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";
	bool isSave = argc > 1 && std::string(argv[1]) == "-save";
	bool isTest = argc > 1 && std::string(argv[1]) == "-test";
	std::unique_ptr<IGameMode> mode;

	if (isLoad || isTest)
		mode = std::make_unique<ReplayMode>();
	else
		mode = std::make_unique<InteractiveMode>();

	Game g(std::move(mode));
	g.setIsLoad(isLoad || isTest);
	g.setIsSilent(isSilent || isTest);
	g.setIsSave(isSave);
	g.setIsAutoTest(isTest);
	g.run();

	if (isTest) {
		return g.didReplayTestPass() ? 0 : 1;
	}

	return 0;
}
