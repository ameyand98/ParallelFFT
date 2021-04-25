#include <complex> 
#include <vector>
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;
using comp_double = complex<double>; 
const double PI = 3.1415926;

void fft(vector<comp_double>* array);

int main() {
    vector<comp_double>* array = new vector<comp_double>{1, 2, 3, 4};
    fft(array);
    for (int i = 0; i < array->size(); i++) {
        cout << array->at(i) << " ";
    }

}

void fft(vector<comp_double>* array) {
    int size = array->size();
    if (size == 1) return;

    vector<comp_double>* ar1 = new vector<comp_double>(size / 2);
    vector<comp_double>* ar2 = new vector<comp_double>(size / 2);
    for (int i = 0; 2 * i < size; i++) {
        ar1->at(i) = array->at(2 * i);
        ar2->at(i) = array->at(2 * i + 1);
    }

    fft(ar1);
    fft(ar2);

    double angle = 2 * PI / size;
    comp_double w(1), wn(cos(angle), sin(angle));
    for (int i = 0; 2 * i < size; i++) {
        array->at(i) = ar1->at(i) + w * ar2->at(i);
        array->at(i + size / 2) = ar1->at(i) - w * ar2->at(i);
        w *= wn;
    }

}
