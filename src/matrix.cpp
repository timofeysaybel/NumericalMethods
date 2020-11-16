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
                res.mat[res.index[i]][j] += mat[res.index[i]][k]*another.mat[res.index[k]][j];
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
            res[i] += mat[index[i]][j]*vec[j];
        }
    }

    return res;
}

void Matrix::calcLU(vector<double> &b)
{
    if (mat[index[0]][0] == 0)
    {
        int maj = findMajor(0);
        int t = index[0];
        index[0] = index[maj];
        index[maj] = t;
    }

    for (int i = 1; i < n; i++)
    {
        mat[index[0]][i] = mat[index[0]][i] / mat[index[0]][0];
        //b[index[0]] /= mat[index[0]][0];
    }
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            if (i == j)
            {
                if (mat[index[i]][j] == 0)
                {
                    int majIndx = findMajor(j);
                    int tmp = index[j];
                    index[j] = index[majIndx];
                    index[majIndx] = tmp;
                }
                for (int k = j + 1; k < n; k++)
                {
                    mat[index[i]][k] = mat[index[i]][k] / mat[index[i]][j];
                    //b[index[i]] /= mat[index[i]][j];
                }
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
        res.index[i] = index[i];
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
        res.index[i] = index[i];
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

vector<double> Matrix::solveL(vector<double> b)
{
    Matrix tmp = getL();

    for (int i = 0; i < n; i++)
    {
        b[tmp.index[i]] /= tmp.mat[tmp.index[i]][i];

        for (int j = i + 1; j < n; j++)
            b[tmp.index[j]] -= tmp.mat[tmp.index[j]][i]*b[tmp.index[i]];

    }

    return b;
}

vector<double> Matrix::solveU(vector<double> y)
{
    Matrix tmp = getU();

    for (int i = 0; i < n; i++)
    {
        for (int j = i - 1; j >= 0; j--)
            y[tmp.index[j]] -= tmp.mat[tmp.index[j]][i] * y[tmp.index[i]];
    }

    return y;
}

double Matrix::determinant()
{
    if (n == 1)
        return mat[0][0];
    if (n == 2)
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    double res = 0;

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