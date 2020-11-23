#include <fstream>
#include <cmath>

#include "../inc/equation.h"

Equation Equation::getFromFile(string filename)
{
    vector<vector<double>> res;
    vector<double> b;

    ifstream file(filename);

    if (!file.is_open() || file.eof())
    {
        cerr << "Не удалось открыть файл или файл пустой" << endl;
        throw -1;
    }

    size_t size;
    file >> size;

    if (size > INT32_MAX)
        throw -1;

    b.resize(size);
    res.resize(size);
    for (int i = 0; i < size; i++)
        res[i].resize(size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (file.eof())
            {
                cerr << "Неверный формат файла" << endl;
                throw -1;
            }

            file >> res[i][j];
        }
        if (file.eof())
        {
            cerr << "Неверный формат файла" << endl;
            throw -1;
        }
        file >> b[i];
    }

    file.close();

    Equation result(Matrix(res,size),b);

    return result;
}

Equation Equation::getFromFunc(double (*f)(int, int))
{
    Equation res;

    size_t size;
    cout << "Введите размер матрицы" << endl;
    cin >> size;

    res.matrix = Matrix::getFromFunc(f,size);
    res.b.resize(size);

    for (int i = 0; i < size; i++)
    {
        res.b[i] = res.matrix.getSumCol(i);
    }

    return res;
}

void Equation::print(vector<double> x)
{
    size_t size = matrix.getSize();

    if (size > 10)
    {
        string outfile("result");
        ofstream stream(outfile);

        stream << "Матрица L: " << endl;
        matrix.getL().print(stream);
        stream << "Матрица U: " << endl;
        matrix.getU().print(stream);
        stream << "Решение СЛАУ:" << endl;
        for (int i = 0; i < size; i++)
            stream << x[i] << " ";
        stream << endl << endl;
        stream << "Норма невязки: " << matrix.discrepancy(x,b) << endl;
        cout << "Результат сохранён в файл result" << endl;
    }
    else
    {
        cout << "Матрица L:" << endl;
        matrix.getL().print();
        cout << "Матрица U:" << endl;
        matrix.getU().print();
        cout << "Решение СЛАУ:" << endl;
        for (int i = 0; i < size; i++)
            cout << x[i] << " ";
        cout << endl << endl;
        cout << "Норма невязки: " << matrix.discrepancy(x,b) << endl;
    }
}

vector<double> Equation::solve()
{
    matrix.calcLU(b);
    return matrix.solve(b);
}