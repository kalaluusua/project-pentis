#ifndef DYNAMICS
#define DYNAMICS

#include "thread"
#include <ctgmath>

vector<Color> COLORS = {Color::White, Color::Red, Color::Green, Color::Blue,
                         Color::Yellow, Color::Magenta, Color::Cyan };

bool Game::spamMove(Clock &c, int &t, Keyboard::Key key, bool &wasPressed) {
	bool move = false;
	if (Keyboard::isKeyPressed(key) && (c.getElapsedTime() > milliseconds(t) || !wasPressed)) {
		move = true;
		c.restart();
		if (!wasPressed) t = delay;
		else t = spamSpeed;
	}
	else if (!Keyboard::isKeyPressed(key)) c.restart();
	if (!Keyboard::isKeyPressed(key)) wasPressed = false;
	else wasPressed = true;

	return move;
}

bool Game::nonSpamMove(Keyboard::Key key, bool &wasPressed) {
	bool move = false;
	if (Keyboard::isKeyPressed(key) && !wasPressed) move = true;
	if (!Keyboard::isKeyPressed(key)) wasPressed = false;
	else wasPressed = true;

	return move;
}


/* All timed functions: Gravity, Piece Freezing,
   Fast drop handling and spawning new piece */
bool Game::timingFreezingEnding(Clock &clock, RectangleShape tile, RectangleShape mid, RenderWindow &app, Sprite grid, Texture gridtexture, Text t1, Text t2, Text n1, Text n2) {
	// Return value
	bool rowsWereRemoved = false; 
	// while (time to move piece down or spacebar was pressed)
	while (clock.getElapsedTime() > milliseconds(1000 - Speed) || fastDrop) {
		tile.move(0, TSIZE);
		if (fastDrop) Points++;
		updatePosition(tile, p);
		// If piece can't be moved down freeze it
		if (!isValid()) {
			tile.move(0, -TSIZE);
			if (fastDrop) Points--;
			updatePosition(tile, p);
            for (auto i : pentoCoord) {
                Grid[i[1]][i[0]] = p.getColor() + 1;
                Points++;
            }
			// Spawn new piece
			tile.setPosition(4 * TSIZE, 0);
			p = randPentomino();
			fastDrop = false;
			updatePosition(tile, p);

			// Guarantee that piece spawns on top row
			bool isTop = false;
			while (!isTop) {
				for (auto i : pentoCoord) if (i[1] == 0) isTop = true;
				if (!isTop) {
					tile.move(0, -TSIZE);
					updatePosition(tile, p);
				}
			}

			// End game logic: if new piece spawns on freezed piece -> Game Over
			for (auto i : pentoCoord) {
				if (Grid[i[1]][i[0]] != 0) {
					Running = false;
					break;
				}
			}
		}
		// Check if any rows are full and restart main clock
		clock.restart();
		if (removeFullRows(tile, mid, app, grid, gridtexture, t1, t2, n1, n2)) rowsWereRemoved = true;
	}
	return rowsWereRemoved;
}

void Game::drawPieces(RectangleShape tile, RectangleShape mid, RenderWindow &app) {
	// Draw static tiles
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (Grid[y][x] > 0)
			{
				tile.setPosition(x * TSIZE, y * TSIZE);
				mid.setPosition((x * TSIZE) + 1, (y * TSIZE) + 1);
				tile.setFillColor(COLORS[Grid[y][x] - 1]);
				app.draw(tile);
				app.draw(mid);

			}
		}
	}

	// Draw moving piece
	for (auto i : pentoCoord)
	{
		tile.setPosition(i[0] * TSIZE, i[1] * TSIZE);
		mid.setPosition((i[0] * TSIZE) + 1, (i[1] * TSIZE) + 1);
		tile.setFillColor(COLORS[p.getColor()]);
		app.draw(tile);
		app.draw(mid);
	}
}

bool Game::removeFullRows(RectangleShape tile, RectangleShape mid, RenderWindow &app, Sprite grid, Texture gridtexture, Text t1, Text t2, Text n1, Text n2) {
	// First make tempRow for animation
	RectangleShape tempRow(Vector2f(TSIZE * 10, TSIZE));
	tempRow.setTexture(&gridtexture);
	tempRow.setTextureRect(sf::IntRect(0, 0, 500, 50));
	app.draw(grid);
	app.draw(t1);
	app.draw(t2);
	app.draw(n1);
	app.draw(n2);
	drawPieces(tile, mid, app);
	int rowsRemoved = 0;
	// If row has 10 tiles freezed replace it with the row above it
	for (int row = 0; row < 20; row++) {
		int tileCount = 0;
		for (int col = 0; col < 10; col++) {
			if (Grid[row][col] != 0) {
				tileCount++;
			}
		}
		if (tileCount == 10) {
			tempRow.setPosition(0, row * TSIZE);
			app.draw(tempRow);
			for (int rowToCpy = row; rowToCpy > 0; rowToCpy--) {
				for (int colToCopy = 0; colToCopy < 10; colToCopy++) {
					Grid[rowToCpy][colToCopy] = Grid[rowToCpy - 1][colToCopy];
				}
			}
			rowsRemoved++;
		}
		tileCount = 0;
	}
	if (rowsRemoved) {
		// Increase remove count in objects privates
		removes++;
		// Increase speed only for tetris
		if (!_gameMode) Speed = Speed + speedUp - removes;
		Points += pow(2, rowsRemoved) * 10;
		app.display();
		// Freeze screen for a moment to gice dropping effect 
		this_thread::sleep_for(chrono::milliseconds(1000 - Speed));
		return true;
	}
	else return false;
}

