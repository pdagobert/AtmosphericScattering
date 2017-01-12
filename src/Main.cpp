#include "Application.hpp"
#include <exception>
#include <iostream>

int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch( std::exception& e )
	{
		std::cout << "exception caught" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
}