#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Matrix
{
    vector<vector<double>> mat;
    size_t n;
    vector<int> index;

    Matrix subMatrix(int k, int m);

    int findMajor(int k);

    double findElement(int mode, int &maxI, int &maxJ,int iter);

    int findMax() const;

    double countFault();

public:

    Matrix() {}

    Matrix(vector<vector<double>> vec, size_t size) : mat(vec), n(size) {}

    Matrix(size_t n);

    Matrix(const Matrix &another);

    ~Matrix();

    static Matrix getFromFile(string filename);

    static Matrix getFromFunc(double (*f)(int,int),size_t size);

    Matrix operator=(const Matrix &another);

    Matrix operator*(const Matrix &another) const;

    vector<double> operator*(const vector<double> &vec) const;

    void print(ostream &stream = cout) const;

    size_t getSize() {return n;}

    double determinant();

    Matrix transpose() const;

    bool operator==(const Matrix &another);

    int Jacobi(double precision, int mode, Matrix &solution,vector<double> &sol);
};
