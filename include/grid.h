#ifndef GRID_DEF
#define GRID_DEF

#include<vector>
#include <math.h>
#include <stdio.h>

#include "math_ext.h"

// 1D GRID
template <class T>
class Grid {


private:
    int size, data_dim;
    unit res_t, res_s; 	// spatial and temporal resolutions

    vector<vector<T>> data;

public:
    Grid();
    Grid(int size, int data_dim, unit res_t, unit res_s);
	~Grid();

    T read(int ix, int dataset);
	void write(int ix, int dataset, T value);
    void inc(int ix, int dataset, T value);

};

#endif
