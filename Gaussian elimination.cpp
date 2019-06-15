#include <locale>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <math.h>
using namespace std;

void printSystem(float** m, float* v, int n)	//вывод системы
{
	cout << "Ax = b" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout.width(10);		cout << setprecision(3) << m[i][j];
		}
		cout.width(15);		cout << "x" << i + 1;
		if (i == (n / 2))
		{
			cout.width(5);		cout << "=";
		}
		else
		{
			cout.width(5);		cout << " ";
		}
		cout.width(10);		cout << v[i] << endl;
	}
}

void main()
{
	int N;
	setlocale(LC_ALL, "Rus");
	ifstream fin;
	ofstream fout;
	fin.open("input.txt");
	if (!fin.is_open())
		cout << "Ошибка открытия файла" << endl;
	else
	{
		fin >> N;
		float** a = new float* [N];
		for (int i = 0; i < N; i++)
			a[i] = new float[N];
		float* b = new float[N];
		float* x = new float[N];
		int* right_index = new int[N];

		for (int i = 0; i < N; i++)		//вектор от 0 до N-1 для сохранения правильной
			right_index[i] = i;			//последовательности решений Х

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
				fin >> a[i][j];
			fin >> b[i];
		}
		fin.close();
		
		cout << "Система в матричной форме" << endl;
		printSystem(a, b, N);
		
		//     основной цикл метода Гаусса с выбором главных элементов
		bool F = false;
		int k = 0;
		while ((k < N) && (F == false))
		{
			float max_el = a[k][k];
			int m_i = k;				//переменные для поиска максимального элемента
			int m_j = k;				// в матрице
			for (int i = k; i < N; i++)
			{
				for (int j = k; j < N; j++)			//поиск максимального элемента
				{
					if (abs(a[i][j]) > abs(max_el))
					{
						max_el = a[i][j];
						m_i = i;
						m_j = j;
					}
				}
			}
			if (abs(max_el) < 1E-10)		//проверка является ли макс элемент очень маленьким и близким к 0
			{								// если да, то метод не сойдется к ответу ввиду того, что будет 
				cout << "Ошибка вычислений" << endl;	//накапливаться ошибка
				F = true;
			}
			if (F == false)
			{				//берем позицию макс элемента и меняем строчки или столбцы в зависимости от позиции так чтобы
																							//макс элемент матрицы
				if (m_i != k)																//оказался на главной диагонали
				{																	//для последующего выполнения
					swap(b[k], b[m_i]);	//замена вектора В							//прямого хода метода Гаусса
					for (int j = k; j < N; j++)
						swap(a[k][j], a[m_i][j]);	//замена строк матрицы А
					if (m_j != k)
					{
						swap(right_index[k], right_index[m_j]);		//замена индексов Х при перестановке столбцов
						for (int i = 0; i < N; i++)
							swap(a[i][k], a[i][m_j]);		//замена столбцов матрицы А
					}
				}
				else if (m_j != k)
				{
					swap(right_index[k], right_index[m_j]);		//замена индексов Х при перестановке столбцов
					for (int i = 0; i < N; i++)
						swap(a[i][k], a[i][m_j]);			//замена столбцов матрицы А
				}
				//   прямой ход метода Гаусса
				for (int i = k + 1; i < N; i++)
				{
					float t = a[i][k] / a[k][k];	//расчет по известным формулам
					b[i] = b[i] - t * b[k];
					for (int j = k; j < N; j++)
						a[i][j] = a[i][j] - t * a[k][j];
				}
			}
			k++;
		}
		if (F == false)
		{
			//  обратный ход метода Гаусса
			float* x0 = new float[N];
			x0[N - 1] = b[N - 1] / a[N - 1][N - 1]; //вычисление последнего Х
			for (int k = N - 1; k >= 0; k--)
			{
				float s = 0;						//вычисление остальных Х
				for (int j = k + 1; j < N; j++)
					s = s + a[k][j] * x0[j];			//расчет по известным формулам
				x0[k] = (b[k] - s) / a[k][k];
			}
			for (int i = 0; i < N; i++)		//восстановление индектов Х для восстановления
				x[right_index[i]] = x0[i];	//првильной последовательности решений Х
			for (int i = 0; i < N; i++)			//индексы смещаются и нарушается послед-ть только при перестановке столбцов
			{
				if ((x[i] == -0) || (abs(x[i]) < 0.0001))
					x[i] = 0;					//данные действия совершаются для правильного отображения 0
			}
			delete[] x0;

			cout << endl << "Решение системы" << endl;

			for (int i = 0; i < N; i++)
				cout << "x" << i + 1 << "= " << x[i] << endl;
		}

		for (int i = 0; i < N; i++)
			delete[] a[i];

		delete[] x;
		delete[] right_index;
		delete[] b;
	}
	system("pause");
}
