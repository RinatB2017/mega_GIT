#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

//////////////////Шаблоны функций

/*
максимум из двух значений одного типа
Требование к типу T: должна быть определена операция <
bool operator < (const T &x, const T &y) { ... }
*/
template <class T>
inline const T& Max(const T &x, const T &y) {
    return x < y ? y : x;
}

/*
минимум из двух значений одного типа
Требование к типу T: аналогично Max
*/
template <class T>
inline const T& Min(const T &x, const T &y) {
    return x < y ? x : y;
}

/*
копирование массивов равного размера из элементов одинакового типа
Аргументы:
указатель x на первый элемент результирующего массива,
указатель y на константный объект - адрес первого элемента исходного массива,
count - число элементов в массивах.
Требование к типу T: операция присваивания должна быть корректно определена
*/
template <class T>
void Copy(T *x, const T *y, unsigned count) {
    while (count--) *x++ = *y++;
}

/*
сравнение массивов равного размера из элементов одинакового типа
Требование к типу T: должна быть определена операция !=
bool operator != (const T &x, const T &y) { ... }
*/
template <class T>
bool Compare(const T *x, const T *y, unsigned count) {
    while (count--) if (*x++ != *y++) return false;
    return true;
}

/*
ввод массива из потока
Аргументы:
указатель x на первый элемент массива,
count - число элементов в массиве,
ссылка на поток ввода stream (по умолчанию стандартный поток ввода cin).
Требование к типу T: должна быть определена операция ввода из потока >>
istream& operator >> (istream &stream, T &x) { ... }
*/
template <class T>
void Read(T *x, unsigned count, istream &stream = cin) {
    while (count--) stream >> *x++;
}

/*
вывод массива в поток
Аргументы:
указатель x на константый объект - адрес первого элемента массива,
count - число элементов в массиве,
ссылка поток вывода stream (по умолчанию стандартный поток вывода cout),
указатель delim на константую строку, которая выводится после каждого элемента массива (по умолчанию " ").
Требование к типу T: должна быть определена операция вывода в поток <<
ostream& operator << (ostream &stream, const T &x) { ... }
*/
template <class T>
void Write(const T *x, unsigned count, ostream &stream = cout, const char *delim = " ") {
    while (count--) stream << *x++ << delim;
}

//////////////////Шаблон класса динамического массива

template <class T>
class Array {
private:
    unsigned FCount;	// число элементов
    T* FData;			   // указатель на первый элемент массива (если FCount > 0)
public:
    // Создание массива из count элементов, по умолчанию 0
    Array(unsigned count = 0) : FCount(0) { // инициализируем элемент FCount конструктором копирования
        Resize(count, false);
    }

    // Создание массива из count элементов, которые инициализируются
    // count элементами, на которые указывает data.
    Array(unsigned count, const T *data) : FCount(0) {
        Assign(count, data);
    }

    // Конструктор копирования
    Array(const Array& array) : FCount(0) {
        Assign(array.FCount, array.FData);
    }

    // Деструктор (освобождает память).
    ~Array() {
        Clear();
    }

    // Возвращает размер массива
    unsigned Count() const { return FCount; }

    // Задает размер массива
    void Count(unsigned count) { Resize(count); }

    // Устанавливает размер массива в count и копирует в него count элементов
    // data[0], data[1], ..., data[count - 1].
    void Assign(unsigned count, const T *data);

    // Устанавливает размер массива. Старые элементы (сколько влезут в новый размер)
    // по умолчанию остаются (параметр store = true), либо теряются (store = false).
    void Resize(unsigned count, bool store = true);

    // Удаление всех элементов.
    void Clear() {
        Count(0);
    }

    // Операция присваивания. Устанавливается такой же размер и копируются данные из array.
    Array& operator = (const Array& array)	{
        if (this != &array) // Если не самоприсваивание (Array<T> a; a = a;), то
            Assign(array.FCount, array.FData); //выполняем присваивание
        //Возвращаем ссылку на левый аргумент операции присваивания, чтобы позволить, например,
        //дальнейшее присваивание (Array<T> a, b, c; a = b = c;)
        return *this;
    }

    // Операция индексации (для константного массива)
    const T& operator [] (unsigned index) const	{
        assert(index < FCount); // проверка корректности индекса
        return FData[index]; // и возврат константной ссылки на элемент
    }

    // Операция индексации (для неконстантного массива)
    T& operator [] (unsigned index)	{
        assert(index < FCount);	// проверка корректности индекса
        return FData[index];	   // и возврат ссылки на элемент
    }

    // Операция вывода в поток
    friend ostream & operator << (ostream &stream, const Array& array) {
        Write(array.FData, array.FCount, stream); // Вывод в поток и
        return stream; // возврат ссылки на поток, чтобы позволить последующий вывод (нап: cout << a << b).
    }

