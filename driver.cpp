#include <iostream>

void Menu()
{
    std::cout << "** Main Menu **\n";
    std::cout << 
}

int main(int argc, char *argv[])
{

    char MenuOption = 'M';
    bool state = true;
    
    do
    {
      	std::cin >> MenuOption;
	
	switch(MenuOption)
	{
	case 'm':
	case 'M':
	    Menu();
	    break;
	case 'q':
	case 'Q':
	    state = false;
	    break;
	default:
	    Menu();
	}
	
    }while(state);
    
    return 0;
}
