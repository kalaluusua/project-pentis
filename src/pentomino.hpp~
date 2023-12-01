#ifndef PENTOMINO
#define PENTOMINO

#include <vector>
#include <map>
using namespace std;

typedef vector<vector<int>> Matrix;

// Pentis blocks
// Contains maps that hold the blocks and corresponding symbols and colors
// (source for coloring: http://harddrop.com/file/pic/gallery/1083.jpg)
struct blockMap {

	map<char, pair<int, Matrix>> monominoes =
	{
		{
			'.',
			{3,	{
				{ 1 }
			}	}
		}
	};

	map<char, pair<int, Matrix>> dominoes =
	{
		{
			',',
			{1,	{
				{ 0, 0 },
				{ 1, 1 }
			}	}
		}
	};

	map<char, pair<int, Matrix>> trominoes =
	{
		{
			'i',
			{0,	{
				{ 0, 0, 0 },
				{ 1, 1, 1 },
				{ 0, 0, 0 }
			}	}
		},
		{
			'l',
			{1,	{
				{ 1, 0 },
				{ 1, 1 }
			}	}
		}
	};

	map<char, pair<int, Matrix>> tetrominoes =
	{
		{
			'I',
			{0,	{
				{ 0, 0, 0, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'J',
			{1,	{
				{ 1, 0, 0 },
				{ 1, 1, 1 },
				{ 0, 0, 0 }
			}	}
		},
		{
			'L',
			{2, {
				{ 0, 0, 1 },
				{ 1, 1, 1 },
				{ 0, 0, 0 }
			}	}
		},
		{
			'O',
			{3,	{
				{ 1, 1 },
				{ 1, 1 }
			}	}
		},
		{
			'S',
			{4,	{
				{ 0, 1, 1 },
				{ 1, 1, 0 },
				{ 0, 0, 0 }
			}	}
		},
		{
			'T',
			{5,	{
				{ 0, 1, 0 },
				{ 1, 1, 1 },
				{ 0, 0, 0 }
			}	}
		},
		{
			'Z',
			{6,	{
				{ 1, 1, 0 },
				{ 0, 1, 1 },
				{ 0, 0, 0 }
			}	}
		}
	};

	// Pentominoes labeled using naming convention from https://en.wikipedia.org/wiki/Pentomino#/media/File:Pentomino_Naming_Conventions.svg
	map<char, pair<int, Matrix>> pentominoes =
	{
		{
			'I',
			{0,	{
				{ 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0 },
				{ 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0 }
			}	}
		},
		{
			'l',
			{1,	{
				{ 0, 0, 0, 0 },
				{ 1, 0, 0, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'L',
			{2,	{
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 1 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'V',
			{3,	{
				{ 0, 0, 1 },
				{ 0, 0, 1 },
				{ 1, 1, 1 }
			}	}
		},
		{	'n',
			{4,	{
				{ 0, 0, 0, 0 },
				{ 0, 0, 1, 1 },
				{ 1, 1, 1, 0 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'N',
			{6,	{
				{ 0, 0, 0, 0 },
				{ 1, 1, 0, 0 },
				{ 0, 1, 1, 1 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'Y',
			{5,	{
				{ 0, 0, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'y',
			{5,	{
				{ 0, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 }
			}	}
		},
		{
			'T',
			{5,	{
				{ 0, 1, 0 },
				{ 0, 1, 0 },
				{ 1, 1, 1 }
			}	}
		},
		{
			'P',
			{1,	{
				{ 0, 0, 0 },
				{ 1, 1, 0 },
				{ 1, 1, 1 }
			}	}
		},
		{
			'p',
			{2,	{
				{ 0, 0, 0 },
				{ 0, 1, 1 },
				{ 1, 1, 1 }
			}	}
		},
		{
			'U',
			{3,	{
				{ 1, 0, 1 },
				{ 1, 1, 1 },
				{ 0, 0, 0 }
			}	}
		},
		{	'F',
			{4,	{
				{ 1, 0, 0 },
				{ 1, 1, 1 },
				{ 0, 1, 0 }
			}	}
		},
		{
			'f',
			{6,	{
				{ 0, 0, 1 },
				{ 1, 1, 1 },
				{ 0, 1, 0 }
			}	}
		},
		{	'W',
			{3,	{
				{ 0, 0, 1 },
				{ 0, 1, 1 },
				{ 1, 1, 0 }
			}	}
		},
		{
			'Z',
			{4,	{
				{ 0, 0, 1 },
				{ 1, 1, 1 },
				{ 1, 0, 0 }
			}	}
		},
		{
			'z',
			{6,	{
				{ 1, 0, 0 },
				{ 1, 1, 1 },
				{ 0, 0, 1 }
			}	}
		},
		{
			'X',
			{3,	{
				{ 0, 1, 0 },
				{ 1, 1, 1 },
				{ 0, 1, 0 }
			}	}
		}
	};
};


class Pentomino {
public:
	Pentomino(std::map<char, pair<int,Matrix>> blocks, char blockSymbol)
		: _pentomino(blocks[blockSymbol].second), _rotation(0), _color(blocks[blockSymbol].first), _symbol(blockSymbol) { }

	// Default constructor
    Pentomino() {}

	// Function rotates n x n Matrix 90 degrees clockwise
	void rotate90deg();

	// Function rotates n x n Matrix 90 degrees counter-clockwise
	void rotateNeg90deg();

	// Function prints n x n Matrix to standard output:
	// e.g. when n = 3:
	// 1 2 3 \n
	// 1 2 3 \n
	// 1 2 3 \n
	void print(Matrix const& m);

	size_t getPentominoSize() const;

	Matrix translatePentomino(int x, int y);
    
    int getRotation() const { return _rotation; }
    
    void setRotation(int rotation);
    
    int getColor() const { return _color; } 

	char getSymbol() const { return _symbol; }

private:
	Matrix _pentomino;
    int _rotation;
    int _color;
	char _symbol;
};

#endif
