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

void Game::drawPieces(RectangleShape tile, RectangleShape mid, RenderWindow &app, const vector<int>& rowsToSkip) {
    // Draw static tiles
    for (int y = 0; y < 20; y++) {
        if (find(rowsToSkip.begin(), rowsToSkip.end(), y) != rowsToSkip.end()) {
            continue; // Skip the rows specified in rowsToSkip
        }
        for (int x = 0; x < 10; x++) {
            if (Grid[y][x] > 0) {
                tile.setPosition(x * TSIZE, y * TSIZE);
                mid.setPosition((x * TSIZE) + 1, (y * TSIZE) + 1);
                tile.setFillColor(COLORS[Grid[y][x] - 1]);
                app.draw(tile);
                app.draw(mid);
            }
        }
    }

    // Draw moving piece
    for (auto i : pentoCoord) {
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

    vector<int> fullRows;
    int rowsRemoved = 0;

    // Identify full rows
    for (int row = 0; row < 20; row++) {
        int tileCount = 0;
        for (int col = 0; col < 10; col++) {
            if (Grid[row][col] != 0) {
                tileCount++;
            }
        }
        if (tileCount == 10) {
            fullRows.push_back(row);
        }
    }

    // Redraw the grid without the full rows
    if (!fullRows.empty()) {
        app.clear(); // Clear the window for redrawing
        app.draw(grid);
        app.draw(t1);
        app.draw(t2);
        app.draw(n1);
        app.draw(n2);
        drawPieces(tile, mid, app, fullRows); // Draw pieces, skipping the full rows
        app.display();
        this_thread::sleep_for(chrono::milliseconds(500)); // Pause to show the grid before rows disappear
    }

    // Remove full rows and update grid
    for (int fullRow : fullRows) {
        for (int rowToCpy = fullRow; rowToCpy > 0; rowToCpy--) {
            for (int colToCopy = 0; colToCopy < 10; colToCopy++) {
                Grid[rowToCpy][colToCopy] = Grid[rowToCpy - 1][colToCopy];
            }
        }
        rowsRemoved++;
    }

    // Redraw the grid with the pieces in their new positions
    if (rowsRemoved > 0) {
        app.clear(); // Clear the window for redrawing
        app.draw(grid);
        app.draw(t1);
        app.draw(t2);
        app.draw(n1);
        app.draw(n2);
        drawPieces(tile, mid, app); // Redraw the pieces in their new positions
        app.display();
        this_thread::sleep_for(chrono::milliseconds(500 - Speed)); // Pause for the drop effect
    }

    // Update game state after rows have dropped
    if (rowsRemoved > 0) {
        removes++;
        if (!_gameMode) Speed = Speed + speedUp - removes;
        Points += pow(2, rowsRemoved) * 10;
        return true;
    } else {
        return false;
    }
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
    auto tmpPos = tile.getPosition();
    int bSymbol = p.getSymbol();

    // Define test positions for J, L, T, S, Z Tetrominoes
    std::map<std::pair<int, int>, std::vector<std::vector<int>>> standardTests = {
        {{0, 1}, {{-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
        {{1, 0}, {{1, 0}, {1, -1}, {0, 2}, {1, 2}}},
        {{1, 2}, {{1, 0}, {1, -1}, {0, 2}, {1, 2}}},
        {{2, 1}, {{-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
        {{2, 3}, {{1, 0}, {1, 1}, {0, -2}, {1, -2}}},
        {{3, 2}, {{-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
        {{3, 0}, {{-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
        {{0, 3}, {{1, 0}, {1, 1}, {0, -2}, {1, -2}}}
    };

    // Define test positions for I Tetromino
    std::map<std::pair<int, int>, std::vector<std::vector<int>>> ITetrominoTests = {
        {{0, 1}, {{-2, 0}, {1, 0}, {-2, -1}, {1, 2}}},
        {{1, 0}, {{2, 0}, {-1, 0}, {2, 1}, {-1, -2}}},
        {{1, 2}, {{-1, 0}, {2, 0}, {-1, 2}, {2, -1}}},
        {{2, 1}, {{1, 0}, {-2, 0}, {1, -2}, {-2, 1}}},
        {{2, 3}, {{2, 0}, {-1, 0}, {2, 1}, {-1, -2}}},
        {{3, 2}, {{-2, 0}, {1, 0}, {-2, -1}, {1, 2}}},
        {{3, 0}, {{1, 0}, {-2, 0}, {1, -2}, {-2, 1}}},
        {{0, 3}, {{-1, 0}, {2, 0}, {-1, 2}, {2, -1}}}
    };

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> iTrominoTests = {
        {{0, 1}, {{-1, 0}, {1, 0}, {-1, 1}, {1, -1}}},
        {{1, 0}, {{1, 0}, {-1, 0}, {1, -1}, {-1, 1}}},
        {{1, 2}, {{-1, 0}, {1, 0}, {-1, -1}, {1, 1}}},
        {{2, 1}, {{1, 0}, {-1, 0}, {1, 1}, {-1, -1}}},
        {{2, 3}, {{1, 0}, {-1, 0}, {1, -1}, {-1, 1}}},
        {{3, 2}, {{-1, 0}, {1, 0}, {-1, 1}, {1, -1}}},
        {{3, 0}, {{1, 0}, {-1, 0}, {1, 1}, {-1, -1}}},
        {{0, 3}, {{-1, 0}, {1, 0}, {-1, -1}, {1, 1}}}
    };

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> otherTests = {
        {{0, 1}, {{0, 1}}},
        {{1, 0}, {{0, -1}}},
        {{1, 2}, {{-1, 0}}},
        {{2, 1}, {{1, 0}}},
        {{2, 3}, {{0, -1}}},
        {{3, 2}, {{0, 1}}},
        {{3, 0}, {{1, 0}}},
        {{0, 3}, {{-1, 0}}}
    };


    // Select the appropriate test based on the current rotation and symbol
    std::vector<std::vector<int>>* currentTests = nullptr;

    vector<char> exceptions = { 'I', 'O', 'V', 'U', 'W', 'X', 'i', 'l', ',', '.' };
	bool isException = find(exceptions.begin(), exceptions.end(), bSymbol) != exceptions.end();

    if (!isException) {
        currentTests = &standardTests[{tmpRotation, newRotation}];
    }
    else if (bSymbol == 'I') {
        currentTests = &ITetrominoTests[{tmpRotation, newRotation}];
    } else if (bSymbol == 'i') {
        currentTests = &iTrominoTests[{tmpRotation, newRotation}];
    } else {
        currentTests = &otherTests[{tmpRotation, newRotation}];
    }

    // Perform the tests
    if (currentTests) {
        for (auto& test : *currentTests) {
            int adjX = test[0];
            int adjY = test[1];
            tile.move(adjX*TSIZE, adjY*TSIZE);

            updatePosition(tile, p);

            if (isValid()) {
                return true;
            }

            // Reset tile position
            tile.setPosition(tmpPos);
        }
    }

    // Additional tests for Pentominoes
    if (p.getPentominoSize() == 5) {
        std::vector<std::vector<int>> additionalTests = {
            {0, 0}, {1, 0}, {-1, 0}, {2, 0}, {-2, 0}, {0, 1}
        };

        for (auto& test : additionalTests) {
            int adjX = test[0];
            int adjY = test[1];
            tile.move(adjX*TSIZE, adjY*TSIZE);

            updatePosition(tile, p);

            if (isValid()) {
                return true;
            }

            // Reset tile position
            tile.setPosition(tmpPos);
        }
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
