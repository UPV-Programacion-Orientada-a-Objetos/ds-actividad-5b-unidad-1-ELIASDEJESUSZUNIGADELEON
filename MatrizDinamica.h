#pragma once

#include "MatrizBase.h"
#include <stdexcept>
#include <iomanip>
#include <iostream>

// Matriz dinamica basada en T** con Regla de los Cinco
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
    T** _datos;

    void reservar(int f, int c) {
        this->_filas = f;
        this->_columnas = c;
        if (f <= 0 || c <= 0) {
            _datos = nullptr;
            return;
        }
        _datos = new T*[f];
        for (int i = 0; i < f; ++i) {
            _datos[i] = new T[c];
            for (int j = 0; j < c; ++j) _datos[i][j] = T{};
        }
    }

    void liberar() {
        if (!_datos) return;
        for (int i = 0; i < this->_filas; ++i) delete[] _datos[i];
        delete[] _datos;
        _datos = nullptr;
        this->_filas = 0;
        this->_columnas = 0;
    }

    void copiarDesde(const MatrizDinamica& o) {
        reservar(o._filas, o._columnas);
        for (int i = 0; i < this->_filas; ++i)
            for (int j = 0; j < this->_columnas; ++j)
                _datos[i][j] = o._datos[i][j];
    }

public:
    MatrizDinamica(int filas, int columnas)
        : MatrizBase<T>(filas, columnas), _datos(nullptr) { reservar(filas, columnas); }

    ~MatrizDinamica() override { liberar(); }

    MatrizDinamica(const MatrizDinamica& o)
        : MatrizBase<T>(o._filas, o._columnas), _datos(nullptr) { copiarDesde(o); }

    MatrizDinamica(MatrizDinamica&& o) noexcept
        : MatrizBase<T>(o._filas, o._columnas), _datos(o._datos) {
        o._datos = nullptr; o._filas = 0; o._columnas = 0;
    }

    MatrizDinamica& operator=(const MatrizDinamica& o) {
        if (this != &o) { MatrizDinamica tmp(o); swap(tmp); }
        return *this;
    }

    MatrizDinamica& operator=(MatrizDinamica&& o) noexcept {
        if (this != &o) { liberar(); this->_filas = o._filas; this->_columnas = o._columnas; _datos = o._datos; o._datos = nullptr; o._filas = 0; o._columnas = 0; }
        return *this;
    }

    void swap(MatrizDinamica& o) noexcept {
        int tf = this->_filas; this->_filas = o._filas; o._filas = tf;
        int tc = this->_columnas; this->_columnas = o._columnas; o._columnas = tc;
        T** td = _datos; _datos = o._datos; o._datos = td;
    }

    void cargarValores() override {
        for (int i = 0; i < this->filas(); ++i)
            for (int j = 0; j < this->columnas(); ++j) {
                T v{}; std::cin >> v; _datos[i][j] = v;
            }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->filas() != otra.filas() || this->columnas() != otra.columnas())
            throw std::runtime_error("Dimensiones incompatibles para suma");
        MatrizDinamica<T>* r = new MatrizDinamica<T>(this->filas(), this->columnas());
        for (int i = 0; i < this->filas(); ++i)
            for (int j = 0; j < this->columnas(); ++j)
                r->_datos[i][j] = _datos[i][j] + otra.get(i, j);
        return r;
    }

    void imprimir() const override {
        for (int i = 0; i < this->filas(); ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->columnas(); ++j)
                std::cout << std::fixed << std::setprecision(1) << _datos[i][j] << " | ";
            std::cout << '\n';
        }
    }

    T get(int i, int j) const override { return _datos[i][j]; }
    void set(int i, int j, const T& v) override { _datos[i][j] = v; }
};
