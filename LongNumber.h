#ifndef BIGINT_H 
#define BIGINT_H 

#include <iostream> 

using namespace std;

class LongNumber {

private:
    char* storage;      //Хранилище элементов числа 
    int length;         //Длина без '\0' 
    int capacity;       //Длина с '\0' 
    int size;           // Размер массива 

    //Перевыделение памяти 
    void enscure_size(int new_length);

    //Вспомогательная функция для convert (вычисление длины строки)
    int my_strlen(const char* str);
    LongNumber convert_coefficient(const char* symbol, int i);

public:
    LongNumber(); 
    explicit LongNumber(int len);
    LongNumber(const char* char_array_number);
    LongNumber(const LongNumber& other); 
    ~LongNumber();
    int GetLength() const;
    LongNumber& operator = (const LongNumber& other);
    void operator += (const LongNumber& other);
    void operator -= (const LongNumber& other);
    void operator *= (const LongNumber& other);
    [[nodiscard]] LongNumber power(int exp) const;
    bool operator<(const LongNumber& other) const;
    bool operator>(const LongNumber& other) const;
    bool operator<=(const LongNumber& other) const;
    bool operator>=(const LongNumber& other) const;
    bool operator==(const LongNumber& other) const;
    bool operator!=(const LongNumber& other) const;
    void print() const;
    void println() const;
    //Перевод LongNumber в строку 
    [[nodiscard]] char* toCharArray() const;
    //Перевод от 2-ой по 16-ую систему счисления в десятичную
    int convert(const LongNumber& notation, LongNumber* summary);
    LongNumber sqrt() const;
    void generate_random_char_array(int numDigits);
};

#endif //BIGINT_H 
