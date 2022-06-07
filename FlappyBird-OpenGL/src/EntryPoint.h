#pragma once
#include "Application.h"

int main(void)
{
	Application& app = Application::GetInstance();
	app.Run();

	return 0;
}
