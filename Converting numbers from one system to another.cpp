#include <iostream>
#include <string>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	string X;
	int s1, s2;
	int result = 0;

	while (true) {
		cin >> X;
		cin >> s1;
		cin >> s2;

		int* remainders = new int[32];
		int index = 0;

		for (int i = 0; i < X.size(); i++) {
			int value = X[i] - '0';
			result = result * s1 + value;
		}

		// Закончили с переводом из s1 в 10-ую систему

		while (result > 0) {
			remainders[index] = result % s2;			// остаток
			result = result / s2;
			index++;
		}

		for (int i = index - 1; i >= 0; i--) {
			cout << remainders[i];
		}

		delete[] remainders;

		cout << endl;
	}

	return 0;
}
