#ifndef GAME
#define GAME

#include "cScreen.hpp"
#include "pentomino.hpp"
using namespace std;
using namespace sf;
typedef std::vector<std::vector<int>> Matrix;

class Game : public cScreen
{
public:
    Game(int gameMode) { _gameMode = gameMode; }
    virtual int Run(RenderWindow &App);
    Pentomino randPentomino();
	// Special random rules hold for tetrominoes (see tetropermutation variable)
	char randTetromino(map<char, pair<int, Matrix>> m);
    bool isValid();
    void updatePosition(RectangleShape &tile, Pentomino p);
    bool wallKick(RectangleShape &tile, Pentomino p, int prevRotation);
	int getGameMode() const { return _gameMode; }
	int getRemoves() const { return removes; }
	void drawPieces(RectangleShape tile, RectangleShape mid, RenderWindow &app);
	bool removeFullRows(RectangleShape tile, RectangleShape mid, RenderWindow & app, Sprite grid, Texture gridtexture, Text t1, Text t2, Text n1, Text n2);
	bool timingFreezingEnding(Clock &clock, RectangleShape tile, RectangleShape mid, RenderWindow &app, Sprite grid, Texture gridtexture, Text t1, Text t2, Text n1, Text n2);
	bool spamMove(Clock &c, int &t, Keyboard::Key key, bool &wasPressed);
	bool nonSpamMove(Keyboard::Key key, bool &wasPressed);
    int endGame(RenderWindow &App);

private:
    int TSIZE = 50;  // WIDTH OF GAME TILE IN PIXELS
    bool Running = false;
    int Grid[20][10] = {0}; // Grid dimensions
    int Points = 0;
    int _gameMode;   // 0 = tetris, 1 = pentis
    Pentomino p;
    int curX;
    int curY;
    Matrix pentoCoord;
    int end = 0;
    bool fastDrop = false;

	// A vector holding a permutation of all 7 tetrominoes
	// with the corresponding practical max length of 7
	vector<char> tetroPermutation;
    
    // Difficulty
	int removes = 0;      // how many times rows have been removed
    int Speed = 500; // time loop = 1000ms - startSpeed
    int speedUp = 35;     // how much game speeds up when lines are removed

	// Controls
	bool DownPressed = false;
	bool LeftPressed = false;
	bool RightPressed = false;
	bool UpPressed = false;
	bool SpacePressed = false;
	bool LCtrlPressed = false;
	int delay = 200;       // Delay in moving fast when keeping left, right and down pressed
	int spamSpeed = 30;    // the fast moving speed
};

#endif