    // Операция ввода из потока
    friend istream & operator >> (istream &stream, Array& array) {
        Read(array.FData, array.FCount, stream); // Ввод из потока и
        return stream; // возврат ссылки на поток, чтобы позволить последующий ввод (нап: cin >> a >> b).
    }

    // Операция равенства
    friend bool operator == (const Array& x, const Array& y) {
        if (&x != &y) { // Если массивы являются различными объектами, то выполняем сравнение
            if (x.FCount == y.FCount) // Если число элементов одинаково,
                return Compare(x.FData, y.FData, x.FCount); // то выполняем поэлементное сравнение
            else return false; // Иначе, массивы не равны
        }
        return true; 	// Иначе возвращаем истину (т. к. любой массив сам себе равен)
    }

    friend bool operator !=(const Array& x, const Array& y) { // Операция неравенства
        return !(x == y);
    }
};

//////////////////Определения не-встраиваемых функций класса-шаблона

template <class T>
void Array<T>::Assign(unsigned count, const T *data) {
    Resize(count, false); //устанавливаем размер, без сохранения элементов
    Copy (FData, data, FCount); //и копируем данные
}

template <class T>
void Array<T>::Resize(unsigned count, bool store) {
    if (FCount != count) { // Если число элементов изменяется.
        if (count) { // Если новое число элементов не нулевое, то распределяем память;
            T *p = new T[count]; // Создаем динамический массив из count элементов,
            if (store) // и копируем туда старые элементы (сколько влезет), если требуется
                Copy(p, FData, Min(FCount, count));
            if (FCount) delete[] FData; // Уничтожаем старый массив, если он не пуст.
            FData = p; 		// Сохраняем в классе адрес первого элемента нового массива.
        }
        else delete[] FData; // иначе освобождаем память.
        FCount = count; // Сохраняем новое число элементов в классе.
    }
}

//////////////////Вспомогательный класс для вывода массива в поток
template <class T>
class Wr {
    const char *Name; // имя массива
    const Array<T> &Arr; // ссылка на массив
public:
    Wr(const char *n, const Array<T> &a) : Name(n), Arr(a) { }
    friend ostream& operator <<(ostream &s, const Wr<T> &w) {
        // Выводим имя массива, знак равенства, элементы и конец строки.
        s << w.Name << " = " << w.Arr << endl; return s;
    }
};

//////////////////Демо-программа

typedef int Type; // Тип элементов массива для демки
typedef Array<Type> TypeArray; // Чтобы все время не указывать параметры шаблона, используют синонимы типа
typedef Wr<Type> W;

const unsigned Size = 5;
const Type Init[Size] = { 1, 2, 3, 4, 5 };

int main() {
    TypeArray a, // пустой массив
            b(3), // массив из 3 элементов, создаваемый конструктором по умолчанию
            // (для int конструктор по умолчанию определен компилятором, который
            // ничего не делает, так что значение элементов не определено)
            c(Size, Init), // массив из Size эл-в с инициализацией другим массивом
            d(c); // конструктор копирования

    W wa("a", a), wb("b", b), wc("c", c), wd("d", d); // вспомогательные классы вывода
    cout << "Initial arrays" << endl;
    cout << wa << wb << wc << wd << endl; //вывод массивов в поток

    c.Resize(c.Count() * 2, false); // Удвоим количество элементов в c без сохранения
    cout << "Resized C without items=" << endl;
    cout << wc;

    unsigned i;
    for (i = 0; i < d.Count(); i++) { // Заполним c, используя элементы d
        c[i * 2] = d[i];
        c[i * 2 + 1] = 2 * d[i];
    }
    a = c; // и скопируем c в a
    cout << "After copying" << endl;
    cout << wc << wa;

    d.Count(d.Count() * 2); // Удваиваем количество элементов в массиве d
    for (i = d.Count() / 2; i < d.Count(); i++) d[i] = c[i]; // и скопируем новые элементы из c
    cout << "Resized D with half items from C=" << endl;
    cout << wd;

    cout << "a == b: " << (a == b) << endl; //операции равенства
    cout << "a == c: " << (a == c) << endl;
    cout << "a == d: " << (a == d) << endl;
    cout << "a != b: " << (a != b) << endl;
    cout << "a != c: " << (a != c) << endl;
    cout << "a != d: " << (a != d) << endl;

    W x[] = { wa, wb, wc, wd }; //массив из вспомогательных классов вывода
    ofstream f("out.txt"); //выведем все массивы в файл "out.txt"
    Write(x, 4, f, "---\n"); //при помощи функции Write

    cout << endl << "Press q to exit: ";
    cin.ignore(100, 'q'); return 0;
}
