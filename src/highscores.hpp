#ifndef HIGHSCORES
#define HIGHSCORES

#include "cScreen.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <list>
using namespace sf;
using namespace std;

class Highscores : public cScreen
{
public:
    Highscores() {}
    virtual int Run(RenderWindow &App);
    vector<Text> setHighscores(string stream);
};

int Highscores::Run(RenderWindow &App)
{
    bool Running = true;
    Event event;
    int menu = 0;
    
    Font font;
    if (!font.loadFromFile("res/mono.ttf"))
    {
        throw "Error loading font res/mono.ttf";
    }
    
    vector<Text> t_entries = setHighscores("res/t_highscores.txt");
    vector<Text> p_entries = setHighscores("res/p_highscores.txt");
    
    
    // Titles
    Text Title1;
    Text Title2;
    
    Title1.setFont(font);
    Title1.setCharacterSize(50);
    Title1.setString("Tetris");
    Title1.setPosition({ 100, 60 });
    
    Title2.setFont(font);
    Title2.setCharacterSize(50);
    Title2.setString("Pentis");
    Title2.setPosition({ 420, 60 });
    
    
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
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case Keyboard::Escape:
                        return (0);
                        break;
                    case Keyboard::Left:
                        menu = max(0, menu-1);
                        break;
                    case Keyboard::Right:
                        menu = min(1, menu+1);
                        break;
                    default:
                        break;
                }
            }
        }
        
        App.clear();
        
        Title1.setFillColor(Color::White);
        Title2.setFillColor(Color::White);
        
        switch (menu)
        {
            case 0:
                Title1.setFillColor(Color::Red);
                for (auto elem : t_entries) { elem.setFont(font); App.draw(elem); }
                break;
            case 1:
                Title2.setFillColor(Color::Red);
                for (auto elem : p_entries) { elem.setFont(font); App.draw(elem); }
                break;
        }
        
        App.draw(Title1);
        App.draw(Title2);
        
        App.display();
		
    }
    
    // This should never happen
    return (-1);
}

vector<Text> Highscores::setHighscores(string stream)
{
    vector<Text> entries;
    Text e1;  entries.push_back(e1);
    Text e2;  entries.push_back(e2);
    Text e3;  entries.push_back(e3);
    Text e4;  entries.push_back(e4);
    Text e5;  entries.push_back(e5);
    Text e6;  entries.push_back(e6);
    Text e7;  entries.push_back(e7);
    Text e8;  entries.push_back(e8);
    Text e9;  entries.push_back(e9);
    Text e10; entries.push_back(e10);
    
    float pos = 210;
    for (auto i = entries.begin(); i < entries.end(); i++) {
        i->setCharacterSize(50);
        i->setPosition({ 100, pos });
        pos += 50;
    }
    
    // Error handling and setup
    ifstream scorefile(stream);
    scorefile.exceptions (ifstream::badbit);
    if (!scorefile.is_open()) throw "Error while opening file";
    
    string scoreline;
    string nameline;
    int j = 0;
    int linesread = 0;
    
    while (getline(scorefile, scoreline)) {
        Text elem = entries[j];
        ++linesread;
        if (linesread % 2 == 0) {
            string dots;
            for (int i = 0; i + scoreline.length() + nameline.length() <= 16; i++) {
                dots += ".";
            }
            elem.setString(nameline + dots + scoreline);
            entries[j] = elem;
            j++;
        }
        else {
            nameline = scoreline;
        }
    }
    
    scorefile.close();
    
    return entries;
}

#endif