// Helper function to check if new pentoCoord are valid
// aka not out of bounds or on top of static blocks
bool Game::isValid()
{
    for (auto i : pentoCoord)
    {
        if (i[0] < 0 || i[0] > 9) return false;
        if (i[1] < 0 || i[1] > 19) return false;
		if (Grid[i[1]][i[0]] > 0) return false;
    }
    return true;
}

bool Game::wallKick(sf::RectangleShape &tile, Pentomino p, int tmpRotation) {
    
    auto newRotation = p.getRotation();
    
    // tmp position to fall back to if isValid is false
    auto tmpPos = tile.getPosition();
    
    int bSymbol = p.getSymbol();
    Matrix m;
    // Tetromino wall kick data from http://tetris.wikia.com/wiki/SRS
    // Pentomino wall kick data derived using https://pbs.twimg.com/media/DX83TS-WAAAAzeN.jpg:large
    // and http://tetris.wikia.com/wiki/Pentomino

	vector<char> exceptions = { 'I', 'O', 'V', 'U', 'W', 'X', 'i', 'l', ',', '.' };
	bool isException = find(exceptions.begin(), exceptions.end(), bSymbol) != exceptions.end();

    if (!isException) { // Not I, O, V, U, W, X, i, l, ',' or '.'
        if (tmpRotation == 0 && newRotation == 1)      m = { {-1, 0},    {-1,-1},    { 0, 2},    {-1, 2} };
        else if (tmpRotation == 1 && newRotation == 0) m = { { 1, 0},    { 1, 1},    { 0,-2},    { 1,-2} };
        else if (tmpRotation == 1 && newRotation == 2) m = { { 1, 0},    { 1, 1},    { 0,-2},    { 1,-2} };
        else if (tmpRotation == 2 && newRotation == 1) m = { {-1, 0},    {-1,-1},    { 0, 2},    {-1, 2} };
        else if (tmpRotation == 2 && newRotation == 3) m = { { 1, 0},    { 1,-1},    { 0, 2},    { 1, 2} };
        else if (tmpRotation == 3 && newRotation == 2) m = { {-1, 0},    {-1, 1},    { 0,-2},    {-1,-2} };
        else if (tmpRotation == 3 && newRotation == 0) m = { {-1, 0},    {-1, 1},    { 0,-2},    {-1,-2} };
        else if (tmpRotation == 0 && newRotation == 3) m = { { 1, 0},    { 1,-1},    { 0, 2},    { 1, 2} };
    }
    else if (bSymbol == 'I') { // Is I
        if (tmpRotation == 0 && newRotation == 1)      m = { {-2, 0},    { 1, 0},    {-2, 1},    { 1,-2} };
        else if (tmpRotation == 1 && newRotation == 0) m = { { 2, 0},    {-1, 0},    { 2,-1},    {-1, 2} };
        else if (tmpRotation == 1 && newRotation == 2) m = { {-1, 0},    { 2, 0},    {-1,-2},    { 2, 1} };
        else if (tmpRotation == 2 && newRotation == 1) m = { { 1, 0},    {-2, 0},    { 1, 2},    {-2,-1} };
        else if (tmpRotation == 2 && newRotation == 3) m = { { 2, 0},    {-1, 0},    { 2,-1},    {-1, 2} };
        else if (tmpRotation == 3 && newRotation == 2) m = { {-2, 0},    { 1, 0},    {-2, 1},    { 1,-2} };
        else if (tmpRotation == 3 && newRotation == 0) m = { { 1, 0},    {-2, 0},    { 1, 2},    {-2,-1} };
        else if (tmpRotation == 0 && newRotation == 3) m = { {-1, 0},    { 2, 0},    {-1,-2},    { 2, 1} };
    }
	else if (bSymbol == 'i') { // Is i
		if (tmpRotation == 0 && newRotation == 1)      m = { {-1, 0},    { 1, 0},    {-1, 1},    { 1,-1} };
		else if (tmpRotation == 1 && newRotation == 0) m = { { 1, 0},    {-1, 0},    { 1,-1},    {-1, 1} };
		else if (tmpRotation == 1 && newRotation == 2) m = { {-1, 0},    { 1, 0},    {-1,-1},    { 1, 1} };
		else if (tmpRotation == 2 && newRotation == 1) m = { { 1, 0},    {-1, 0},    { 1, 1},    {-1,-1} };
		else if (tmpRotation == 2 && newRotation == 3) m = { { 1, 0},    {-1, 0},    { 1,-1},    {-1, 1} };
		else if (tmpRotation == 3 && newRotation == 2) m = { {-1, 0},    { 1, 0},    {-1, 1},    { 1,-1} };
		else if (tmpRotation == 3 && newRotation == 0) m = { { 1, 0},    {-1, 0},    { 1, 1},    {-1,-1} };
		else if (tmpRotation == 0 && newRotation == 3) m = { {-1, 0},    { 1, 0},    {-1,-1},    { 1, 1} };
	}
    else { // Is O, V, U, W, X, l, ',' or '.'
        if      (tmpRotation == 0 && newRotation == 1) m = { { 0, 1} };
        else if (tmpRotation == 1 && newRotation == 0) m = { { 0,-1} };
        else if (tmpRotation == 1 && newRotation == 2) m = { {-1, 0} };
        else if (tmpRotation == 2 && newRotation == 1) m = { { 1, 0} };
        else if (tmpRotation == 2 && newRotation == 3) m = { { 0,-1} };
        else if (tmpRotation == 3 && newRotation == 2) m = { { 0, 1} };
        else if (tmpRotation == 3 && newRotation == 0) m = { { 1, 0} };
        else if (tmpRotation == 0 && newRotation == 3) m = { {-1, 0} };
    }
    
    for (auto i : m) {
        int adjX = i[0];
        int adjY = i[1];
        tile.move(adjX*TSIZE, adjY*TSIZE);
        
        updatePosition(tile, p);
        
        if (isValid())
            return true;
        
        // Reset tile position
        tile.setPosition(tmpPos);
    }
    
    return false;
}

