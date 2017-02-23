/* 
 * File:   main.cpp
 * Author: Travis
 *
 * Created on October 15, 2015, 9:25 AM
 */

#include <cstdlib>
//#include <iostream>
#include "CImg.h"
#include "Grid_1.h"
#include <ctime>

#define RED 0
#define GREEN 1
#define BLUE 2

using namespace std;
using namespace cimg_library;

template <typename T>
void draw_grid(CImg<T> &canvas, unsigned char color[], Grid &grid) {

    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    //draw vertical lines
    for (int i = 0; i <= grid._cols; i++) {
        y0 = grid._toff + 1;
        y1 = canvas.height() - grid._boff;
        x1 = x0 = grid._loff + 1 + i + (grid._cellwid * i);
        canvas.draw_line(x0, y0, x1, y1, color, 1);
    }

    //draw horizontal lines
    for (int i = 0; i <= grid._rows; i++) {
        x0 = grid._loff + 1;
        x1 = canvas.width() - grid._roff;
        y1 = y0 = grid._toff + 1 + i + (grid._cellht * i);
        canvas.draw_line(x0, y0, x1, y1, color, 1);

    }
}

template <typename T>
void color_cell(CImg<T> &canvas, unsigned char color[], Grid &grid, int col, int row) {

    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    x0 = grid._loff + 2 + col + (grid._cellwid * col);
    y0 = grid._toff + 2 + row + (grid._cellht * row);
    x1 = x0 + grid._cellwid - 1;
    y1 = y0 + grid._cellht - 1;

    canvas.draw_rectangle(x0, y0, x1, y1, color, 1);
}

template <typename T>
void redraw_cells(CImg<T> &canvas, Grid &grid) {
    unsigned char black[] = {0, 0, 0};
    unsigned char green[] = {144, 238, 144};
    for (unsigned int r = 0; r < grid.rows(); r++) {
        for (unsigned int c = 0; c < grid.cols(); c++) {
            if (grid.isOn(c, r)) {
                color_cell(canvas, green, grid, c, r);
            } else {
                color_cell(canvas, black, grid, c, r);
            }
        }
    }
}

int isLiving(unsigned char map[], Grid &grid, unsigned int c, unsigned int r) {

    return ((r >= 0)
            && (c >= 0)
            && (r < grid.rows())
            && (c < grid.cols())
            && (map[grid.cols() * r + c]))
            ? 1 : 0;
}

void update_living(Grid &grid, unsigned char map[]) {
    int n = 0;

    for (unsigned int k = 0; k < grid.size(); k++) {
        map[k] = grid.isOn(k);
    }
    for (unsigned int c = 0; c < grid.cols(); c++) {
        for (unsigned int r = 0; r < grid.rows(); r++) {

            n = (isLiving(map, grid, c - 1, r)
                    + isLiving(map, grid, c + 1, r)
                    + isLiving(map, grid, c, r - 1)
                    + isLiving(map, grid, c, r + 1)
                    + isLiving(map, grid, c + 1, r + 1)
                    + isLiving(map, grid, c + 1, r - 1)
                    + isLiving(map, grid, c - 1, r + 1)
                    + isLiving(map, grid, c - 1, r - 1)
                    );
            switch (n) {
                case 0:
                case 1:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    if (grid.isOn(c, r)) {
                        grid.flip(c, r);
                    }
                    break;
                case 3:
                    if (!grid.isOn(c, r)) {
                        grid.flip(c, r);
                    }
                    break;

                default:
                    break;

            }

        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    int col = 0, row = 0;

    time_t lastclick;
    time(&lastclick);

    unsigned char mdgrey2[] = {128, 128, 128};
    unsigned char black[] = {0, 0, 0};
    unsigned char green[] = {144, 238, 144};

    int x = 0, y = 0;

    Grid grid(152, 82, 10, 10, 20, 20);
    CImg <unsigned char> canvas(grid.canvasWidth(), grid.canvasHeight(), 1, 3, 0);


    unsigned char map[grid.cols() * grid.rows()];

    redraw_cells(canvas, grid);

    draw_grid(canvas, mdgrey2, grid);

    CImgDisplay disp(canvas, "Lab 8");


    while (!disp.is_closed()) {
        disp.wait();

        //left click
        if (disp.button()&1) {
            time(&lastclick);

            x = disp.mouse_x();
            y = disp.mouse_y();

            if (((col = grid.getCol(x)) < 0) || ((row = grid.getRow(y)) < 0))
                continue;
            if (!grid.isOn(col, row))
                grid.flip(col, row);
            redraw_cells(canvas, grid);
            disp.display(canvas);

        }

        while ((time(NULL) - lastclick) > 1) {
            if (disp.button() || disp.is_closed())
                break;

            update_living(grid, map);
            redraw_cells(canvas, grid);
            disp.display(canvas);
            cimg::wait(75);

        }
    }
    return 0;
}

