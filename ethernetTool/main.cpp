#include "start.h"

int main()
{
	std::cout << "ethernetTool v1.0.3\n\n";
	bool running = true;
	std::vector<std::string> tokens;
	while (running)
	{
		getInput(tokens);
		startMenu(running, tokens);
	}
}