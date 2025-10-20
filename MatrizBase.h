#pragma once
// Clase base abstracta para matrices genericas
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

    // Constructor protegido para inicializar dimensiones desde derivadas
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}

public:
    virtual ~MatrizBase() {}

    // Metodos puros requeridos
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    // Acceso generico a elementos (obligatorio en derivadas)
    virtual T get(int i, int j) const = 0;
    virtual void set(int i, int j, const T& v) = 0;

    // Dimensiones
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }

    // Operador + natural, delega a sumar()
    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
};
