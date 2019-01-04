#include <iostream>
#include <SFML/Graphics.hpp>
#include "cScreen.hpp"
using namespace sf;
using namespace std;

class Menu : public cScreen
{
private:
    bool playing;
public:
    Menu() {playing = false;}
    virtual int Run(RenderWindow &App);
};

int Menu::Run(RenderWindow &App)
{
    bool Running = true;
    Event event;
    
    Text Menu1;
    Text Menu2;
    Text Menu3;
    Text Menu4;
    Text Menu5;
    Text Menu6;
    int menu = 0;
    
    Font font;
    if (!font.loadFromFile("res/mono.ttf"))
    {
        throw "Error loading font res/mono.ttf";
    }
    
    
    // Alternate between
    Menu1.setFont(font);
    Menu1.setCharacterSize(50);
    Menu1.setString("Tetris");
    Menu1.setPosition({ 100, 160 });
    
    Menu2.setFont(font);
    Menu2.setCharacterSize(50);
    Menu2.setString("Continue");
    Menu2.setPosition({ 100, 160 });
    
    // Alternate between
    Menu3.setFont(font);
    Menu3.setCharacterSize(50);
    Menu3.setString("Pentis");
    Menu3.setPosition({ 100, 240 });
    
    Menu4.setFont(font);
    Menu4.setCharacterSize(50);
    Menu4.setString("Main menu");
    Menu4.setPosition({ 100, 240 });
    
    // Permanent
    Menu5.setFont(font);
    Menu5.setCharacterSize(50);
    Menu5.setString("Highscore");
    Menu5.setPosition({ 100, 360 });
    
    Menu6.setFont(font);
    Menu6.setCharacterSize(50);
    Menu6.setString("Exit");
    Menu6.setPosition({ 100, 440 });
    
    
    while (Running)
    {
        // Verifying events
        while (App.pollEvent(event))
        {
            // Window closed
            if (event.type == Event::Closed)
            {
                return (-1);
            }
            // Key pressed
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case Keyboard::Up:
                        menu = max(0, menu-1);
                        break;
                    case Keyboard::Down:
                        menu = min(3, menu+1);
                        break;
                    case Keyboard::Return:
                        switch (menu)
                    {
                        case 0:
                            if (playing)
                            {
                                return (1);
                            }
                            else
                            {
                                playing = true;
                                // reset game and start tetris
                                return (3);
                            }
                        case 1:
                            if (playing)
                            {
                                playing = false;
                                return (0);
                            }
                            else
                            {
                                playing = true;
                                // reset game and start pentis
                                return (4);
                            }
                        case 2:
                            return (2);
                        case 3:
                            return (-1);
                    }
                    default:
                        break;
                }
            }
        }
        
        Menu1.setFillColor(Color::White);
        Menu2.setFillColor(Color::White);
        Menu3.setFillColor(Color::White);
        Menu4.setFillColor(Color::White);
        Menu5.setFillColor(Color::White);
        Menu6.setFillColor(Color::White);
        
        switch (menu)
        {
            case 0:
                Menu1.setFillColor(Color::Red);
                Menu2.setFillColor(Color::Red);
                break;
            case 1:
                Menu3.setFillColor(Color::Red);
                Menu4.setFillColor(Color::Red);
                break;
            case 2:
                Menu5.setFillColor(Color::Red);
                break;
            case 3:
                Menu6.setFillColor(Color::Red);
                break;
        }
        
        // Draw
        App.clear();
        
        if (!playing)
        {
            App.draw(Menu1);
            App.draw(Menu3);
        }
        else
        {
            App.draw(Menu2);
            App.draw(Menu4);
        }
        App.draw(Menu5);
        App.draw(Menu6);
        
        App.display();
    }
    
    // This should never happen
    return (-1);
}
