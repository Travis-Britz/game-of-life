/* 
 * File:   Grid<T>.h
 * Author: Travis
 *
 * Created on October 15, 2015, 6:36 PM
 */

#ifndef GRID_H
#define	GRID_H

#include "CImg.h"

template <typename T>
class Grid {
public:
    Grid(unsigned int, unsigned int); //cols,rows
    ~Grid();
    unsigned int cols(); //get cols
    unsigned int rows(); //get rows
    unsigned int size(); //rows*cols

    unsigned int cellWidth(); //get cell width
    unsigned int cellHeight(); //get cell height
    bool is_closed();

    cimg_library::CImg<T> canvas();
    cimg_library::CImgDisplay * display();

    // Grid<T> cellWidth(int); //set width
    // Grid<T> cellHeight(int); //set height

    // Grid<T> padding(int); //set padding on all sides
    // Grid<T> padding(int, int); //set padding of left&right and top&bottom
    // Grid<T> padding(int, int, int, int); //set padding of left, right, top, bottom

    Grid<T>* flip(unsigned int, unsigned int); //flip col,row
    Grid<T>* flipXY(int, int); //flip x,y
    Grid<T>* flip(unsigned int); //flip cell at x,y coordinate

    Grid<T>* color(unsigned char *); //set color to {RED,GREEN,BLUE}
    Grid<T>* color(unsigned int); //set color

    Grid<T>* drawGrid();

    bool isOn(unsigned int, int);
    bool isOn(unsigned int);


    unsigned int color(); //get color
    cimg_library::CImg<T> _canvas;
    cimg_library::CImgDisplay _window;
private:
    unsigned int _rows, _cols;
    unsigned char _red, _green, _blue;
    unsigned int _cellht, _cellwid;
    unsigned int _loff, _roff, _toff, _boff;

    unsigned char *a;
};

template <typename T>
unsigned int Grid<T>::cols() {
    return _cols;
}

template <typename T>
unsigned int Grid<T>::rows() {
    return _rows;
}

template <typename T>
unsigned int Grid<T>::size() {
    return _rows * _cols;
}

template <typename T>
Grid<T>::Grid(unsigned int c, unsigned int r) {
    _cols = c;
    _rows = r;
    a = new unsigned char[c * r];

    for (unsigned int i = 0; i < r * c; i++) {
        a[i] = 0;
    }

    _red = _green = _blue = 255;

    _cellht = _cellwid = 30;

    _loff = _roff = _toff = _boff = 20;

    cimg_library::CImg<T> _canvas(_cellwid * _cols + _loff + _roff + _cols + 1, _cellht * _rows + _toff + _boff + _rows + 1, 1, 3, 0);
    cimg_library::CImgDisplay _window(_canvas, "Life");
}

template <typename T>
Grid<T>::~Grid() {
    delete a;
}

template <typename T>
cimg_library::CImgDisplay *Grid<T>::display() {
    return &_window;
}

template <typename T>
cimg_library::CImg<T> Grid<T>::canvas() {
    return _canvas;
}

template <typename T>
unsigned int Grid<T>::color() {
    unsigned int _color = _red;
    _color = (_color << 8) + _green;
    _color = (_color << 8) + _blue;
    return _color;
}

template <typename T>
Grid<T>* Grid<T>::color(unsigned char c[]) {

    _red = c[0];
    _green = c[1];
    _blue = c[2];
    return this;
}

template <typename T>
Grid<T>* Grid<T>::color(unsigned int c) {
    _red = (c & 0xff0000) >> 16;
    _green = (c & 0x00ff00) >> 8;
    _blue = c & 0x0000ff;
    return this;
}

template <typename T>
unsigned int Grid<T>::cellHeight() {
    return _cellht;
}

template <typename T>
unsigned int Grid<T>::cellWidth() {
    return _cellwid;
}

template <typename T>
Grid<T>* Grid<T>::flip(unsigned int c, unsigned int r) {

    unsigned char color[3] = {};
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    //assign and check true/false
    if (a[_cols * r + c] = !a[_cols * r + c]) {
        color[0] = _red;
        color[1] = _green;
        color[2] = _blue;
    } else {
        color[0] = color[1] = color[2] = 0;
    }




    x0 = _loff + 2 + c + (_cellwid * c);
    y0 = _toff + 2 + r + (_cellht * r);
    x1 = x0 + _cellwid - 1;
    y1 = y0 + _cellht - 1;

    _canvas.draw_rectangle(x0, y0, x1, y1, color, 1);

    return this;
}

template <typename T>
Grid<T>* Grid<T>::flip(unsigned int x) {
    a[x] = !a[x];
    return this;
}

template <typename T>
Grid<T>* Grid<T>::flipXY(int x, int y) {
    if (x < _loff || x > _canvas.width() - _roff) return;
    if (y < _toff || y > _canvas.height() - _boff) return;

    unsigned char color[3] = {};
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;


    int c = (x - x / _cellwid - _loff) / _cellwid;
    int r = (y - y / _cellht - _toff) / _cellht;

    //assign and check true/false
    if (a[_cols * r + c] = !a[_cols * r + c]) {
        color[0] = _red;
        color[1] = _green;
        color[2] = _blue;
    } else {
        color[0] = color[1] = color[2] = 0;
    }

    x0 = _loff + 2 + c + (_cellwid * c);
    y0 = _toff + 2 + r + (_cellht * r);
    x1 = x0 + _cellwid - 1;
    y1 = y0 + _cellht - 1;


    _canvas.draw_rectangle(x0, y0, x1, y1, color, 1);
    return this;
}

template <typename T>
Grid<T>* Grid<T>::drawGrid() {
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    unsigned char mdgrey2[] = {128, 128, 128};
    unsigned char* color = mdgrey2;
    //draw vertical lines
    for (int i = 0; i <= _cols; i++) {
        y0 = _toff + 1;
        y1 = _canvas.height() - _boff;
        x1 = x0 = _loff + 1 + i + (_cellwid * i);
        _canvas.draw_line(x0, y0, x1, y1, color, 1);
    }

    //draw horizontal lines
    for (int i = 0; i <= _rows; i++) {
        x0 = _loff + 1;
        x1 = _canvas.width() - _roff;
        y1 = y0 = _toff + 1 + i + (_cellht * i);
        _canvas.draw_line(x0, y0, x1, y1, color, 1);

    }
}

template <typename T>
bool Grid<T>::is_closed(){
    return _window.is_closed();
}

#endif	/* GRID_H */

