/* 
 * File:   Grid.h
 * Author: Travis
 *
 * Created on October 15, 2015, 6:36 PM
 */

#ifndef GRID_H
#define	GRID_H

#include "CImg.h"

class Grid {
public:
    Grid(unsigned int, unsigned int, unsigned int cellwid, int unsigned cellht, int unsigned xoff, int unsigned yoff); //cols,rows
    ~Grid();
    unsigned int cols(); //get cols
    unsigned int rows(); //get rows
    unsigned int size(); //rows*cols

    unsigned int cellWidth(); //get cell width
    unsigned int cellHeight(); //get cell height

    unsigned int canvasWidth();
    unsigned int canvasHeight();

    int getCol(int x);
    int getRow(int x);

    // Grid cellWidth(int); //set width
    // Grid cellHeight(int); //set height

    // Grid padding(int); //set padding on all sides
    // Grid padding(int, int); //set padding of left&right and top&bottom
    // Grid padding(int, int, int, int); //set padding of left, right, top, bottom

    Grid* flip(unsigned int, unsigned int); //flip col,row
    Grid* flipXY(int, int); //flip x,y
    Grid* flip(unsigned int); //flip cell at x,y coordinate

    Grid* color(unsigned char *); //set color to {RED,GREEN,BLUE}
    Grid* color(unsigned int); //set color

    Grid* drawGrid();

    bool isOn(unsigned int, unsigned int);
    bool isOn(unsigned int);


    unsigned int color(); //get color
    unsigned int _rows, _cols;
    unsigned char _red, _green, _blue;
    unsigned int _cellht, _cellwid;
    unsigned int _loff, _roff, _toff, _boff;
private:
    unsigned int _canvaswid, _canvasht;

    unsigned char *a;
};

unsigned int Grid::cols() {
    return _cols;
}

unsigned int Grid::rows() {
    return _rows;
}

unsigned int Grid::size() {
    return _rows * _cols;
}

Grid::Grid(unsigned int cols, unsigned int rows, unsigned int cellwid, unsigned int cellht, unsigned int xoff, unsigned int yoff) {
    _cols = cols;
    _rows = rows;
    _cellwid = cellwid;
    _cellht = cellht;
    _roff = _loff = xoff;
    _toff = _boff = yoff;
    _canvaswid = _cellwid * _cols + _loff + _roff + _cols + 1;
    _canvasht = _cellht * _rows + _toff + _boff + _rows + 1;

    a = new unsigned char[_cols * _rows];
    for (unsigned int i = 0; i < _rows * _cols; i++) {
        a[i] = 0;
    }

}

Grid::~Grid() {
    delete a;
}

unsigned int Grid::color() {
    unsigned int _color = _red;
    _color = (_color << 8) + _green;
    _color = (_color << 8) + _blue;
    return _color;
}

Grid* Grid::color(unsigned char c[]) {

    _red = c[0];
    _green = c[1];
    _blue = c[2];
    return this;
}

Grid* Grid::color(unsigned int c) {
    _red = (c & 0xff0000) >> 16;
    _green = (c & 0x00ff00) >> 8;
    _blue = c & 0x0000ff;
    return this;
}

unsigned int Grid::cellHeight() {
    return _cellht;
}

unsigned int Grid::cellWidth() {
    return _cellwid;
}

Grid* Grid::flip(unsigned int c, unsigned int r) {
    a[_cols * r + c] = !a[_cols * r + c];
    return this;
}

Grid* Grid::flip(unsigned int x) {
    a[x] = !a[x];
    return this;
}

bool Grid::isOn(unsigned int c, unsigned int r) {
    return a[_cols * r + c];
}

bool Grid::isOn(unsigned int x) {
    return a[x];
}

unsigned int Grid::canvasHeight() {
    return _canvasht;
}

unsigned int Grid::canvasWidth() {
    return _canvaswid;
}

int Grid::getCol(int x) {

    //check that x is inside the margins
    if (x <= _loff || x >= _canvaswid - _roff) return -1;

    //check if x is on a border
    if (!((x - _loff) % (_cellwid + 1) - 1)) {
        //        std::cout << "line: " << (x - _loff) << std::endl;
        return -1;

    }

    //    std::cout << "x: " << x - _loff << "   ";
    //    int origin = ;
    //    int col = (x - _loff - 1) / (_cellwid + 1);
    //    std::cout << "col: " << col << std::endl;

    return (x - _loff - 1) / (_cellwid + 1);
}

int Grid::getRow(int y) {

    //check it's in the margin
    if (y <= _toff || y >= _canvasht - _boff)
        return -1;

    //check if x is on a border
    if (!((y - _toff) % (_cellht + 1) - 1))
        return -1;

    return (y - _toff - 1) / (_cellht + 1);

}

#endif	/* GRID_H */

