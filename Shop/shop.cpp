#include <iostream>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <string>
#define MAX 20

using namespace std;

struct product
{
    char name[MAX];
    double price;
    int count;

};

int IndexProductInWarehouse(product* warehouse, char* name, int countOfProducts)
{

    int index = -1;
    for (int j = 0; j < countOfProducts; j++)
    {
        bool other = true;
        for (int i = 0; warehouse[j].name[i] != '\0' || name[i] != '\0'; i++)
        {
            if (warehouse[j].name[i] > name[i] || warehouse[j].name[i] < name[i])
            {
                other = false;
                break;
            }
        }
        if (other)
        {
            index = j;
        }
    }
    return index;
}

void input(product * warehouse, int & countOfProducts, int size) //ccылка
{
    if (countOfProducts < size)
    {
        char name[MAX];
        double price;
        int count;
        cout << "Введите название товара: " << '\n';
        //временное слово
        cin.ignore(); //игнорирует \n 
        cin.get(name, MAX);
        int index = IndexProductInWarehouse(warehouse, name, countOfProducts);
        if (index == -1)
        {
            strcpy_s(warehouse[countOfProducts].name, name);
            do
            {
                cout << "Введите цену товара: " << '\n';
                cin >> price;
            } while (price <= 0);
            warehouse[countOfProducts].price = price;
            do
            {
                cout << "Введите количество товара: " << '\n';
                cin >> count;
            } while (count <= 0);
            warehouse[countOfProducts].count = count;
            countOfProducts++;
        }
        else
        {
            int addCount;
            cout << "Товар уже есть в списке, введите количетво добавляемого товара: " << '\n';
            cin >> addCount;
            warehouse[index].count += addCount;
        }
        int answer;
        cout << "Добавить еще один товар? 1-да 2-нет\n";
        cin >> answer;
        if (answer == 1)
        {
            input(warehouse, countOfProducts, size);
        }

    }
    else 
    {
        cout << "Склад заполнен\n";
    }
    
}

void ProductDelete(product* warehouse,int index, int & countOfProducts)
{
    for (int i = index; i < countOfProducts-1; i++)
    {
        swap(warehouse[i], warehouse[i + 1]);
    }
    countOfProducts--;


}

void Buying(product* warehouse, int & countOfProducts, list<product> & check)
{
    char name[MAX];
    int index ,answer;
    double finalPrice= 0;
    cout << "Введите имя товара: ";
    cin.ignore(); //игнорирует \n 
    cin.get(name, MAX);

    index = IndexProductInWarehouse(warehouse, name, countOfProducts);
    if (index == -1)
    {
        cout << "\nТовара нет в списке\n";
    }
    else
    {
        product buy;
        int count;
        do
        {
            cout << "Введите количество товара: " << '\n';
            cin >> count;
        } while ((count <= 0) || (count > (warehouse)[index].count)); //(warehouse)[index].count

        strcpy_s(buy.name, name);
        buy.count = count;
        buy.price = warehouse[index].price;

        check.push_back(buy); //добавление в чек товара
        

        warehouse[index].count -= count;

        if (warehouse[index].count == 0)
            ProductDelete(warehouse, index, countOfProducts);
    }
    cout << "Хотите купить другой товар? 1-да 2-нет\n";
    cin >> answer;
    if (answer == 1)
        Buying(warehouse, countOfProducts, check);
    else 
    {
        cout << "Чек:\n";
        for (product n : check)
        {
            cout << n.name << ' ' << n.price << ' ' << n.count << "шт" << "\n";
            finalPrice += n.price * n.count;
        }
        cout << endl;
        cout << "Итого: " << finalPrice << endl;
    }
    
}

void FileOut(ofstream & fileOut, int countOfProducts, product *warehouse, int size)
{
    fileOut << size << endl;
    for (int i = 0; i < countOfProducts; i++)
    {
        fileOut << warehouse[i].name << endl; 
        fileOut << warehouse[i].price << endl;
        fileOut << warehouse[i].count << endl;
    }
    fileOut.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    ifstream fileIn("warehouse.txt");
    product* warehouse = nullptr;
    int answer, countOfProducts = 0, size; // количество будет известно после считки
    bool firstStart = true;
    if (fileIn.is_open())
        firstStart = false;
    if (firstStart)
    {
        do
        {
            cout << "Введите размер склада" << endl;
            cin >> size;
        } while (size <= 0);
    }
    else
    {
        //cin >> size 
        fileIn >> size;
        fileIn.ignore(); // игнорируем \n

    }
    warehouse = (product*)malloc(sizeof(product) * size); // = new product[size]

    if (!firstStart)
    {//
        ///   vv  vv<summary>
        /// 12
        /// 35
        ///         /// <returns></returns>              cin >>   warehouse[i].price >> warehouse[i].count
        int i = 0;
        while (fileIn.getline(warehouse[i].name, MAX) && fileIn >> warehouse[i].price >> warehouse[i].count) { //считывание с файла
            fileIn.ignore(); 
            i++;
            countOfProducts++;
        }
    }
    fileIn.close();
    ofstream fileOut("warehouse.txt"); //fstream не работает если создается новый файл
    //

    do //меню
    {
        cout << "1. Добавить товар\n";
        cout << "2. Купить\n";
        cout << "3. Печать товаров \n";
        cout << "4. Выход\n";
        cin >> answer;

        switch (answer)
        {
        case 1:
            system("cls");
            input(warehouse, countOfProducts, size);
            FileOut(fileOut, countOfProducts, warehouse, size);
            break;
        case 2:
            system("cls");
            if (countOfProducts == 0)
                cout << "Продуктов нет\n" << endl;
            else
            {
                list<product> check;//можно и обычным массивом с размером сайз, но тогда при большом кооличестве товаров будет выделять много неиспользуемой памяти
                Buying(warehouse, countOfProducts, check);
            }
            FileOut(fileOut, countOfProducts, warehouse, size);
            break;


        case 3:
            system("cls");
            if (countOfProducts == 0)
                cout << "Продуктов нет\n" << endl;
            else
            {
                cout << "|";
                cout.width(MAX);

                cout.setf(ios::left);
                
                cout << "Название товара";
                cout << "|";
                
                cout.width(10);
                cout.setf(ios::left);
                cout << "Цена ";
                cout << "|";
                
                cout.width(10);
                cout.setf(ios::left);
                cout << "Количество" << "|" <<  '\n';
                for (int i = 0; i < countOfProducts; i++)
                {
                    cout << "|";
                    cout.width(MAX);
                    cout.setf(ios::left);
                    cout << warehouse[i].name;
                    
                    cout << "|";
                    cout.setf(ios::left);
                    cout.width(8);
                    cout << ' ' << warehouse[i].price;
                    cout << "|";
                    
                    cout.width(8);
                    cout.setf(ios::left);
                    cout << ' ' << warehouse[i].count << "|" << '\n';
                }
            }
            FileOut(fileOut, countOfProducts, warehouse, size);
            break;

        case 4:
            FileOut(fileOut, countOfProducts, warehouse, size);
            break;

        default:
            printf("Нет такого пункта\n");
            break;
        }
    } while (answer != 4);
    free(warehouse);
    return 0;

}
