#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <omp.h>

using namespace std;

// Функция для считывания матрицы из файла
vector<vector<int>> readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> matrix;
    int value;
    while (file >> value) {
        matrix.emplace_back();
        matrix.back().push_back(value);
        while (file.peek() != '\n' && file >> value) {
            matrix.back().push_back(value);
        }
    }
    return matrix;
}

// Функция для записи матрицы в файл
void writeMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    for (const auto& row : matrix) {
        for (int value : row) {
            file << value << '\t';
        }
        file << '\n';
    }
}

// Функция для перемножения матриц с использованием OpenMP с фиксированным количеством потоков
vector<vector<int>> matrixMultiplication(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2, const int size) {
    vector<vector<int>> result(size, vector<int>(size, 0));

#pragma omp parallel for collapse(2) num_threads(20) 
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Чтение матриц из файлов
    vector<vector<int>> matrix1 = readMatrixFromFile("D:/parallel programming/python_pp/matrix_1000x1000.txt"); 
    vector<vector<int>> matrix2 = readMatrixFromFile("D:/parallel programming/python_pp/matrix_1000x1000_1.txt"); 

    // Выполнение перемножения матриц и замер времени
    auto start = chrono::high_resolution_clock::now();
    vector<vector<int>> result = matrixMultiplication(matrix1, matrix2, 1000); 
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    // Запись результата и информации о времени выполнения в файл
    //writeMatrixToFile(result, "D:/parallel programming/result/res_1000_20_threads.txt");
    ofstream timeFile("D:/parallel programming/result_2/stats_20_threads.txt", std::ios::app); 

    timeFile << result.size() << "\t" << duration.count() << "\n";
    cout << "Матрица результатов и информация о времени выполнения записаны в файлы." << endl;

    return 0;
}
