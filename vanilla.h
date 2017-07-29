#ifndef VANILLA_H_INCLUDED
#define VANILLA_H_INCLUDED

#include <math.h>
#include <sstream>
#include <vector>
#include <array>
#include <stdlib.h>


int convertTo1(double number) // a method that converts a number to 1 or -1 depending on its sign. If entry is 0, return 1;
{
    if (number == 0)
    {
        return 0;
    }
 //   std::cout << number/fabs(number);
    return number/(fabs(number));
}

template<typename T>
void clearVector(std::vector<T*>& vec, int start, int upTo ) // completely eliminates all elements in a vector;
{
    if (upTo <= 0 || upTo > vec.size())
    {
        upTo = vec.size();
    }
   // int size = vec.size();
    for (int i = start;i < upTo; i ++)
    {
        if (vec[i] != nullptr)
        delete vec[i];
    }
    vec.clear();
}

template<typename T>
std::vector<T> addVector(std::vector<T>& vec1, std::vector<T>& vec2)
{
    std::vector<T> vec;
    int size1 = vec1.size();
    int size2 = vec2.size();
    int i1 =0, i2 = 0;
    while (i1 < size1 || i2 < size2)
    {
        if (i1 < size1)
        {
            vec.push_back(vec1[i1]);
            i1++;
        }
        if (i2 < size2)
        {
            vec.push_back(vec2[i2]);
            i2++;
        }
    }
    return vec;
}

template <typename T>
int arrSize(T arr)
{
    return sizeof(arr)/sizeof(T);
}


template <typename T >
void arrPushBack(T arr[], T blank, T target) //adds an element to an array
{
    int size = arrSize(arr);
    for (int i = 0; i < size; i ++)
    {
        if (arr[i] == blank || arr[i] == NULL)
        {
            arr[i] = target;
        }
    }
}

template <typename T, int size>
void arrPushBack(std::array<T, size>& arr, T blank, T target) //adds an element to an array
{
    for (int i = 0; i < size; i ++)
    {
        if (arr[i] == blank || arr[i] == NULL)
        {
            arr[i] = target;
            break;
        }
    }
}

int findIntLength(int x)
{
    if (x / 10 == 0)
    {
        return 1;
    }
    else
    {
        return 1 + findIntLength(x/10);
    }
}

std::string convert(double input)
{
     std::ostringstream os;
    os << input;
    return os.str();
}

std::string convert(int input) //takes an int and returns the int in string form
{
    std::ostringstream os;
    os << input;
    return os.str();
}

double convert(std::string input)
{
    double j;
     std::istringstream os(input);
    os >> j;
    return j;
}

template<typename T>
class Shelf
{
    T* arr = nullptr;
    int size = 0;

public:
    Shelf(int howBig)
    {
        arr = new T[howBig];
        size = howBig;
    }
    Shelf(std::initializer_list<T> l)
    {
        size = l.size();
        arr = new T[size];
     //   std::initializer_list<T>::iterator it;
     int i= 0;
        for(T it : l)
        {
            arr[i] = it;
       //     std::cout << arr[i] << std::endl;
            i++;
        }
    }
    T get(int location)
    {
        if (location < size)
        {
            return arr[location];
        }
        else
        {
            return nullptr;
        }
    }
    int getSize()
    {
        return size;
    }
    int contains(T ifContains)
    {
     for (int x = 0; x < size; x++)
     {
         if (arr[x] == ifContains)
         {
             return x;
         }
     }
     return -1;
    }
    T replace(int location, T what)
    {
        T returnValue = arr[location];
        arr[location] = what;
        return returnValue;
    }
    void getRidOfDuplicates() //gets rid of all duplicate members. WARNING: may not work because only pointers can be nullptr;
    {
        for (int i = 0; i < size; i++)
        {
            for (int y = 0; y < i; y ++)
            {
                if (arr[i] == arr[y])
                {
                    arr[y] = nullptr;
                }
            }
        }
    }
    void fill(T items)
    {
        for (int i = 0; i < size; i ++)
        {
            arr[i] = items;
        }
    }


};

int charCount(std::string s, char c)
    {
       unsigned int fin = 0;
        for (unsigned int x = 0; x < s.length(); x++)
        {
            if (s[x] == c)
            {
                fin++;
            }
        }
        return fin;
    }
    int gcf(int x, int y)
    {
        x = abs(x);
        y = abs(y);
        if (x== y)
        {
            return x;
        }
        else if (x == 1 || y == 1)
        {
            return 1;
        }
        if (y > x)
        {
            return gcf(x, y-x);
        }
        else
            return gcf(x-y, y);

    }

 std::string* divideString(std::string input) //divides a string into ints
    {
        input +=" ";
        int howBig = charCount(input, ' ') + charCount(input, '@');
        std::string *arr = new std::string[howBig];
        int l = 0;
        std::string seg = "";
        for (unsigned int v = 0; v< input.length(); v++)
        {
            if ((input[v] == ' ' || input[v] == '@' )&& (input[v-1] != ' ' || input[v-1] != '@'))
            {
                arr[l] = (seg);
                l++;
                seg = "";
            }
            else if (input[v] != ' ')
                seg+= input[v];

         //       std::cout << seg << std::endl;
        }
      /*  for (unsigned int u = 0; u < sizeof(arr)/sizeof(int); u ++)
        {
//std::            cout << arr[u] << std::endl;
        }*/
        return arr;
    }

template<typename T>
void addAll(std::vector<T>& a, std::vector<T>& b)
{
    int size = b.size();
    for (int x = 0; x < size; x++)
    {
        a.push_back(b[x]);
    }
}

#endif // VANILLA_H_INCLUDED
