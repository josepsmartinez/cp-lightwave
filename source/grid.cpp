#include "grid.h"

template<class T>
Grid<T>::Grid() {}

template<class T>
Grid<T>::Grid(int size, int data_dim, unit res_t, unit res_s) :
    size(size), data_dim(data_dim), res_t(res_t), res_s(res_s)
{
    // Allocates iteration data
    data.resize(size);
    for(int i=0; i<data.size(); i++) data[i].resize(data_dim);
}

template<class T>
Grid<T>::~Grid()
{

}

template<class T>
T Grid<T>::read(int ix, int dataset)
{
    return data[ix][dataset];
}

template<class T>
void Grid<T>::write(int ix, int dataset, T value)
{
    data[ix][dataset] = value;
}

template<class T>
void Grid<T>::inc(int ix, int dataset, T value)
{
    data[ix][dataset] += value;
}





template class Grid<unit>;
