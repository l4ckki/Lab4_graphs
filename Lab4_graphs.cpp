/* Кулаков Роман Андреевич ПС-22
* Visual Studio 2022
*    18. Имеется  сеть  автомобильных  дорог.  Для каждой дороги
известна максимальная масса груза, которую можно  провезти  по 
этой   дороге.   С   помощью  алгоритма  Дейкстры   определить 
максимальный   груз,   который  можно  провезти  между   двумя 
указанными городами. Проиллюстрировать в таблице по шагам этапы
поиска (9).
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>

#define NOMINMAX
#include <Windows.h>

const int INFINITE_NUM = 2147483647; // Максимальное значение для int
const std::string INFINITE_SYMBOL = "OO";

struct TableVerticle
{
    int label;
    int prevTableVerticle;
};

int ClearInputBuffer();
int PrintMenu();
int MenuInput();
int PrintSteps(std::vector<TableVerticle> tempTableVerticles, std::vector<TableVerticle> endTableVerticle);

int ChangeLocalization()
{
    setlocale(LC_CTYPE, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    return 0;
}

std::map<int, std::string> ReadCityNamesFile(std::fstream& cityNamesFile)
{   
    std::string name;
    int index;
    std::map<int, std::string> verticlesNames;

    index = 1;

    while (std::getline(cityNamesFile, name))
    {   
        verticlesNames[index] = name;
        index++;
    }

    return verticlesNames;
}

std::vector<std::vector<int>> ReadCityRoutesFile(std::fstream& citiesRoutesFile, std::map<int, std::string> map)
{   
    std::string buffer, number;
    int count, index, startTableVerticle, endTableVerticle, weight;

    std::vector<std::vector<int>> adjencyMatrix(map.size() + 1, std::vector<int>(map.size() + 1));
    count = 0;
    index = 0;

    while (std::getline(citiesRoutesFile, buffer))
    {   
        std::istringstream(buffer) >> startTableVerticle >> endTableVerticle >> weight;
        adjencyMatrix[startTableVerticle][endTableVerticle] = weight;
    }

    return adjencyMatrix;
}

int MinNum(int num1, int num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    else
    {
        return num2;
    }
}

int MaxNum(int num1, int num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    else
    {
        return num2;
    }
}

bool isNotVisited(int verticle)
{
    if (verticle == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SetTempMarks(int verticle, std::vector<TableVerticle> endTableVerticles, std::vector<TableVerticle>& tempTableVerticles, std::vector<std::vector<int>>adjencyMatrix) 
{   
    int min;
    int max;

    for (int i = 1; i != adjencyMatrix[verticle].size(); ++i) 
    {
        if (i != verticle && adjencyMatrix[verticle][i] != 0) 
        {
            min = MinNum(endTableVerticles[verticle].label, adjencyMatrix[verticle][i]);
            max = MaxNum(min, tempTableVerticles[i].label);

            if (tempTableVerticles[i].label < max)
            {
                tempTableVerticles[i].label = max;
                tempTableVerticles[i].prevTableVerticle = verticle;
            }
                
        }
    }

    return 0;
}

int FindMaxTableVerticle(std::vector<TableVerticle> endTableVerticles, std::vector<TableVerticle> tempTableVerticles)
{
    int max, maxTableVerticleNum;
    
    max = 0;
    maxTableVerticleNum = 0;

    for (int i = 1; i < tempTableVerticles.size(); ++i)
    {
        if (tempTableVerticles[i].label >= max && isNotVisited(endTableVerticles[i].label))
        {
            max = tempTableVerticles[i].label;
            maxTableVerticleNum = i;
        }
    }

    return maxTableVerticleNum;
}

std::vector<TableVerticle> SetLabels(std::vector<std::vector<int>> adjencyMatrix, int routeBegin)
{   
    std::vector<TableVerticle> lableTable;

    for (int i = 0; i < adjencyMatrix.size(); ++i)
    {
        TableVerticle newTableVerticle;
        newTableVerticle.label = -1;
        newTableVerticle.prevTableVerticle = 0;

        lableTable.push_back(newTableVerticle);

        if (i == routeBegin)
            lableTable[i].label = INFINITE_NUM;
    }

    return lableTable;
}

std::vector<TableVerticle> FindMaxWeight(int routeBegin, int routeEnd, std::vector<std::vector<int>> adjencyMatrix)
{
    std::vector<TableVerticle> endTableVerticles, tempTableVerticles;
    int lastVisitedTableVerticle;

    tempTableVerticles = SetLabels(adjencyMatrix, routeBegin);
    endTableVerticles = tempTableVerticles;
    lastVisitedTableVerticle = routeBegin;

    PrintSteps(tempTableVerticles, endTableVerticles);

    while (endTableVerticles[routeEnd].label == -1)
    {
        SetTempMarks(lastVisitedTableVerticle, endTableVerticles, tempTableVerticles, adjencyMatrix);
        PrintSteps(tempTableVerticles, endTableVerticles);
        lastVisitedTableVerticle = FindMaxTableVerticle(endTableVerticles, tempTableVerticles);
        endTableVerticles[lastVisitedTableVerticle] = tempTableVerticles[lastVisitedTableVerticle];

        if (endTableVerticles[lastVisitedTableVerticle].label == -1)
            return endTableVerticles;

        if (lastVisitedTableVerticle == routeEnd)
        {
            PrintSteps(endTableVerticles, tempTableVerticles);
        }
    }

    endTableVerticles = tempTableVerticles;

    return endTableVerticles;
}

int PrintSteps(std::vector<TableVerticle> tempTableVerticles, std::vector<TableVerticle> endTableVerticles)
{   
    std::cout << "|";
    for (int i = 1; i < tempTableVerticles.size(); ++i)
    {   
        if (endTableVerticles[i].label == INFINITE_NUM)
        {
            std::cout << "C " << INFINITE_SYMBOL;
        }
        else if (endTableVerticles[i].label != -1)
        {
            std::cout << "C " << endTableVerticles[i].label << '(' << endTableVerticles[i].prevTableVerticle << ')';
        }
        else
        {
            std::cout << "D " << tempTableVerticles[i].label;
            if (tempTableVerticles[i].label != -1)
                std::cout << '(' << tempTableVerticles[i].prevTableVerticle << ')';
        }
        std::cout << "|";
    }

    std::cout << std::endl;

    return 0;
}

void PrintRoute(std::vector<TableVerticle> route, TableVerticle verticle, std::map<int, std::string> citiesNames)
{   
    std::vector<std::string> routeNames;

    while (verticle.prevTableVerticle != 0) {
        int n = verticle.prevTableVerticle;
        routeNames.push_back(citiesNames[n]);
        verticle = route[verticle.prevTableVerticle];
    }

    for (auto it = routeNames.rbegin(); it != routeNames.rend(); ++it) {
        std::cout << *it << "->";
    }
}

int PrintGraphTable(std::vector<std::vector<int>> adjencyMatrix, std::map<int, std::string> citiesNamesMap)
{   
    std::cout << "Заданные маршруты графа: " << std::endl << std::endl;
    for (int i = 0; i < adjencyMatrix.size(); i++)
    {
        for (int j = 0; j < adjencyMatrix[i].size(); j++)
        {   
            if (adjencyMatrix[i][j] != 0)
            {
                std::cout << "Маршрут из " << citiesNamesMap[i] << "(" << i << ")";
                std::cout << " в " << citiesNamesMap[j] << "(" << j << ")" << " равен " << adjencyMatrix[i][j] << std::endl;
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{   
    int routeBegin, routeEnd;
    bool isUsing = true;
    std::vector<TableVerticle> route;

    ChangeLocalization();

    if (argc != 3)
    {
        std::cout << "Недостаточно параметров" << std::endl;
        return -1;
    }
    
    std::fstream citiesNamesFile(argv[1]);
    std::fstream citiesRoutesFile(argv[2]);

    if (!citiesNamesFile || !citiesRoutesFile)
    {
        std::cout << "Один или оба файла не открылись" << std::endl;

        return -2;
    }

    std::map<int, std::string> citiesNamesMap = ReadCityNamesFile(citiesNamesFile);
    std::vector<std::vector<int>> adjencyMatrix = ReadCityRoutesFile(citiesRoutesFile, citiesNamesMap);

    citiesNamesFile.close();
    citiesRoutesFile.close();

    while (isUsing)
    {   
        PrintMenu();
        
        int mode = MenuInput();

        if (mode == 1)
        {   
            std::cout << "Введите начало маршрута - ";
            std::cin >> routeBegin;
            std::cout << "Введите конец маршрута - ";
            std::cin >> routeEnd;
            std::cout << std::endl;
            if (routeBegin == routeEnd)
            {
                std::cout << "Вы выбрали один и тот же город, попробуйте другой маршрут" << std::endl;
            }
            else if (citiesNamesMap.count(routeBegin) && citiesNamesMap.count(routeEnd))
            {
                route = FindMaxWeight(routeBegin, routeEnd, adjencyMatrix);

                std::cout << std::endl;

                if (route[routeEnd].label == -1)
                {
                    std::cout << "Дороги из " << citiesNamesMap[routeBegin] << " в " << citiesNamesMap[routeEnd] << " не существует, попробуйте другой маршрут";
                    std::cout << std::endl;
                }     
                else
                {
                    std::cout << "Максимальный груз из города " << citiesNamesMap[routeBegin] << " в " << citiesNamesMap[routeEnd] << " = " << route[routeEnd].label;
                    std::cout << std::endl << std::endl;

                    std::cout << "Маршрут: ";
                    PrintRoute(route, route[routeEnd], citiesNamesMap);
                    std::cout << citiesNamesMap[routeEnd] << std::endl;
                }
            }
            else
            {   
                std::cout << "Одного или двух городов нет в списке, попробуйте другой маршрут" << std::endl;
                ClearInputBuffer();
            }

            std::cout << std::endl;
        }
        else if (mode == 2)
        {
            PrintGraphTable(adjencyMatrix, citiesNamesMap);
            std::cout << std::endl;
        }
        else if (mode == 3)
        {
            isUsing = false;
        }
        else
        {
            ClearInputBuffer();
        }
    }

    return 0;
}

int ClearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return 0;
}

int PrintMenu()
{
    std::cout << "1. Выбрать маршрут для расчета" << std::endl;
    std::cout << "2. Вывести все маршруты графа" << std::endl;
    std::cout << "3. Выйти из программы" << std::endl;
    std::cout << std::endl;

    return 0;
}

int MenuInput()
{   
    int mode;
    std::cin >> mode;
    
    return mode;
}
