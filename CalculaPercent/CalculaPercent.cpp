// CalculaPercent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>

#define TITLE "CALCULO DE COEFICIENTES DE PROPIEDAD"
#define AUTHOR "Bernat Gendra"
#define CONTACT "bernat.gendra@gmail.com"
#define DATE "25-12-2017"

using namespace std;

void printHeader();
vector<double> repartePorcentajes(vector<double> partes, int decimales = 2);
double sumVector(vector<double> vector);
vector<int> findNMax(vector<double> lista, int N = 1);
string getText(const vector<double> &partes, const vector<double> &percents);
string saveToFile(string text);

int main()
{
	double supTotal;
	int numPartes;
	vector<double> partes;
	vector<double> percents;
	bool check = false;
	int userIn;

	printHeader();

	cout << "Introduce la superficie total del edificio" << endl;
	cin >> supTotal;
	cout << "Cuantos elementos quieres introducir?" << endl;
	cin >> numPartes;
	cout << endl;

	partes.resize(numPartes);
	percents.resize(numPartes);

	while (!check) {
		double sum{ 0 };
		for (int i = 0; i < numPartes; i++) {
			cout << "Introduce el elemento " << i+1 << endl;
			cin >> partes[i];
			sum += partes[i];
		}
		if (sum == supTotal) {
			check = true;
		}
		else {
			cout << "La suma de las superficies de los elementos no se corresponde con el total.";
			cout << endl << "Por favor, vuelve a introducirlas." << endl;
		}
	}
	percents = repartePorcentajes(partes);	
	
	cout << endl;
	cout << "Has introducido una superficie total de " << supTotal;
	cout << " con " << numPartes << " elementos:" << endl;
	
	cout << endl;
	string text = getText(partes, percents);
	cout << text << endl;
	bool correct = false;

	while (!correct) {
		correct = true;
		cout << "Quieres (1) solo guardarlo en un fichero o (2) tambien copiarlo en el portapapeles" << endl;
		cin >> userIn;
		string cmd;
		string file;
		switch (userIn)
		{
		case 1:
			file = saveToFile(text);
			cmd = "notepad " + file + ".txt";
			system(cmd.c_str());
			break;

		case 2:
			file = saveToFile(text);
			cmd = "type " + file + ".txt | clip";
			system(cmd.c_str());
			break;
		
		default:
			correct = false;
		}
	}
	
	
	//system("pause");
	return 0;
}

void printHeader()
{
	string title = TITLE;
	int len = title.length() + 2 ;

	for (int i = 0; i < len; i++) {
		cout << "*";
	}
	cout << endl << " " << title << endl;
	for (int i = 0; i < len; i++) {
		cout << "_";
	}
	cout << endl;
	cout << " Autor: " << AUTHOR << endl;
	cout << " e-mail: " << CONTACT << endl;
	cout << " Fecha: " << DATE << endl;
	for (int i = 0; i < len; i++) {
		cout << "*";
	}
	cout << endl << endl;
}

// Compute percentages using Hamilton method to ensure that add up to 100%
vector<double> repartePorcentajes(vector<double> partes, int decimales)
{
	int unidades , faltan;
	int numPartes = partes.size();
	double cuota, total;
	vector<double> percents , residuos;
	vector<int> incs;

	percents.resize(numPartes);
	residuos.resize(numPartes);

	unidades = 100 * pow(10, decimales);
	total = sumVector(partes);
	cuota = unidades / total;

	for (int i = 0; i < numPartes; i++) {
		percents[i] = floor(partes[i] * cuota);
		residuos[i] = partes[i] - percents[i];
	}
	faltan = unidades - static_cast<int>(sumVector(percents));
	incs = findNMax(residuos, faltan);
	for (int i = 0; i < numPartes; i++) {
		percents[i] = (percents[i] + static_cast<double>(incs[i])) / pow(10, decimales);
	}
	return percents;
}


double sumVector(vector<double> vector)
{
	double sum{ 0 };
	for (int i = 0; i < vector.size(); i++) {
		sum += vector[i];
	}
	return sum;
}

vector<int> findNMax(vector<double> lista, int N)
{
	vector<int> inc, ind;
	int finded{ 0 };
	int aux;
	int size = lista.size();
	inc.resize(size);
	ind.resize(N);
	// Check every element
	for (int i = 0; i < size; i++) {
		aux = i;
		// Compare it with the stored indices, and include it if necessary, keeping the list sorted
		for (int j = 0; j < finded; j++) {
			if (lista[aux] > lista[ind[j]]) {
				swap(aux, ind[j]);
			}
		}
		// If there are not enough stored indices, included aux at the end
		if (finded < N) {
			ind[finded] = aux;
			finded++;
		}
	}
	// Construct an array with ones in the finded indices
	for (int i = 0; i < N; i++) {
		inc[ind[i]] = 1;
	}
	return inc;
}

string getText(const vector<double>& partes, const vector<double>& percents)
{
	string str;
	std::ostringstream strs;
	int size = partes.size();
	for (int i = 0; i < size; i++) {
		strs << "Cuota: el elemento " << (i + 1);
		strs << " tiene una cuota de participación en los beneficios y gastos del " << percents[i] << "%.\n";
	}
	return strs.str();
}

string saveToFile(string text)
{
	cin.ignore(100, '\n');
	cout << "Introduce el nombre del fichero sin extensión (por defecto: outfile):" << endl;
	string file;
	getline(cin, file);
	if (file.empty()) {
		file = "outfile";
	}
	ofstream outf(file + ".txt");

	if (outf) {
		outf << text;
	}
	else {
		cout << "Ha habido un problema guardando los resultados en el fichero" << endl;
	}
	outf.close();

	return file;
}
