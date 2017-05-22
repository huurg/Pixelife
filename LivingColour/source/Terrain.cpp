#include "Terrain.h"

#include <iostream>
using namespace std;

void Terrain::print() const {
    cout << "Terrain object:" << endl;
    cout << "\trgb = (" << (int)colour.r << ", " << (int)colour.g << ", " << (int)colour.b << ")" << endl;
    cout << "\txyi = (" << x << ", " << y << ", " << id << ")" << endl;
}
