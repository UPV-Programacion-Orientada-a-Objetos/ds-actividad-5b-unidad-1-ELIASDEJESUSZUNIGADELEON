#pragma once

#include "MatrizBase.h"
#include <stdexcept>
#include <iomanip>
#include <iostream>

// Matriz estatica: dimensiones de tiempo de compilacion
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                _datos[i][j] = T{};
    }

    void cargarValores() override {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j) {
                T v{}; std::cin >> v; _datos[i][j] = v;
            }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (M != otra.filas() || N != otra.columnas())
            throw std::runtime_error("Dimensiones incompatibles para suma");
        MatrizEstatica<T, M, N>* r = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                r->_datos[i][j] = _datos[i][j] + otra.get(i, j);
        return r;
    }

    void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j)
                std::cout << std::fixed << std::setprecision(1) << _datos[i][j] << " | ";
            std::cout << '\n';
        }
    }

    T get(int i, int j) const override { return _datos[i][j]; }
    void set(int i, int j, const T& v) override { _datos[i][j] = v; }
};
