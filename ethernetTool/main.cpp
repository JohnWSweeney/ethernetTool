#include "ethernetTool.h"

int main()
{
	std::cout << "ethernetTool v0.0.0" << '\n';
	bool running = true;
	std::vector<std::string> tokens;
	while (running)
	{
		getInput(tokens);
		selectThread(running, tokens);
	}
}