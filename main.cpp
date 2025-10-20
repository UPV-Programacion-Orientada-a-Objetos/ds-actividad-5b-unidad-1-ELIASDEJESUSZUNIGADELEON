#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "MatrizBase.h"
#include "MatrizDinamica.h"
#include "MatrizEstatica.h"

using std::cout;
using std::cin;
using std::endl;

// (La clase base se incluye desde MatrizBase.h)
// (Las clases derivadas se incluyen desde sus headers)

int main() {
	cout << "--- Sistema generico de Algebra Lineal ---" << endl << endl;
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
	MatrizBase<float>* C = (*A) + (*B); // manejado por metodo virtual de A (dinamica)
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

