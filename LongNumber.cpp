#include "LongNumber.h"
#include <string>

//PRIVATE 
//Перевыделение памяти 
void LongNumber::enscure_size(int new_length) 
{
    size = capacity = new_length + 1;
    char* new_storage = new char[size];
    for (int i = 0; i < new_length; i++)
    {
        //Запись числа и зануление всех цифр разряд, которых больше длины исходного числа
        if (i < length) new_storage[i] = storage[i];
        else new_storage[i] = '0';
    }

    new_storage[length] = '\0';
    delete[] storage;
    storage = new char[size];
    for (int i = 0; i < length; i++) storage[i] = new_storage[i];
    delete[] new_storage;
}

//Вспомогательная функция для convert (вычисление длины строки)
int LongNumber::my_strlen(const char* str) 
{
    int length = 0;
    while (str[length] != '\0') length++;
    return length;
}

//PUBLIC 
//Конструктор по умолчанию 
LongNumber::LongNumber() 
{
    length = size = capacity = 0;
    storage = nullptr;
}

// Конструктор с начальной емкостью 
LongNumber::LongNumber(int len) 
{
    size = capacity = len + 1;
    length = len;
    storage = new char[size];
    for (int i = 0; i < length; i++) {
        storage[i] = '0';
    }

    storage[length] = '\0';
}

// Конструктор из строки в число
LongNumber::LongNumber(const char* char_array_number)
{
    if (char_array_number == nullptr) {
        throw std::invalid_argument("char_array_number pointer is null !(LongNumber(const char* char_array_number))!");
    }

    length = 0;
    while (char_array_number[length] != '\0') length++;
    //Получение длины строки 
    size = capacity = length + 1;
    storage = new char[size];
    int j = length;

    //Заполняем в обратном порядке 
    for (int i = 0; i < length; i++) {
        storage[i] = char_array_number[j - 1];
        j--;
    }

    storage[size - 1] = '\0';
}

//Конструктор копирования 
LongNumber::LongNumber(const LongNumber& other)
{
    length = other.length;
    capacity = other.capacity;
    size = other.size;
    storage = new char[capacity];
    for (int i = 0; i < length; i++) storage[i] =
        other.storage[i];
    storage[length] = '\0';
}

//Деструктор по умолчанию 
LongNumber::~LongNumber()
{
    delete[] storage;
    length = size = capacity = 0;
    storage = nullptr;
}

//Getter для получения длины числа 
int LongNumber::GetLength() const {
    return length;
}

//Операторы 

//Оператор присваивания 
LongNumber& LongNumber::operator = (const LongNumber& other) {
    if (this == &other) return *this; //Если число присваивается самосебе

    length = other.length;
    capacity = other.capacity;
    size = other.size;
    delete[] storage;
    storage = new char[capacity];
    for (int i = 0; i < length; i++) storage[i] = other.storage[i];
    storage[length] = '\0';
    return *this;
}

//Оператор сложения 
void LongNumber::operator += (const LongNumber& others) 
{
    LongNumber other = others;
    if (*this < others) {
        other = *this;
        *this = others;
    }

    int max_size_sum = max(length, other.length) + 1;
    enscure_size(max_size_sum);

    int carry = 0; //Переносимый разряд 
    for (int i = 0; i < capacity; i++) {
        //Получение цифр числа 
        int storage1 = (i < length) ? (storage[i] - '0') : 0;
        int storage2 = (i < other.length) ? (other.storage[i] - '0') : 0;

        //Расчет суммы из которой будут переносить 
        int sum = storage1 + storage2 + carry;

        carry = sum / 10;
        storage[i] = (sum % 10) + '0';
    }

    if (storage[length] != '0') length++;
    else enscure_size(length);
    storage[length] = '\0';
}

//Оператор вычитания 
void LongNumber::operator -= (const LongNumber& other) 
{
    //Проверка Уменьшаемое > Вычитаемого 
    if (*this < other) throw std::invalid_argument("Subtraction ERROR! \n The minuend cannot be less than the subtrahend!");
    if (*this == other) {
        LongNumber temp("0");
        *this = temp;
    }
    else {
        int borrow = 0;
        for (int i = 0; i < length; i++) {
            //Получение цифр чисел 
            int digit1 = (i < length) ? (storage[i] - '0') : 0;
            int digit2 = (i < other.length) ? (other.storage[i] - '0') : 0;

            int diff = digit1 - digit2 - borrow;
            borrow = (diff < 0) ? 1 : 0;
            //Переносимый разряд 
            if (borrow) diff += 10;

            storage[i] = diff + '0';
        }

        int LEADING_ZERO = -1; //Удаление ведущих нулей 
        for (int i = length; i > 0; i--) {
            if (storage[i - 1] == '0') {
                LEADING_ZERO += 1;
            }
            else break;
        }

        if (LEADING_ZERO != -1) {
            length = length - LEADING_ZERO - 1;
            enscure_size(length);
        }
        else storage[length] = '\0';
    }
}

