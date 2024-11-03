#include <iostream>
#include <chrono>
#include <string>
#include "LongNumber.h"

//Генерация числа 
void generate_random_char_array(char*& string_number, int* length = nullptr, int* base = nullptr)
{
	unsigned seed = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
	srand(seed);
	if (base == nullptr)
	{
		throw std::invalid_argument("base is null");
	}

	if (length == nullptr)
	{
		throw std::invalid_argument("length is null");
	}

	if (*length == 0)
	{
		*length = rand() % 100 + 1;
	}

	if (*base == 0)
	{
		*base = rand() % 15 + 2;
	}

	if (*base < 2 || *base > 16)
	{
		throw std::invalid_argument("generate_random_char_array ERROR! The number system was entered incorrectly, run the program again!"); 
	}

	char alphabet[] = "0123456789ABCDEF";
	string_number = new char[*length + 1];
	for (int i = 0; i < *length; i++)
	{
		string_number[i] = alphabet[rand() % *base];
		//Избегаем ведущего нуля 
		if (i == 0)
		{
			while (string_number[i] == '0')
			{
				string_number[i] = alphabet[rand() % *base];
			}
		}
	}

	string_number[*length] = '\0';
}

int main()
{
	int max_elements = 2560;	//Длина генерируемого числа 
	int base = 16;				//Система счисления числа 
	int number_iterations = 1;	//Количество итераций для получения времени
	double time;				// переменная для хранения времени выполнения
	char* string_number_1;		//Переменная для хранения числа в виде строки

	generate_random_char_array(string_number_1, &max_elements, &base);
	LongNumber number(string_number_1);
	LongNumber sum("0");
	cout << "Generated number: ";
	number.println();
	cout << "Length = " << number.GetLength() << "; Base = "<< base << endl; 

	number.convert(LongNumber(to_string(base).c_str()), &sum);
	cout << "Number in the 10-th number system: ";
	sum.println();	// Вывод результата 

	// Засекаем время начала выполнения 
	auto start = chrono::high_resolution_clock::now();

	for (int i = 1; i <= number_iterations; i++)
	{
		// Вызов метода convert для преобразования 
		number.convert(LongNumber(to_string(base).c_str()), &sum);
	}

	// Засекаем время окончания выполнения 
	auto finish = chrono::high_resolution_clock::now();

	// Вычисляем время выполнения и усредняем его на 10000 итераций
	chrono::duration<double, milli> duration = finish - start;
	time = duration.count() / number_iterations;	// усредняем
	// Выводим результат 
	cout << "Average execution time in milliseconds: " << time << endl;

	return 0;
}
