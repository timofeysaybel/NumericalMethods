#include "matrix.h"

class Equation
{
    Matrix matrix;
    vector<int> b;

    double discrepancy(vector<int> x);

public:

    Equation() {}

    Equation(Matrix m, vector<int> v) : matrix(m), b(v) {}

    ~Equation() {}

    static Equation getFromFile(string filename);

    static Equation getFromFunc(int (*f)(int, int));

    vector<int> solve();

    void print(vector<int>);
};
