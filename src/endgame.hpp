#ifndef ENDGAME
#define ENDGAME

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <list>
using namespace sf;
using namespace std;

unsigned int eolUnix = 10, eolNonUnix = 13, charSize = 35;

int Game::endGame(RenderWindow &App)
{
	Running = true;

	string input;
    string stream;
    
    if (_gameMode == 0)
    {
        stream = "res/t_highscores.txt";
    }
    else
    {
        stream = "res/p_highscores.txt";
    }

	Font font;
    if (!font.loadFromFile("res/mono.ttf"))
    {
        throw "Error loading font res/mono.ttf";
    }

    Text textdidntmake;
    textdidntmake.setFont(font);
    textdidntmake.setCharacterSize(charSize);
    textdidntmake.setPosition(70, charSize);
    textdidntmake.setString("Not in the top 10...");
    textdidntmake.setFillColor(Color::White);

	Text textAskname;
	textAskname.setFont(font);
    textAskname.setCharacterSize(charSize);
	textAskname.setPosition(50, 30);
	textAskname.setString("You're in the top 10!");
	textAskname.setFillColor(Color::White);

    Text score;
    score.setFont(font);
    score.setPosition(130, 80);
    string points = "Your score: ";
    points += to_string(Points);
    score.setString(points);
    score.setCharacterSize(charSize);
    score.setFillColor(Color::Red);
    
    Text name;
    name.setFont(font);
    name.setCharacterSize(charSize);
    name.setPosition(100, 375);
    name.setString("Name:");
    name.setFillColor(Color::White);
    
    Text textUnderline;
    textUnderline.setFont(font);
    textUnderline.setPosition(250, 400);
    textUnderline.setString("---------");
    textUnderline.setCharacterSize(charSize);
    textUnderline.setFillColor(Color(110,110,110));
    
    Text textinput;
    textinput.setFont(font);
    textinput.setPosition(250, 375);
    textinput.setString(input);
    textinput.setCharacterSize(charSize);
    textinput.setFillColor(Color::White);
    
	Text textEnter;
	textEnter.setFont(font);
	textEnter.setPosition(180, 630);
	textEnter.setString("Enter to continue");
	textEnter.setFillColor(Color::White);

	Text textEscape;
	textEscape.setFont(font);
	textEscape.setPosition(200, 670);
	textEscape.setString("Escape to leave");
	textEscape.setFillColor(Color::White);

	string highslist;
	Text texthslist;
	texthslist.setFont(font);
	texthslist.setPosition(100, 210);
	texthslist.setString(highslist);
	texthslist.setCharacterSize(charSize);
	texthslist.setFillColor(Color::White);

	string posmarker;
	Text textposmarker;
	textposmarker.setFont(font);
	textposmarker.setString("*");
	textposmarker.setCharacterSize(28);
	textposmarker.setFillColor(Color::Red);

	Text texttop10;
    texttop10.setString("Tetris TOP10:");
    if (_gameMode == 1) texttop10.setString("Pentis TOP10:");
    texttop10.setFont(font);
    texttop10.setCharacterSize(50);
    texttop10.setPosition(150, 140);
    texttop10.setFillColor(Color::White);
 

	string scoreline;
	string nameline;
	string defaultname = "Unknown";
	list<pair<int,string>> listofscores;
	int linesread = 0;
	bool isEmpty = false;

    // Error handling and setup
    ifstream scorefile(stream);
    scorefile.exceptions (ifstream::badbit);
    if (!scorefile.is_open()) throw "Error while opening file";
    
    if (scorefile.peek() == std::ifstream::traits_type::eof())
    {
        isEmpty = true;
    }
    else {
        while (getline(scorefile, scoreline)) {
            ++linesread;
            if (linesread % 2 == 0) {
                listofscores.push_back(make_pair(stoi(scoreline), nameline));
            }
            else nameline = scoreline;
        }
        scorefile.close();
    }

	bool madetop10 = false;
	bool pushedenter = false;

	//set madetop10 flag
	if (isEmpty) {
		madetop10 = true;
	}
	else {
		listofscores.sort();
		listofscores.reverse();
		if (Points > listofscores.back().first || listofscores.size() < 10) madetop10 = true;
	}

	if (!madetop10) {
		//create string from the list of highscores to output to the user
		for (auto v : listofscores) {
			highslist += v.second;
			for (int i = 0; i + v.second.length() + to_string(v.first).length() <= 16; i++) {
				highslist += ".";
			}
			highslist += to_string(v.first);
			highslist += "\n";
		}
		texthslist.setString(highslist);
	}

	while (Running) {

		Event ev;

		while (App.pollEvent(ev)) {
			//if window closed, close the program
			if (ev.type == Event::Closed)
			{
				Running = false;
				return (-1);
			}
			//if esc pressed, return to main menu
			if (ev.key.code == Keyboard::Escape) {
				Running = false;
				return (5);
			}

			if (ev.type == Event::TextEntered)
			{
				if (madetop10) {
					//if backspace pressed and inputfield isn't empty and made it to top10 and enter haven't been pressed
					//delete character from inputfield
					if (ev.text.unicode == 8 && input.size() != 0 && !pushedenter) {
						input.pop_back();
						textinput.setString(input);
					}
					//input characters according to the given limits
					//(no more chars than 9, made it to top10, enter haven't been pressed)
					if (ev.text.unicode < 128 && ev.text.unicode != 8 && ev.text.unicode != 27 && input.size() < 9 && !pushedenter && ev.text.unicode != 32 && ev.text.unicode != eolUnix && ev.text.unicode != eolNonUnix && ev.text.unicode != 9) {
						input.push_back((char)ev.text.unicode);
						textinput.setString(input);
					}

					//on press of enter save score and return to main menu
					if ((ev.text.unicode == eolUnix && !pushedenter) || (ev.text.unicode == eolNonUnix && !pushedenter)) {
						if (input.empty()) {
							input += defaultname;
						}
						// Error handling and setup
						ofstream scorefile(stream, ios::trunc);
						scorefile.exceptions(ifstream::badbit);
						if (!scorefile.is_open()) throw "Error while writing to file";

						//add contents of the file to list of pairs list(pair<points, name>)
						//sort the list to descending order and delete the member with least points if list size >10
						//write the new highscores to the scorefile
						listofscores.push_back(make_pair(Points, input));
						listofscores.sort();
						listofscores.reverse();
						if (listofscores.size() > 10) listofscores.pop_back();
						for (auto v : listofscores)
							scorefile << v.second << endl << v.first << endl;

						scorefile.close();

						//find players position on top10 list
						int position = 0;
						for (auto it : listofscores) {
							if (it.first == Points && it.second == input) {
								break;
							}
							position++;
						}
						//set position of "*" indicator
						textposmarker.setPosition(75, 230 + (position * 49));

						//create string from the list of highscores to output to the user
						for (auto v : listofscores) {
							highslist += v.second;
							for (int i = 0; i + v.second.length() + to_string(v.first).length() <= 16; i++) {
								highslist += ".";
							}
							highslist += to_string(v.first);
							highslist += "\n";
						}
						texthslist.setString(highslist);
						pushedenter = true;
						list<pair<int, string>> listofscores;
					}
				}
				
			}
		}

		// draw the contents according to the scene
		App.clear();
        
        if (madetop10)
        {
            if (pushedenter)
            {
				App.draw(textposmarker);
                App.draw(texttop10);
                App.draw(texthslist);
                App.draw(textposmarker);
            }
            else
            {
                App.draw(name);
                App.draw(textUnderline);
                App.draw(textAskname);
                App.draw(textinput);
                App.draw(score);
                App.draw(textEnter);
            }
        }
        else
        {
            App.draw(texttop10);
            App.draw(texthslist);
            App.draw(textdidntmake);
            App.draw(score);
        }
        App.draw(textEscape);
        App.display();

	}
    return (-1);
}

#endif
