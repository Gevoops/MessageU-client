#include "Menu.h"
#include <iostream>

Menu::Menu() {

}

void Menu::run()
{
	int choice;
	printMenu();
	std::cin >> choice;
	handleChoice(choice);
}

void Menu::printMenu()
{
	std::cout << "\n" <<
		"MessageU client at your service." << std::endl << std::endl <<
		"110) Register" << std::endl <<
		"120) Request for clients list" << std::endl <<
		"130) Request for public key" << std::endl <<
		"140) Request for waiting messages" << std::endl <<
		"150) Send a text message" << std::endl <<
		"151) Send a request for symmetric key" << std::endl <<
		"152) Send your symmetric key" << std::endl <<
		"0) Exit client" << std::endl << "?" << std::endl;
}

void Menu::handleChoice(int choice)
{

}


