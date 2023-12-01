#include <iostream>
#include <SFML/Graphics.hpp>
// Basic Screen Class
#include "cScreen.hpp"
// Including each screen of application
#include "menu.hpp"
#include "game.hpp"
#include "highscores.hpp"
using namespace sf;
using namespace std;

int main(void)
{

    // Applications variables
    int tsize = 45;
    int swidth = tsize * 10;
    int sheight = tsize * 20;
    int sideb = tsize*4;
    vector<cScreen*> Screens;
    int screen = 0;
    srand(int(time(NULL)));
    
    // Window creation
    RenderWindow App(VideoMode(swidth + sideb, sheight), "Tetris/Pentis",  Style::Close);
    
	Image icon;
	if (!icon.loadFromFile("res/icon.png"))
	{
		throw "Error loading texture res/icon.png";
	}
	App.setIcon(150, 150, icon.getPixelsPtr());

    // Screens-object preparations
    Menu *menu = new Menu();
    Screens.push_back(menu);
    Game *game = new Game(0);
    Screens.push_back(game);
    Highscores *t_highscores = new Highscores();
    Screens.push_back(t_highscores);
    
    
    /* Main loop with exeption handling
    and appropriate memory management */
    try {
        while (screen >= 0)
        {
            if (screen == 3)
            {
                // start clean tetris
                delete Screens[1];
                Game *game = new Game(0);
                Screens[1] = game;
                screen = 1;
            }
            if (screen == 4)
            {
                // start clean pentis
                delete Screens[1];
                Game *game = new Game(1);
                Screens[1] = game;
                screen = 1;
            }
            if (screen == 5)
            {
                delete Screens[0];
                Menu *menu = new Menu();
                Screens[0] = menu;
                screen = 0;
            }
            screen = Screens[screen]->Run(App);
        }
    } catch(string msg) {
        for (auto elem : Screens) {
            delete elem;
        }
        cerr << msg << endl;
    } catch (ifstream::failure e) {
        cerr << "Error while reading file" << endl;
    }
    
    // Calling the default destructor of the base class cScreen
    for (auto elem : Screens) {
        delete elem;
    }
    
    return EXIT_SUCCESS;
}

