#include <cmath>
#include "../inc/matrix.h"

using namespace std;

Matrix::~Matrix()
{
    for (int i = 0; i < mat.size(); i++)
        mat[index[i]].clear();
    mat.clear();
    index.clear();
}

Matrix::Matrix(size_t size)
{
    n = size;

    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = i;

    mat.resize(n);
    for (int i = 0; i < n; i++)
    {
        mat[index[i]].resize(n);
        for (int j = 0; j < n; j++)
            mat[index[i]][j] = 0.0;
    }
}

Matrix::Matrix(vector <vector<double>> vec, size_t size)
{
    n = size;

    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = i;
    mat.resize(n);

    for (int i = 0; i < n; i++)
    {
        mat[index[i]].resize(n);
        for (int j = 0; j < n; j++)
            mat[index[i]][j] = vec[i][j];
    }
}

Matrix::Matrix(const Matrix &another)
{
    n = another.n;
    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = another.index[i];
    mat.resize(n);
    for (int i = 0; i < n; i++)
        mat[i].resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[index[i]][j] = another.mat[index[i]][j];
        }
    }
}

Matrix Matrix::operator=(const Matrix &another)
{
    n = another.n;
    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = another.index[i];
    mat.resize(n);
    for (int i = 0; i < n; i++)
        mat[i].resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[index[i]][j] = another.mat[index[i]][j];
        }
    }

    return *this;
}

Matrix Matrix::getFromFunc(double (*f)(int, int),size_t size)
{
    Matrix res;

    res.n = size;

    res.mat.resize(res.n);
    for (int i = 0; i < res.n; i++)
        res.mat[i].resize(res.n);

    for (int i = 0; i < res.n; i++)
    {
        for (int j = 0; j < res.n; j++)
            res.mat[i][j] = f(i,j);
    }

    res.index.resize(res.n);
    for (int i = 0; i < res.n; i++)
        res.index[i] = i;

    return res;
}

Matrix Matrix::operator*(const Matrix &another) const
{
    if (n != another.n)
    {
        cerr << "Разный размер матриц при перемножении" << endl;
        throw -1;
    }

    Matrix res(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res.mat[res.index[i]][j] = 0;
            for (int k = 0; k < n; k++)
            {
                res.mat[res.index[i]][j] += mat[index[i]][k]*another.mat[another.index[k]][j];
            }
        }
    }

    return res;
}

vector<double> Matrix::operator*(const vector<double> &vec) const
{
    if (n != vec.size())
    {
        cerr << "Разные размеры матрицы и вектора" << endl;
        throw -1;
    }

    vector<double> res(n);

    for (int i = 0; i < n; i++)
    {
        res[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            res[i] += mat[index[i]][j]*vec[index[j]];
        }
    }

    return res;
}

void Matrix::calcL(int k)
{
    double sum = 0.0;

    for (int i = k; i < n; i++)
    {
        sum = 0.0;
        for (int j = 0; j < k; j++)
        {
            sum += mat[index[i]][j] * mat[index[j]][k];
        }
        mat[index[i]][k] = mat[index[i]][k] - sum;
    }
}

void Matrix::calcU(int i)
{
    double sum = 0.0;

    for (int k = i + 1; k < n; k++)
    {
        sum = 0.0;
        for (int j = 0; j < i; j++)
        {
            sum += mat[index[i]][j] * mat[index[j]][k];
        }
       mat[index[i]][k] = (mat[index[i]][k] - sum) / mat[index[i]][i];
    }
}

void Matrix::calcLU(vector<double> &b)
{
    swapLines(0);

    for (int i = 1; i < n; i++)
        mat[index[0]][i] /=  mat[index[0]][0];

    for (int i = 1; i < n; i++)
    {
        calcL(i);
        swapLines(i);
        calcU(i);
    }

    for (int i = 0; i < n; i++)
    {
        if (mat[index[i]][i] == 0)
        {
            cerr << "Матрица A вырожденная" << endl;
            throw -1;
        }
    }
}

Matrix Matrix::getL() const
{
    Matrix res(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i >= j)
                res.mat[i][j] = mat[index[i]][j];
        }
    }

    return res;
}

Matrix Matrix::getU() const
{
    Matrix res(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i < j)
                res.mat[i][j] = mat[index[i]][j];
            if (i == j)
                res.mat[i][j] = 1;
        }
    }

    return res;
}

void Matrix::print(ostream &stream) const
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            stream << mat[index[i]][j] << " ";
        stream << endl;
    }
    stream << endl;
}

int Matrix::getSumCol(int k)
{
    int res = 0;

    for (int i = 0; i < n; i++)
        res += mat[index[i]][k];

    return res;
}

vector<double> Matrix::solveL(vector<double> b)
{
    for (int i = 0; i < n; i++)
    {
        b[index[i]] /= mat[index[i]][i];

        for (int j = i + 1; j < n; j++)
            b[index[j]] -= mat[index[j]][i] * b[index[i]];

    }
    return b;
}

vector<double> Matrix::solveU(vector<double> y)
{
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
            y[index[j]] -= mat[index[j]][i] * y[index[i]];
    }
    vector<double> res(n);
    for (int i = 0; i < n; i++)
        res[i] = y[index[i]];
    return res;
}

int Matrix::findMajor(int k)
{
    int max = mat[index[k]][k], maxi = k;
    for (int i = k + 1; i < n; i++)
    {
        if (abs(max) < abs(mat[index[i]][k]))
        {
            max = mat[index[i]][k];
            maxi = index[i];
        }
    }
    return maxi;
}

void Matrix::swapLines(int k)
{
    int maj = findMajor(k);
    int t = index[k];
    index[k] = index[maj];
    index[maj] = t;
}

double Matrix::discrepancy(vector<double> x, vector<double> b)
{
    double sum = 0.0;
    auto vec = (getL()*getU()) * x;
    for (int i = 0; i < n; i++)
        vec[i] -= b[index[i]];

    for (int i = 0; i < n; i++)
        sum += vec[i] * vec[i];

    return sqrt(sum);
}