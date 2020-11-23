#include <cmath>
#include "../inc/matrix.h"
#include <unistd.h>

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
            mat[i][j] = 0.0;
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

Matrix Matrix::getFromFile(string filename)
{
    Matrix res;
    ifstream file(filename);

    if (!file.is_open() || file.eof())
    {
        cerr << "Не удалось открыть файл или файл пустой" << endl;
        throw -1;
    }

    file >> res.n;

    if (res.n > INT32_MAX)
        throw -1;

    res.mat.resize(res.n);
    for (int i = 0; i < res.n; i++)
        res.mat[i].resize(res.n);

    for (int i = 0; i < res.n; i++)
    {
        for (int j = 0; j < res.n; j++)
        {
            if (file.eof())
            {
                cerr << "Неверный формат файла" << endl;
                throw -1;
            }

            file >> res.mat[i][j];
        }
        if (file.eof())
        {
            cerr << "Неверный формат файла" << endl;
            throw -1;
        }
    }

    file.close();

    return res;
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
        res[i] = 0;
        for (int j = 0; j < n; j++)
        {
            res[i] += mat[index[i]][j]*vec[j];
        }
    }

    return res;
}

void Matrix::print(ostream &stream) const
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            stream << mat[index[i]][j] << "\t";
        stream << endl;
    }
    stream << endl;
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

Matrix Matrix::transpose() const
{
    Matrix res(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            res.mat[j][i] = mat[i][j];
    }

    return res;
}

bool Matrix::operator==(const Matrix &another)
{
    if (another.n != n)
        return false;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] != another.mat[i][j])
                return false;
        }
    }

    return true;
}

int Matrix::Jacobi(double precision, int mode, Matrix &solution, vector<double> &coefficients)
{
    int iterations = 0;
    int i,j,k;
    int maxI,maxJ;
    double fi;

    Matrix rotation(n);

    double fault = countFault();

    while (fault > precision)
    {
        findElement(mode,maxI,maxJ,iterations);
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                rotation.mat[i][j] = 0;
            }
            rotation.mat[i][i] = 1;
        }
        double x = -2.0 * mat[maxI][maxJ], y = mat[maxI][maxI] - mat[maxJ][maxJ];
        if (y == 0)
        {
            rotation.mat[maxI][maxI] = rotation.mat[maxJ][maxJ] = rotation.mat[maxJ][maxI] = 1 / 2.0;
            rotation.mat[maxI][maxJ] = -1 / 2.0;
        }
        else
        {
            fi = 0.5 * atan((2.0 * mat[maxI][maxJ]) / (mat[maxI][maxI] - mat[maxJ][maxJ]));
            rotation.mat[maxI][maxI] = rotation.mat[maxJ][maxJ] = cos(fi);
            rotation.mat[maxI][maxJ] = -sin(fi);
            rotation.mat[maxJ][maxI] = sin(fi);
        }

        (*this) = rotation.transpose()*(*this)*rotation;

        fault = countFault();

        iterations++;
    }

    Matrix tmp(n);
    coefficients.resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            tmp.mat[i][j] = mat[i][j];
        coefficients[i] = mat[i][i];
    }
    solution = tmp;

    return iterations;
}

double Matrix::findElement(int mode, int &maxI, int &maxJ,int iter)
{
    double max = 0.0;

    switch(mode)
    {
        case 0:
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (abs(mat[i][j]) > abs(max) && i != j)
                    {
                        max = mat[i][j];
                        maxI = i;
                        maxJ = j;
                    }
                }
            }
            break;
        case 1:
            maxI = findMax();
            for (int j = 0; j < n; j++)
            {
                if (abs(max) < abs(mat[maxI][j]) && maxI != j)
                {
                    max = mat[maxI][j];
                    maxJ = j;
                }
            }
            break;
        case 2:
            while(iter >= 0)
            {
                for (int i = 0; i < n - 1; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        if (i != j)
                        {
                            iter--;
                            if (iter <= 0)
                            {
                                max = mat[i][j];
                                maxI = i;
                                maxJ = j;
                                return max;
                            }
                        }
                    }
                }
            }
            break;
        default:
            throw -1;
    }
    return max;
}

int Matrix::findMax() const
{
    double max = 0.0;
    int maxI = 0;
    for (int i = 0; i < n - 1; i++)
    {
        double tmp = 0.0;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
                tmp += mat[i][j] * mat[i][j];
        }
        if (max < tmp)
        {
            max = tmp;
            maxI = i;
        }
    }
    return maxI;
}

double Matrix::countFault()
{
    double res = 0.0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
                res += mat[i][j] * mat[i][j];
        }
    }

    return sqrt(res);
}