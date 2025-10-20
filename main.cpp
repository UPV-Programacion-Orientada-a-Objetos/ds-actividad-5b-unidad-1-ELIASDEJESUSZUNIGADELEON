// Actividad: Sistema Genérico de Álgebra Lineal con Jerarquía de Matrices
// Nota: Sin uso de contenedores STL (vector/array). Se usan solo iostream/iomanip y excepciones estándar.

#include <iostream>
#include <iomanip>
#include <stdexcept>

using std::cout;
using std::cin;
using std::endl;

// -------------------------------------------------------------
// Clase base abstracta
// -------------------------------------------------------------
template <typename T>
class MatrizBase {
protected:
	int _filas;
	int _columnas;

	// Ctor protegido para establecer dimensiones desde derivadas
	MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}

public:
	virtual ~MatrizBase() {}

	// Requeridos por enunciado
	virtual void cargarValores() = 0;
	virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
	virtual void imprimir() const = 0;

	// Extensiones mínimas para poder operar genéricamente
	virtual T get(int i, int j) const = 0;
	virtual void set(int i, int j, const T& v) = 0;

	int filas() const { return _filas; }
	int columnas() const { return _columnas; }

	// Sobrecarga de operador + (natural), delega a sumar()
	MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
		return this->sumar(otra);
	}
};

// -------------------------------------------------------------
// Matriz Dinámica: usa T** y Regla de los Cinco
// -------------------------------------------------------------
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
		for (int i = 0; i < this->_filas; ++i) {
			delete[] _datos[i];
		}
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
	// Constructores
	MatrizDinamica(int filas, int columnas)
		: MatrizBase<T>(filas, columnas), _datos(nullptr) {
		reservar(filas, columnas);
	}

	// Destructor
	~MatrizDinamica() override {
		// Mensaje para demostrar limpieza de memoria
		// cout << "Liberando memoria de Matriz Dinámica..." << endl;
		liberar();
	}

	// Regla de los Cinco
	MatrizDinamica(const MatrizDinamica& o)
		: MatrizBase<T>(o._filas, o._columnas), _datos(nullptr) {
		copiarDesde(o);
	}

	MatrizDinamica(MatrizDinamica&& o) noexcept
		: MatrizBase<T>(o._filas, o._columnas), _datos(o._datos) {
		o._datos = nullptr;
		o._filas = 0;
		o._columnas = 0;
	}

	MatrizDinamica& operator=(const MatrizDinamica& o) {
		if (this != &o) {
			// crear copia temporal y luego swap manual
			MatrizDinamica tmp(o);
			swap(tmp);
		}
		return *this;
	}

	MatrizDinamica& operator=(MatrizDinamica&& o) noexcept {
		if (this != &o) {
			liberar();
			this->_filas = o._filas;
			this->_columnas = o._columnas;
			_datos = o._datos;
			o._datos = nullptr;
			o._filas = 0;
			o._columnas = 0;
		}
		return *this;
	}

	void swap(MatrizDinamica& o) noexcept {
		// sin std::swap para respetar restricciones
		int tf = this->_filas; this->_filas = o._filas; o._filas = tf;
		int tc = this->_columnas; this->_columnas = o._columnas; o._columnas = tc;
		T** td = _datos; _datos = o._datos; o._datos = td;
	}

	// Implementación interfaz
	void cargarValores() override {
		for (int i = 0; i < this->filas(); ++i) {
			for (int j = 0; j < this->columnas(); ++j) {
				T v{};
				cin >> v;
				_datos[i][j] = v;
			}
		}
	}

	MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
		if (this->filas() != otra.filas() || this->columnas() != otra.columnas()) {
			throw std::runtime_error("Dimensiones incompatibles para suma");
		}
		MatrizDinamica<T>* r = new MatrizDinamica<T>(this->filas(), this->columnas());
		for (int i = 0; i < this->filas(); ++i)
			for (int j = 0; j < this->columnas(); ++j)
				r->_datos[i][j] = this->_datos[i][j] + otra.get(i, j);
		return r;
	}

	void imprimir() const override {
		for (int i = 0; i < this->filas(); ++i) {
			cout << "| ";
			for (int j = 0; j < this->columnas(); ++j) {
				cout << std::fixed << std::setprecision(1) << _datos[i][j] << " | ";
			}
			cout << endl;
		}
	}

	T get(int i, int j) const override {
		return _datos[i][j];
	}

	void set(int i, int j, const T& v) override {
		_datos[i][j] = v;
	}
};

