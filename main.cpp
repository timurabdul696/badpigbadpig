#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

class T
{
private:
    int P;
    int size;
    int* data;

public:
    T() : P(0), size(0), data(nullptr) {}

    T(int p_val, int size_val, int* data_val) : P(p_val), size(size_val)
    {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = data_val[i];
        }
    }

    ~T()
    {
        delete[] data;
    }

    T(const T& other) : P(other.P), size(other.size)
    {
        if (size > 0)
        {
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        else
        {
            data = nullptr;
        }
    }

    T& operator=(const T& other)
    {
        if (this == &other) return *this;
        delete[] data;
        P = other.P;
        size = other.size;
        if (size > 0)
        {
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        else
        {
            data = nullptr;
        }
        return *this;
    }

    T(T&& other) noexcept : P(other.P), size(other.size), data(other.data)
    {
        other.size = 0;
        other.data = nullptr;
        other.P = 0;
    }

    T& operator=(T&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] data;
        P = other.P;
        size = other.size;
        data = other.data;
        other.data = nullptr;
        other.size = 0;
        other.P = 0;
        return *this;
    }

    int getLast() const
    {
        if (size == 0) return 0; // Защита от пустых строк
        return data[size - 1];
    }

    int getAt(int col) const
    {
        return data[col];
    }

    int getSize() const
    {
        return size;
    }

    void removeP()
    {
        int new_size = 0;
        for (int i = 0; i < size; i++) {
            if (data[i] != P) {
                new_size++;
            }
        }

        int* new_data = nullptr;
        if (new_size > 0) {
            new_data = new int[new_size];
            int j = 0;
            for (int i = 0; i < size; i++) {
                if (data[i] != P) {
                    new_data[j++] = data[i];
                }
            }
        }

        delete[] data;
        data = new_data;
        size = new_size;
    }

    void printFile(std::ofstream& fout) const
    {
        for (int i = 0; i < size; i++)
        {
            fout << data[i] << " ";
        }
        fout << "\n";
    }
};

int main()
{
    std::ifstream fin("data.dat");

    if (!fin.is_open())
    {
        std::ofstream fout("data.res");
        fout << "ERROR";
        fout.close();
        return 1;
    }

    std::vector<T*> array;
    std::string line;

    // Чтение файла построчно
    while (std::getline(fin, line)) {
        if (line.empty()) continue; // Пропускаем пустые строки

        std::istringstream stream(line);
        int P;
        
        // Первое число в строке - это P
        if (!(stream >> P)) continue;

        std::vector<int> temp;
        int num;
        while (stream >> num) {
            temp.push_back(num);
        }

        T* obj = new T(P, temp.size(), temp.data());
        
        // Удаляем из строки значения, равные P
        obj->removeP();
        
        array.push_back(obj);
    }
    fin.close();

    // Сортировка строк по убыванию последнего элемента
    for (size_t i = 0; i < array.size() - 1; i++)
    {
        for (size_t j = 0; j < array.size() - 1 - i; j++)
        {
            if (array[j]->getLast() < array[j + 1]->getLast())
            {
                T* temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    std::ofstream fout("data.res");
    
    // Вывод полученного двумерного массива
    for (size_t i = 0; i < array.size(); i++)
    {
        array[i]->printFile(fout);
    }

    // Поиск максимумов по каждому столбцу
    int M = 0;
    for (size_t i = 0; i < array.size(); i++) {
        if (array[i]->getSize() > M) {
            M = array[i]->getSize();
        }
    }

    std::vector<int> max_in_col;
    for (int col = 0; col < M; col++) {
        bool found = false;
        int max_val = 0;
        
        for (size_t row = 0; row < array.size(); row++) {
            if (col < array[row]->getSize()) {
                if (!found) {
                    max_val = array[row]->getAt(col);
                    found = true;
                } else if (array[row]->getAt(col) > max_val) {
                    max_val = array[row]->getAt(col);
                }
            }
        }
        if (found) {
            max_in_col.push_back(max_val);
        }
    }

    // Сортируем массив максимумов по убыванию
    for (size_t i = 0; i < max_in_col.size() - 1; i++) {
        for (size_t j = 0; j < max_in_col.size() - 1 - i; j++) {
            if (max_in_col[j] < max_in_col[j + 1]) {
                int temp = max_in_col[j];
                max_in_col[j] = max_in_col[j + 1];
                max_in_col[j + 1] = temp;
            }
        }
    }

    // Выводим массив максимумов в конец файла
    for (size_t i = 0; i < max_in_col.size(); i++) {
        fout << max_in_col[i] << " ";
    }
    fout << "\n";
    fout.close();

    // Очистка памяти
    for (size_t i = 0; i < array.size(); i++) {
        delete array[i];
    }

    return 0;
}
