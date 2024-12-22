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

const int INFINITE_NUM = 2147483647;
const std::string INFINITE_STRING = "OO";

struct Verticle
{
    int label;
    int prevVerticle;
};

int ClearInputBuffer();
int PrintMenu();
int MenuInput();
int PrintSteps(std::vector<Verticle> tempVerticles, std::vector<Verticle> endVerticle);

int ChangeLocalization()
{
    setlocale(LC_CTYPE, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    return 0;
}

std::map<int, std::string> ReadCityNamesFile(std::fstream& cityNamesFile)
{   
    std::string buffer, name, number;
    int index;
    std::map<int, std::string> verticlesNames;

    while (std::getline(cityNamesFile, buffer))
    {   
        name = "";
        number = "";

        for (int i = 0; i < buffer.size(); i++)
        {
            if (buffer[i] >= '0' && buffer[i] <= '9')
            {
                number += buffer[i];
            }
            else if (buffer[i] == ' ')
            {
                index = std::stoi(number);
            }
            else
            {
                name += buffer[i];
            }
        }

        verticlesNames[index] = name;
    }

    return verticlesNames;
}

std::vector<std::vector<int>> ReadCityRoutesFile(std::fstream& citiesRoutesFile, std::map<int, std::string> map)
{   
    std::string buffer;
    int startVerticle, endVerticle, weight;

    std::vector<std::vector<int>> adjencyMatrix(map.size() + 1, std::vector<int>(map.size() + 1));

    while (std::getline(citiesRoutesFile, buffer))
    {
        std::istringstream(buffer) >> startVerticle >> endVerticle >> weight;
        adjencyMatrix[startVerticle][endVerticle] = weight;
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

int SetTempMarks(int verticle, std::vector<Verticle> endVerticles, std::vector<Verticle>& tempVerticles, std::vector<std::vector<int>>adjencyMatrix) 
{   
    int min;
    int max;

    for (int i = 1; i != adjencyMatrix[verticle].size(); ++i) 
    {
        if (i != verticle && adjencyMatrix[verticle][i] != 0) 
        {
            min = MinNum(endVerticles[verticle].label, adjencyMatrix[verticle][i]);
            max = MaxNum(min, tempVerticles[i].label);

            if (tempVerticles[i].label < max)
            {
                tempVerticles[i].label = max;
                tempVerticles[i].prevVerticle = verticle;
            }
                
        }
    }

    return 0;
}

int FindMaxVerticle(std::vector<Verticle> endVerticles, std::vector<Verticle> tempVerticles)
{
    int max, maxVerticleNum;
    
    max = 0;
    maxVerticleNum = 0;

    for (int i = 1; i < tempVerticles.size(); ++i)
    {
        if (tempVerticles[i].label >= max && isNotVisited(endVerticles[i].label))
        {
            max = tempVerticles[i].label;
            maxVerticleNum = i;
        }
    }

    return maxVerticleNum;
}

std::vector<Verticle> FindMaxWeight(int routeBegin, int routeEnd, std::vector<std::vector<int>> adjencyMatrix)
{
    std::vector<Verticle> endVerticles, tempVerticles;

    for (int i = 0; i < adjencyMatrix.size(); ++i) 
    {   
        Verticle newVerticle;
        newVerticle.label = -1;
        newVerticle.prevVerticle = 0;

        tempVerticles.push_back(newVerticle);

        if (i == routeBegin) 
            tempVerticles[i].label = INFINITE_NUM;
    }

    endVerticles = tempVerticles;
    int lastVisitedVerticle = routeBegin;
    int count = 0;

    PrintSteps(tempVerticles, endVerticles);

    while (endVerticles[routeEnd].label == -1)
    {
        SetTempMarks(lastVisitedVerticle, endVerticles, tempVerticles, adjencyMatrix);
        PrintSteps(tempVerticles, endVerticles);
        lastVisitedVerticle = FindMaxVerticle(endVerticles, tempVerticles);
        endVerticles[lastVisitedVerticle] = tempVerticles[lastVisitedVerticle];

        if (endVerticles[lastVisitedVerticle].label == -1)
            return endVerticles;

        if (lastVisitedVerticle == routeEnd)
        {
            PrintSteps(endVerticles, tempVerticles);
        }
    }

    endVerticles = tempVerticles;
    return endVerticles;
}

int PrintSteps(std::vector<Verticle> tempVerticles, std::vector<Verticle> endVerticles)
{   
    std::cout << "|";
    for (int i = 1; i < tempVerticles.size(); ++i)
    {   
        if (endVerticles[i].label == INFINITE_NUM)
        {
            std::cout << "C " << INFINITE_STRING;
        }
        else if (endVerticles[i].label != -1)
        {
            std::cout << "C " << endVerticles[i].label << '(' << endVerticles[i].prevVerticle << ')';
        }
        else
        {
            std::cout << "D " << tempVerticles[i].label;
            if (tempVerticles[i].label != -1)
                std::cout << '(' << tempVerticles[i].prevVerticle << ')';
        }
        std::cout << "|";
    }

    std::cout << std::endl;

    return 0;
}

void PrintRoute(std::vector<Verticle> route, Verticle verticle, std::map<int, std::string> citiesNames)
{   
    std::vector<std::string> routeNames;

    while (verticle.prevVerticle != 0) {
        int n = verticle.prevVerticle;
        routeNames.push_back(citiesNames[n]);
        verticle = route[verticle.prevVerticle];
    }

    for (auto it = routeNames.rbegin(); it != routeNames.rend(); ++it) {
        std::cout << *it << "->";
    }
}

int main(int argc, char* argv[])
{   
    int routeBegin, routeEnd;
    bool isUsing = true;
    std::vector<Verticle> route;

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
    std::vector<std::vector<int>> weightMatrix = ReadCityRoutesFile(citiesRoutesFile, citiesNamesMap);

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
                route = FindMaxWeight(routeBegin, routeEnd, weightMatrix);

                if (route[routeEnd].label == -1)
                {
                    std::cout << "Дороги из " << citiesNamesMap[routeBegin] << " в " << citiesNamesMap[routeEnd] << " не существует, попробуйте другой маршрут";
                    std::cout << std::endl;
                }     
                else
                {
                    std::cout << "Максимальный груз из города " << citiesNamesMap[routeBegin] << " в " << citiesNamesMap[routeEnd] << " = " << route[routeEnd].label;
                    std::cout << std::endl;

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
    std::cout << "2. Выйти из программы" << std::endl;

    return 0;
}

int MenuInput()
{   
    int mode;
    std::cin >> mode;
    
    return mode;
}
