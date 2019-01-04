#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "game.hpp"
#include "pentomino.hpp"
#include "gameDynamics.hpp"
#include "thread"
#include "endgame.hpp"
using namespace sf;
using namespace std;

// Run function called in main.cpp
int Game::Run(RenderWindow &App)
{
	Event evnt;
	int width = TSIZE * 10;   // width of game field
	int initX = 4;	  // initial X-coordinate
	int initY = 0;	  // initial X-coordinate
	int charSize = 40;


	// Rectangle tile

	RectangleShape tile(Vector2f(TSIZE, TSIZE));
	RectangleShape mid(Vector2f(TSIZE - 2, TSIZE - 2)); // Helper tile for achieving apparent block transparency
	mid.setFillColor(Color(0, 0, 0, 70));


	// Check if comming from continue button
	if (!Running)
	{
		Running = true;
		tile.setPosition(initX*TSIZE,  initY*TSIZE);
		p = randPentomino();
		updatePosition(tile, p);
	}
	else
	{
		tile.setPosition(curX*TSIZE, curY*TSIZE);
	}

	// Text
	Font font;
    if (!font.loadFromFile("res/mono.ttf"))
    {
        throw "Error loading font res/mono.ttfs";
    }

	//Grid
	Texture grid;
	if (!grid.loadFromFile("res/grid.png"))
	{
		throw "Error loading texture res/grid.png";
	}
	Sprite gridsprite;
	gridsprite.setTexture(grid);


	Text textPoints;
	textPoints.setFont(font);
    textPoints.setCharacterSize(charSize);
	textPoints.setPosition(width + 12, 200);
	textPoints.setString("Points:");
	textPoints.setFillColor(Color::White);

	Text textRemoves;
	textRemoves.setFont(font);
    textRemoves.setCharacterSize(charSize);
	textRemoves.setPosition(width + 12, 320);
	textRemoves.setString("Removes:");
	textRemoves.setFillColor(Color::White);

	Text numberPoints;
	numberPoints.setFont(font);
    numberPoints.setCharacterSize(charSize);
	numberPoints.setPosition(width + 12, 260);
	numberPoints.setString("0");
	numberPoints.setFillColor(Color::White);

	Text numberRemoves;
	numberRemoves.setFont(font);
    numberRemoves.setCharacterSize(charSize);
	numberRemoves.setPosition(width + 12, 380);
	numberRemoves.setString("0");
	numberRemoves.setFillColor(Color::White);

    // Game clock and c for longpressclock
    Clock clock;
 
	// Player movement
	Clock cleft; int tleft = delay;
	Clock cright; int tright = delay;
	Clock cdown; int tdown = delay;

	// Souunds
	SoundBuffer dropbuffer;
	if (!dropbuffer.loadFromFile("res/drop.wav")) {
		throw "Error loading res/drop.wav";
	}
	Sound dropSound;
	dropSound.setBuffer(dropbuffer);

	SoundBuffer endbuffer;
	if (!endbuffer.loadFromFile("res/end.wav")) {
		throw "Error loading res/end.wav";
	}
	Sound endSound;
	endSound.setBuffer(endbuffer);

	SoundBuffer removebuffer;
	if (!removebuffer.loadFromFile("res/remove.wav")) {
		throw "Error loading res/click.wav";
	}
	Sound removeSound;
	removeSound.setBuffer(removebuffer);

    ////////////////// Game Loop //////////////////
    while (Running) {
        
        // tmp position and rotation to fall back to if !isValid is true
        auto tmpPos = tile.getPosition();
        auto tmpRotation = p.getRotation();
        
		// Handle window closing and esc/pause
		while (App.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) return (-1);
			if (evnt.key.code == Keyboard::Escape) return 0;
		}
		/////// Player Movement ///////
		// move left
		if (spamMove(cleft, tleft, Keyboard::Left, LeftPressed)) {
			tile.move(-TSIZE, 0);
			updatePosition(tile, p);
		}
		// move right
		if (spamMove(cright, tright, Keyboard::Right, RightPressed)) {
			tile.move(TSIZE, 0);
			updatePosition(tile, p);
		}
		// move down
		if (spamMove(cdown, tdown, Keyboard::Down, DownPressed)) {
			tile.move(0, TSIZE);
			updatePosition(tile, p);
		}
		// fast drop
		if (nonSpamMove(Keyboard::Space, SpacePressed)) {
			fastDrop = true;
			dropSound.play();
		}
		// rotate clockwise
		if (nonSpamMove(Keyboard::Up, UpPressed)) {
			p.rotate90deg();
			updatePosition(tile, p);
		}
		// rotate anti-clockwise
		if (nonSpamMove(Keyboard::LControl, LCtrlPressed)) {
			p.rotateNeg90deg();
			updatePosition(tile, p);
		}
        

		// Wall kick
		if (tmpRotation != p.getRotation() && !isValid())
			wallKick(tile, p, tmpRotation);

		// If update not valid, return to tmpPos and tmpRotation
		if (!isValid())
		{
			tile.setPosition(tmpPos);
			p.setRotation(tmpRotation);
			updatePosition(tile, p);
		}

		/* All timed functions: Gravity, Piece Freezing,
		   Fast drop handling and spawning new piece */
		if (timingFreezingEnding(clock, tile, mid, App, gridsprite, grid, textRemoves, textPoints, numberPoints, numberRemoves)) {
			removeSound.play();
		}

		//////////// DRAW //////////////
		App.clear();
		App.draw(gridsprite);
		numberPoints.setString(to_string(Points));
		numberRemoves.setString(to_string(removes));
		App.draw(textRemoves);
		App.draw(textPoints);
		App.draw(numberPoints);
		App.draw(numberRemoves);
		drawPieces(tile, mid, App);

		// Return to original position
		tile.setPosition(curX * TSIZE, curY * TSIZE);

		App.display();

		if (Running == false) {
			endSound.play();
			this_thread::sleep_for(chrono::milliseconds(1000));
			break;
		}
	}

	// Game ended, read user input and return to main menu

	int endvalue = endGame(App);

	return endvalue;
}
