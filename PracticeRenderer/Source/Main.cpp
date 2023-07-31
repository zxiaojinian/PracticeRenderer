#include "Core/Application.h"

int main(int argc, char** argv)
{
	auto app = new PR::Application();
	app->Run();
	delete app;
}
