#include <chrono>
#include <iomanip>

#include "../inc/matrix.h"

#define PRECISION 0.001
#define RES_FILE "result"

double fillFunc(int i,int j)
{
    return i*j + 1;
}

int main()
{
    auto begin = chrono::steady_clock::now();
    cout << "Введите 0, чтобы ввести матрицу A из файла" << endl;
    cout << "Введите 1, чтобы ввести матрицу A по формуле" << endl;
    cout << "Введите любое другое число, чтобы выйти" << endl;
    int choice = 0;
    cin >> choice;
    try
    {
        if (choice != 0 && choice != 1)
            return 0;

        Matrix matrix;
        if (choice == 0)
        {
            cout << "Введите имя файла: ";
            string filename;
            cin >> filename;
            matrix = Matrix::getFromFile(filename);
        }
        else
        {
            cout << "Введите размер матрицы: ";
            int size = 0;
            cin >> size;
            matrix = Matrix::getFromFunc(fillFunc,size);
        }
        matrix.print();
        matrix.transpose().print();
        if (!(matrix == matrix.transpose()))
        {
            cerr << "Матрица несимметричная" << endl;
            return -1;
        }

        cout << "Введите стратегию выбора обнуляемого элемента" << endl;
        cout << "0 - обнуление максимального элемента" << endl;
        cout << "1 - обнуление оптимального элемента" << endl;
        cout << "2 - циклический выбор обнуляемого элемента" << endl;

        int mode = 0;
        cin >> mode;

        if (mode < 0 || mode > 2)
        {
            cerr << "Неверный номер" << endl;
            return -1;
        }

        Matrix solution;
        vector<double> coef;
        int iterations = matrix.Jacobi(PRECISION,mode,solution,coef);
        //cout << fixed << setprecision(3);
        cout << "Собственные значения:" << endl;
        for (int i = 0; i < coef.size(); i++)
            cout << coef[i] << " ";
        cout << endl << endl;

        if (matrix.getSize() > 10)
        {
            ofstream out(RES_FILE);
            //out << fixed << setprecision(3);
            out << "Собственные векторы: " << endl;
            matrix.print(out);
        }
        else
        {
            cout << "Собственные векторы: " << endl;
            matrix.print();
        }

        cout << "Количество итераций: " << iterations << endl;
    }
    catch (int)
    {
        return -1;
    }

    auto end = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "Время выполнения программы: " << elapsed_ms.count() << " миллисекунд" << endl;
    return 0;
}