// -------------------------------------------------------------
// Matriz Estática: usa T datos[M][N]
// -------------------------------------------------------------
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
				T v{};
				cin >> v;
				_datos[i][j] = v;
			}
	}

	MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
		if (M != otra.filas() || N != otra.columnas()) {
			throw std::runtime_error("Dimensiones incompatibles para suma");
		}
		MatrizEstatica<T, M, N>* r = new MatrizEstatica<T, M, N>();
		for (int i = 0; i < M; ++i)
			for (int j = 0; j < N; ++j)
				r->_datos[i][j] = _datos[i][j] + otra.get(i, j);
		return r;
	}

	void imprimir() const override {
		for (int i = 0; i < M; ++i) {
			cout << "| ";
			for (int j = 0; j < N; ++j) {
				cout << std::fixed << std::setprecision(1) << _datos[i][j] << " | ";
			}
			cout << endl;
		}
	}

	T get(int i, int j) const override { return _datos[i][j]; }
	void set(int i, int j, const T& v) override { _datos[i][j] = v; }
};

// -------------------------------------------------------------
// Demo en main: polimorfismo, suma, limpieza memoria
// -------------------------------------------------------------
int main() {
	cout << "--- Sistema generico de Algebra Lineal ---" << endl << endl;

	// Demostracion con float
	cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << endl << endl;
	cout << "Creando Matriz Dinamica A (3x2)..." << endl;
	MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
	// A
	A->set(0, 0, 1.5f); A->set(0, 1, 2.0f);
	A->set(1, 0, 0.0f); A->set(1, 1, 1.0f);
	A->set(2, 0, 4.5f); A->set(2, 1, 3.0f);
	cout << "A =" << endl; A->imprimir(); cout << endl;

	cout << "Creando Matriz Estatica B (3x2)..." << endl;
	MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
	// B
	B->set(0, 0, 0.5f); B->set(0, 1, 1.0f);
	B->set(1, 0, 2.0f); B->set(1, 1, 3.0f);
	B->set(2, 0, 1.0f); B->set(2, 1, 1.0f);
	cout << "B =" << endl; B->imprimir(); cout << endl;

	cout << "SUMANDO: Matriz C = A + B ..." << endl;
	MatrizBase<float>* C = (*A) + (*B); // manejado por método virtual de A (dinámica)
	cout << endl << "Matriz Resultado C (3x2, Tipo FLOAT):" << endl;
	C->imprimir();

	cout << endl << ">> Demostracion de Limpieza de Memoria <<" << endl;
	cout << "Llamando al destructor de C..." << endl; delete C; C = nullptr;
	cout << "Llamando al destructor de A..." << endl; delete A; A = nullptr;
	cout << "Llamando al destructor de B..." << endl; delete B; B = nullptr;

	// Demostracion con int
	cout << endl << ">> Demostracion de Genericidad (Tipo INT) <<" << endl << endl;
	MatrizBase<int>* D = new MatrizDinamica<int>(2, 2);
	MatrizBase<int>* E = new MatrizEstatica<int, 2, 2>();
	// D
	D->set(0, 0, 1); D->set(0, 1, 2);
	D->set(1, 0, 3); D->set(1, 1, 4);
	// E
	E->set(0, 0, 5); E->set(0, 1, 6);
	E->set(1, 0, 7); E->set(1, 1, 8);
	cout << "D =" << endl; D->imprimir(); cout << endl;
	cout << "E =" << endl; E->imprimir(); cout << endl;
	MatrizBase<int>* F = (*D) + (*E);
	cout << "F = D + E:" << endl; F->imprimir();
	delete F; delete D; delete E;

	cout << "Sistema cerrado." << endl;
	return 0;
}

