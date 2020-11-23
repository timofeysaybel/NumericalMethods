#include "matrix.h"

class Equation
{
    Matrix matrix;
    vector<double> b;

public:

    Equation() {}

    Equation(Matrix m, vector<double> v) : matrix(m), b(v) {}

    ~Equation() {}

    static Equation getFromFile(string filename);

    static Equation getFromFunc(double (*f)(int, int));

    vector<double> solve();

    void print(vector<double>);
};
