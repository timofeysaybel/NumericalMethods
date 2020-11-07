#include <cmath>
#include "../inc/matrix.h"

using namespace std;

Matrix::~Matrix()
{
    for (int i = 0; i < mat.size(); i++)
        mat[i].clear();
    mat.clear();
    index.clear();
}

Matrix::Matrix(size_t size)
{
    n = size;

    mat.resize(n);
    for (int i = 0; i < n; i++)
    {
        mat[i].resize(n);
        for (int j = 0; j < n; j++)
            mat[i][j] = 0;
    }

    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = i;
}

Matrix::Matrix(const Matrix &another)
{
    n = another.n;
    mat.resize(n);
    for (int i = 0; i < n; i++)
        mat[i].resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = another.mat[i][j];
        }
    }

    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = i;
}

Matrix Matrix::operator=(const Matrix &another)
{
    n = another.n;
    mat.resize(n);
    for (int i = 0; i < n; i++)
        mat[i].resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = another.mat[i][j];
        }
    }

    index.resize(n);
    for (int i = 0; i < n; i++)
        index[i] = i;

    return *this;
}

Matrix Matrix::getFromFunc(int (*f)(int, int),size_t size)
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
                res.mat[res.index[i]][j] += mat[res.index[i]][k]*another.mat[res.index[k]][j];
            }
        }
    }

    return res;
}

vector<int> Matrix::operator*(const vector<int> &vec) const
{
    if (n != vec.size())
    {
        cerr << "Разные размеры матрицы и вектора" << endl;
        throw -1;
    }

    vector<int> res(n);

    for (int i = 0; i < n; i++)
    {
        res[i] = 0;
        for (int j = 0; j < n; j++)
        {
            res[i] += mat[index[i]][j]*vec[j];
        }
    }

    return res;
}

void Matrix::calcLU()
{
    if (mat[0][0] == 0)
    {
        int maj = findMajor(0);
        int t = index[0];
        index[0] = index[maj];
        index[maj] = t;
    }

    for (int i = 1; i < n; i++)
        mat[index[0]][i] = mat[index[0]][i] / mat[index[0]][0];

    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            if (i == j && mat[i][j] == 0)
            {
                int majIndx = findMajor(j);
                int tmp = index[j];
                index[j] = index[majIndx];
                index[majIndx] = tmp;
            }
            if (i >= j)
            {
                for (int k = 0; k < j; k++)
                    mat[index[i]][j] -= (mat[index[i]][k] * mat[index[k]][j]);
            }
            if (i < j)
            {
                for (int k = 0; k < i; k++)
                    mat[index[i]][j] -= mat[index[i]][k] * mat[index[k]][j];
                mat[index[i]][j] /= mat[index[i]][i];
            }
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
                res.mat[res.index[i]][j] = mat[index[i]][j];
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
                res.mat[res.index[i]][j] = mat[index[i]][j];
            if (i == j)
                res.mat[res.index[i]][j] = 1;
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

vector<int> Matrix::solveL(vector<int> b)
{
    Matrix tmp = getL();

    for (int i = 0; i < n; i++)
    {
        b[i] /= tmp.mat[i][i];

        for (int j = i + 1; j < n; j++)
            b[j] -= tmp.mat[j][i]*b[i];

    }

    return b;
}

vector<int> Matrix::solveU(vector<int> y)
{
    Matrix tmp = getU();

    for (int i = 0; i < n; i++)
    {
        for (int j = i - 1; j >= 0; j--)
            y[j] -= tmp.mat[j][i] * y[i];
    }

    return y;
}

int Matrix::determinant()
{
    if (n == 1)
        return mat[0][0];
    if (n == 2)
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    int res = 0;

    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
            res += subMatrix(0, i).determinant() * mat[0][i];
        else
            res -= subMatrix(0, i).determinant() * mat[0][i];
    }

    return res;
}

Matrix Matrix::subMatrix(int k, int m)
{
    Matrix res(n - 1);

    int ri = 0;
    for (int i = 0; i < n; i++)
    {
        int rj = 0;
        for (int j = 0; j < n; j++)
        {
            if (i == k)
            {
                ri--;
                break;
            }
            if (j == m)
                continue;

            res.mat[index[ri]][rj] = mat[index[i]][j];
            rj++;
        }
        ri++;
    }

    return res;
}

int Matrix::findMajor(int k)
{
    int max = 0, maxi = k;
    for (int i = k; i < n; i++)
    {
        if (abs(max) < abs(mat[index[i]][k]))
        {
            max = mat[index[i]][k];
            maxi = i;
        }
    }
    return maxi;
}