// Helper function to update position and pentoCoord
// according to tiles position and pentoblock in question
void Game::updatePosition(sf::RectangleShape &tile, Pentomino p)
{
    curX = tile.getPosition().x / TSIZE;
    curY = tile.getPosition().y / TSIZE;
    pentoCoord = p.translatePentomino(curX, curY);
}

// Get a tetromino symbol from the tetroPermutation vector (possibly refill first)
char Game::randTetromino(map<char, pair<int, Matrix>> m) {
	if (tetroPermutation.empty())  // if empty
	{
		// Initialize the vector holding the permutation
		vector<char> permutation;
		for (auto const& x : m)
			permutation.push_back(x.first);  // symbol (key)

		// Shuffle the symbols
		random_shuffle(permutation.begin(), permutation.end());
		// Store permutation
		tetroPermutation = permutation;
	}
	char nextSymbol = tetroPermutation.back();
	// Remove the last element
	tetroPermutation.pop_back();
	return nextSymbol;
}

// Function for retrieving a random Pentomino
Pentomino Game::randPentomino() {
    blockMap b;
	map<char, pair<int, Matrix>> m;
    
	if (getGameMode() == 0)
	{
		// Get a tetromino from the tetroPermutation vector
		m = b.tetrominoes;
		return Pentomino(m, randTetromino(m));
	}
	else
	{	
		// Initial probability of Pentominoes in percentages
		int pentoProb = 0;
		
		// Evenly distributed random probability
		int i = rand() % 100;

		// Probability to get a pentomino increases with the increasing amount of removeFullRows-calls
		// percInc percent increase every rmvInc removes
		int percInc = 5;
		int rmvInc = 5;
		if ( i < pentoProb + percInc*(getRemoves()/rmvInc) ) // Player gets a pentomino
			m = b.pentominoes;
		else {
			// Mono-, do-, tro- and tetrominoes are distributed such that the player
			// is 2^n times as likely to get a block of size n than a monomino:

			// Coefficient * Collection sizes
			auto m1s = 1*b.monominoes.size();	// 1*1
			auto m2s = 2*b.dominoes.size();		// 2*1
			auto m3s = 4*b.trominoes.size();	// 4*2
			auto m4s = 8*b.tetrominoes.size();	// 8*7

			unsigned int i = rand() % (m1s + m2s + m3s + m4s);

			if (i < m1s)
				m = b.monominoes;
			else if (m1s <= i && i < (m1s + m2s))
				m = b.dominoes;
			else if ((m1s + m2s) <= i && i < (m1s + m2s + m3s))
				m = b.trominoes;
			else {
				// Get a tetromino from the tetroPermutation vector
				m = b.tetrominoes;
				return Pentomino(m, randTetromino(m));
			}
		}
	}
    
	// Get random block from the selected map
    auto it = m.begin();
    advance(it, rand() % m.size());
    char randSymbol = it->first; // Get the first stored value
    return Pentomino(m, randSymbol);
}

#endif
