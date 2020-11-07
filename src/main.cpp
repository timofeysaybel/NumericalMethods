#include <chrono>

#include "../inc/equation.h"

int fillFunc(int i,int j)
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
        if (choice == 0)
        {
            cout << "Введите имя файла" << endl;
            string filename;
            cin >> filename;
            Equation eq = Equation::getFromFile(filename);
            auto x = eq.solve();
            eq.print(x);
        }
        else
        {
            Equation eq = Equation::getFromFunc(fillFunc);
            auto x = eq.solve();
            eq.print(x);
        }
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