//Оператор умножения 
void LongNumber::operator *= (const LongNumber& other)
{
    if ((*this == LongNumber("0")) || (other == LongNumber("0")))
        *this = LongNumber("0");
    else {
        LongNumber result(length + other.length); // создаем объект с нужным размером
        for (int i = 0; i < length; i++) {
            int carry = 0;
            for (int j = 0; j < other.length; j++) {
                int digit1 = storage[i] - '0'; // преобразуем символ в число
                int digit2 = other.storage[j] - '0'; // аналогично преобразуем

                // Суммируем текущий результат 
                int current_result = result.storage[i + j] - '0';
                int mul = digit1 * digit2 + carry + current_result;

                // Обновляем текущую позицию в результате
                result.storage[i + j] = (mul % 10) + '0';
                carry = mul / 10; // сохраняем перенос 
            }

            // Если остался перенос, добавляем его в следующую позицию
            result.storage[i + other.length] += carry;
        }

        int LEADING_ZERO = -1; //Удаление ведущих нулей 
        for (int i = result.length; i > 0; i--) {
            if (result.storage[i - 1] == '0') {
                LEADING_ZERO += 1;
            }
            else break;
        }

        if (LEADING_ZERO != -1) {
            result.length = result.length - LEADING_ZERO - 1;
            result.enscure_size(result.length);
        }
        else result.storage[result.length] = '\0';

        *this = result; // присваиваем результат текущему объекту
    }
}

//Оператор возведения в степень 
LongNumber LongNumber::power(int exp) const {
    if (exp < 0) throw std::invalid_argument("Power ERROR!");

    LongNumber result("1");
    if (exp == 0) return result;
    LongNumber base = *this;

    while (exp > 0) {
        if (exp % 2 == 1) {
            // если степень нечетная 
            result *= base;
        }

        base *= base; // возводим основание в квадрат 
        exp /= 2; // делим степень пополам 
    }

    return result;
}

//Логические операторы сравнения 

//Оператор строго меньше<
bool LongNumber::operator < (const LongNumber& other) const {
    if (length < other.length) return true;
    if (length > other.length) return false;
    for (int i = length; i > 0; i--) {
        if (storage[i - 1] < other.storage[i - 1]) 
            return true;
        if (storage[i - 1] > other.storage[i - 1])
            return false;
    }

    return false;
}

//Оператор строго больше > 
bool LongNumber::operator > (const LongNumber& other) const {
    return other < *this;
}

//Оператор меньше либо равно <= 
bool LongNumber::operator <= (const LongNumber& other) const {
    return !(*this > other);
}

//Оператор больше либо равно >= 
bool LongNumber::operator >= (const LongNumber& other) const {
    return !(*this < other);
}

//Оператор равно == 
bool LongNumber::operator == (const LongNumber& other) const {
    if (length != other.length) return false;

    for (int i = 0; i < length; i++)
    {
        if (storage[i] != other.storage[i]) return false;
    }

    return true;
}

//Оператор не равно != 
bool LongNumber::operator != (const LongNumber& other) const {
    return !(*this == other);
}

//Функция вывода числа на экран без переноса строки 
void LongNumber::print() const {
    if (size == 0) throw std::invalid_argument("LongNumber is empty!");
    for (int i = length; i > 0; i--) cout << storage[i - 1];
}

//Функция вывода числа на экран c переносом строки 
void LongNumber::println() const {
    if (size == 0) throw std::invalid_argument("LongNumber is empty!");

    for (int i = length; i > 0; i--) cout << storage[i - 1];
    cout << endl;
}

//Перевод LongNumber в строку 
char* LongNumber::toCharArray() const 
{
    char* result = new char[length + 1];
    for (int i = 0; i < length; i++) result[i] = storage[length - 1 - i];
    result[length] = '\0';
    return result;
}

//Перевод от 2-ой по 16-ую систему счисления в десятичную
int LongNumber::convert(const LongNumber& notation, LongNumber* summary) 
{
    LongNumber len(to_string(length).c_str());
    LongNumber len_tmp(to_string(length - 1).c_str());
    int i_int = 0;
    LongNumber coefficient("0");
    LongNumber sum("0");

    if (notation < LongNumber("2") || notation > LongNumber("16"))
    {
        throw std::invalid_argument("Convert ERROR! The number system was entered incorrectly, run the program again!");
    }

    char* snum1 = toCharArray(); // Создаем массив символов числа

    for (LongNumber i = LongNumber("0"); i < len; i += LongNumber("1")) 
    {
        coefficient = convert_coefficient(snum1, i_int);
        LongNumber rezmul(coefficient);
        rezmul *= notation.power(atoi(len_tmp.toCharArray())); // Преобразование к int
        sum += rezmul;
        if (len_tmp != LongNumber("0")) len_tmp -= LongNumber("1");
        i_int++;
    }

    sum.enscure_size(sum.length);
    *summary = sum;
    delete[] snum1; // Освобождаем память после завершения работы с массивом
    return 0; // успешное завершение 
}

LongNumber LongNumber::convert_coefficient(const char* symbol, const int i) 
{
    LongNumber coefficient;
    if ((symbol[i] >= '0') && (symbol[i] <= '9')) {
        coefficient = LongNumber(to_string(symbol[i] - '0').c_str());
    }
    else if (symbol[i] >= 'A' && symbol[i] <= 'F') {
        coefficient = LongNumber(to_string(symbol[i] - 'A' + 10).c_str());
    }
    else {
        delete[] symbol; // Освобождаем память 
        throw std::invalid_argument("Convert ERROR! Invalid character in number for this calculation system!");
    }

    return coefficient;
}

