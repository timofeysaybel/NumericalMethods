#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
    vector<vector<int>> mat;
    size_t n;
    vector<int> index;

    Matrix subMatrix(int k, int m);

    int findMajor(int k);

public:

    Matrix() {}

    Matrix(vector<vector<int>> vec, size_t size) : mat(vec), n(size) {}

    Matrix(size_t n);

    Matrix(const Matrix &another);

    ~Matrix();

    static Matrix getFromFunc(int (*f)(int,int),size_t size);

    Matrix operator=(const Matrix &another);

    Matrix operator*(const Matrix &another) const;

    vector<int> operator*(const vector<int> &vec) const;

    void calcLU();

    Matrix getL() const;

    Matrix getU() const;

    void print(ostream &stream = cout) const;

    int getSumCol(int k);

    size_t getSize() {return n;}

    vector<int> solveL(vector<int> b);

    vector<int> solveU(vector<int> y);

    int determinant();

    vector<int> solve(vector<int> b) {return solveU(solveL(b));}
};