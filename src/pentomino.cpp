#include <iostream>
#include <vector>
#include "pentomino.hpp"
#include <algorithm>
using namespace std;


// Rotates the Pentomino 90 degree and returns the previous orientation
void Pentomino::rotate90deg() {
	Matrix m = _pentomino;
	auto n = m.size();
	Matrix mr(n, std::vector<int>(n));
	for (vector<vector<int>>::size_type i = 0; i < n; i++)
	{
		for (vector<int>::size_type j = 0; j < n; j++)
		{
			mr[j][n - 1 - i] = m[i][j];
		}
	}
	_pentomino = mr;
    _rotation = (_rotation+1)%4;
}

void Pentomino::rotateNeg90deg() {
	for (size_t i = 0; i < 3; i++) rotate90deg();
}

// Print function for debugging purposes
void Pentomino::print(Matrix const& m) {
	std::cout << "Printing out a " << m.size() << " x " << m.size() << " Matrix:" << std::endl;
	for (auto i : m)
	{
		for (auto j : i)
		{
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}
}

// Get the amount of tiles in the block
size_t Pentomino::getPentominoSize() const {
	Matrix m = _pentomino;
	size_t sum = 0;
	for (auto v : m) {
		sum += count(v.begin(), v.end(), 1);
	}
	return sum;
}

// Presents the Pentomino as collection of (x,y)-coordinates in relation to the given grid location
Matrix Pentomino::translatePentomino(int x, int y) {
	Matrix m = _pentomino;
	Matrix translated;
	int i = 0, j = 0;
	for (auto v : m) {
		for (auto e : v) {
			if (e == 1) {
				vector<int> coord = { j + x, i + y };
				translated.push_back(coord);
			}
			j++;
		}
		i++;
		j = 0;
	}
	return translated;
}

void Pentomino::setRotation(int rotation) {
    while (_rotation != rotation) {
        rotate90deg();
    }
}
