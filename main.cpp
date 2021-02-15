#include <iostream>
#include <stdint.h> //библиотека для работы с разными диапазонами чисел прдставления
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cstring>

using namespace std;

void encrypt(uint32_t* v, uint32_t* k) {
//    uint32_t - блоки длиной 32 бита
// созданы переменные для хранения входного блока и промежуточных данных
    uint32_t v0 = v[0];
    uint32_t v1 = v[1];
    uint32_t sum = 0;
    uint32_t i;
// блок для хранения некой константы  который входит в алгоритм шифрования
    uint32_t delta = 0x9e3779b9;
// блок для хранения ключа
    uint32_t k0 = k[0];
    uint32_t k1 = k[1];
    uint32_t k2 = k[2];
    uint32_t k3 = k[3];
    //сам алгоритм шифрования
    for (i = 0; i < 32; i++) {
        sum += delta;
        v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
    }
    // возврат назад пришедшим нашим данным;
    v[0] = v0;
    v[1] = v1;

}

void decrypt(uint32_t* v, uint32_t* k) {
//    uint32_t - блоки длиной 32 бита
// созданы переменные для хранения входного блока и промежуточных данных
    uint32_t v0 = v[0];
    uint32_t v1 = v[1];
    uint32_t sum = 0xC6EF3720;
    uint32_t i;
// блок для хранения некой константы  который входит в алгоритм шифрования
    uint32_t delta = 0x9e3779b9;
// блок для хранения ключа
    uint32_t k0 = k[0];
    uint32_t k1 = k[1];
    uint32_t k2 = k[2];
    uint32_t k3 = k[3];
    //сам алгоритм дешифровки
    for (i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        sum -= delta;
    }
    // возврат назад пришедшим нашим данным;
    v[0] = v0;
    v[1] = v1;

}

// функция генерирования случайного ключа
void genKey(uint32_t *key) {
    int count; // счетчик
    srand(time(NULL)); // чтобы не было коллизии
    for (count = 0; count < 4; count++) {
        // rand()<<16 rand генерирует число и происходит сдвиг на 16 позиций влево  |rand() а после происходит еще присваение 16 битов
        key[count] = rand() << 16 | rand();
    }
}

int main(int argc, char* argv[]) {
    cout << "hi";
    uint32_t key[4]; // массив для хранения ключа
    uint32_t data[2]; // массив для хранения данных
    char name[20]; // массив для хранения имени файла
    char ext[5]; // массив для хранения расширения
    char *tmp; // переменная для хранения временных данных
    if (strcmp(argv[1], "-e") == 0) {
        cout << "hi";
        // сгенерировали ключ
        genKey(key);
        // открытие файли и храни в него ключ
        ofstream keyf("VVA_key.txt", ios::binary);
        keyf.write((char *) &key, sizeof key);
        keyf.close();
        // открытие файли и прочитай из него блок данных
        ifstream encrf(argv[2], ios::binary);
        encrf.read((char *) &data, sizeof data);
        encrf.close();

        encrypt(data, key); // шифровка
        // сохранения
        ofstream crptf(strcat(argv[2], ".enc"), ios::binary);
        crptf.write((char *) &data, sizeof data);
        crptf.close();
    }
    // strcmp сравнивает символы двух строк
    if (strcmp(argv[1], "-d") == 0) {
        // обработка имени файла
        tmp = strchr(argv[2],'.');
        strncpy(name,argv[2],tmp-argv[2]);
        cout<<ext<<endl;
        // открытие файли и храни в него ключ

        ifstream crptf(argv[2], ios::binary);
        crptf.read((char *) &data, sizeof data);
        crptf.close();
        // чтение ключа
        ifstream keyf("VVA_key.txt", ios::binary);
        keyf.read((char *) &key, sizeof key);
        keyf.close();

        decrypt(data,key); // деифровка

        ofstream dcrptf(strcat(name, ".dec.txt"), ios::binary);
        dcrptf.write((char *) &data, sizeof data);
        dcrptf.close();

    }
    return 0;
}

