// Начнём с вопроса. Всё ли в порядке со следующим кодом?

#include <algorithm>
#include <list>
#include <vector>

int main() {
    std::vector<int> v = {3, 4, 5};
    std::list<int> l;
    std::copy(v.begin(), v.end(), l.begin()
}

// Проблема в том, что здесь std::copy пытаются применить к пустому списку.
// Так делать нельзя.
// Все алгоритмы, записывающие результат в выходную последовательность,
// рассчитывают на то, что в выходном контейнере уже выделено достаточно места для хранения элементов!

// Правильно вот так:

#include <algorithm>
#include <list>
#include <vector>

int main() {
    std::vector<int> v = {3, 4, 5};
    std::list<int> l;
    // ...
    l.resize(v.size());  // важно, чтобы все копируемые элементы поместились
    auto iter = std::copy(v.begin(), v.end(), l.begin());
    l.erase(iter, l.end());  // удаляем всё лишнее (в нашем конкретном случае ничего лишнего не будет)
}

// Заранее изменять размер контейнера, чтобы в него поместился ответ, очень неудобно.
// К счастью, существует специальная обёртка над итераторами - std::back_inserter.

#include <algorithm>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::vector<int> v = {3, 4, 5};
    std::list<int> l;
    std::copy(v.begin(), v.end(), std::back_inserter(l));
}

// std::back_inserter ведёт себя как обычный итератор,
// но при попытке записи нового значения он выполняет push_back в соответствующий контейнер.
// Соответственно, его можно использовать с любым контейнером, предоставляющим функцию push_back (вектором, списком, деком, строкой).
// Аналогично, существуют обёртки front_inserter и просто inserter.


// Перейдем теперь к совершенно другой теме.
// Мы будем учиться перегружать различные операторы в своих классах.
// Для этого изучим, какие операторы бывают.
// Рассмотрим битовые операции.
// Они применяются к беззнаковым целым числам:

#include <iostream>

int main() {
    unsigned char x = 13;  // битовое представление:   0000 1101
    auto x1 = x << 2;  // побитовый сдвиг влево на 2:  0011 0100
    auto x2 = x >> 1;  // побитовый сдвиг вправо на 1: 0000 0110

    // Обратите внимание, что со знаковыми типами возникают дополнительные проблемы.
    // Лучше всегда применять эти операции именно к беззнаковым типам.

    unsigned char a = 0x3E;  // 0011 1110
    unsigned char b = 0xA5;  // 1010 0101 
    // Побитовое "И":
    auto c = a & b;          // 0010 0100
    // Побитовое "ИЛИ":
    auto d = a | b;          // 1011 1111
    // Побитовое "Исключающее ИЛИ" (1, если биты разные; 0, если биты одинаковые):
    auto e = a ^ b;          // 1001 1011
    // Побитовое отрицание:
    auto f = ~a;             // 1100 0001

    // Пронумеруем биты справа налево (начиная с младшего) с нуля.
    // Как определить равен ли k-й бит единице?
    unsigned int k = 2;
    bool result = x & (1 << k);

    // Как записать единицу в k-й бит?
    x |= (1 << k);

    // Как записать 0 в k-й бит?
    x &= ~(1 << k);
}


// С помощью обычных чисел (unsigned long) мы можем кодировать комбинации из 32 или 64 битов.
// Что если этого недостаточно?
// В стандартной библиотеке есть контейнер std::bitset, представляющий набор битов произвольной (наперёд заданной длины).
// Внутри он хранит массив из нескольких беззнаковых целых чисел и использует их биты.

#include <bitset>
#include <iostream>

int main() {
    std::bitset<10> b(7);  // битовая маска длины 10, проинициализированная битами числа 7
    std::bitset<17> b2("010001101");  // инициализируем строкой из нулей и единиц

    std::cout << b.test(4) << "\n";  // проверка четвёртого бита
    b.set(4);  // установка четвёртого бита в единицу
    std::cout << b.test(4) << "\n";
    b.reset(3);  // сброс третьего бита в ноль
}

// Почему пришлось вводить два типа операторов: побитовые (&, |) и логические (&&, ||)?
// Во-первых, логические операторы применяются к true/false (если попробуете применить к числу, то число сначала преобразуется в bool).
// Во-вторых, логические операторы ленивы: например, если левая часть в && равна false, то правая не вычисляется.


// Давайте посмотрим на таблицу приоритета операторов: http://en.cppreference.com/w/cpp/language/operator_precedence
// Чем выше группа операторов в этой таблице, тем раньше он выполняется (если не стоят скобки).
// Например, традиционно умножение (*) имеет приоритет над сложением (+).
// У каждой группы операторов указана "ассоциативность": порядок расстановки скобок в сложных выражениях.
// Например, a - b - c проинтерпретируется как (a - b) - c, 
// а x += y *= z как x += (y *= z).

// Заметьте, что у многих операторов (и выражений) есть результат выполнения.
// Например, значение выражения x++ - это то, что будет присвоено y, если написать auto y = x++.

// Постфиксные операторы всегда имеют приоритет над префиксными.
// Постфиксный ++ и -- возвращают копию старого (неизмененного) значение переменной.
// Префиксные ++ и -- возвращают новое (уже измененное) значение (точнее, ссылку на саму измененную переменную).

// Самый низкий приоритет у оператора "запятая".
// Не следует путать этот оператор с разделителем переменных при объявлении или разделителем аргументов функции.
// "Запятая" сначала вычисляет левую часть, затем правую. Результатом является вычисленная правая часть.
// Пример:

for (int i = 0, j = 10; i <= j; ++i, --j) { //...
    //        ^ не оператор        ^ оператор "запятая"
}

// Типичная ошибка начинающих:
int x, y;
std::cin >> x, y;
//           ^ это оператор "запятая"!
//           x считается, а у - нет!
//           Правильно так: std::cin >> x >> y;

// Некоторые операторы (.*, ->* и т. д.) мы рассматривать не будем.


// Почти каждый оператор можно "перегрузить", то есть, придать ему в своём типе данных (своём классе) особый смысл.
// Проиллюстрируем это на примере класса "Комплексное число".
// (В стандартной библиотеке уже есть класс std::complex, но мы напишем свою версию для демонстрации).

// Что мы хотим от класса Complex?
// Мы захотим его использовать примерно так:

int main() {
    Complex z(3, 2);  // создаём комплексное число 3 + 2i
    Complex w = -z;
    Complex u = z + 2 * w;  // хотим перегрузить арифметические операторы, чтобы можно было писать такие выражения
    std::cout << z.Re() << "\n";  // печатаем действительную часть
    std::cout << z << "\n"; // печатаем само число в виде 3 + 2i
    std::cout << z.abs() << "\n";  // модуль числа
}

// Начнём:

#include <cmath>
#include <iostream>

class Complex {
 private:  // доступ к private-именам будет только у функций-членов этого класса
    double x, y;

 public:  // а к публичным именам есть доступ у всех желающих
    Complex(double a, double b = 0.0): x(a), y(b) {  // конструктор
    }

    double Re() const {  // слово const показывает, что эту функцию можно применять к константам
        return x;        // то есть, что функция не изменяет состояние текущего объекта
    }

    double Im() const {
        return y;
    }

    double abs() const {
        return std::sqrt(Re() * Re() + Im() * Im());
    }
};

// Если что-то можно написать снаружи (внешним по отношению к классу образом) - сделаем это:

Complex operator - (const Complex& z) {  // унарный минус
    return Complex(-z.Re(), -z.Im());
}

Complex operator + (const Complex& a, const Complex& b) {  // бинарный плюс
     return Complex(
         a.Re() + b.Re(),
         a.Im() + b.Im()
     );
}

Complex operator - (const Complex& a, const Complex& b) {  // бинарный минус
     return Complex(
         a.Re() - b.Re(),
         a.Im() - b.Im()
     );
}

Complex operator * (const Complex& a, const Complex& b) {  // умножение
    return Complex(
        a.Re() * b.Re() - a.Im() * b.Im(),
        a.Re() * b.Im() + a.Im() * b.Re()
    );
}

// Перегрузим оператор << для вывода в поток.
// Первый его аргумент имеет тип std::ostream& (таков, например, объект std::cout).
// Возвращается сам этот поток, чтобы можно было писать каскадные цепочки вида std::cout << z << "\n";
std::ostream& operator << (std::ostream& out, const Complex& z) {
    out << z.Re() << " + " << z.Im() << "i";  // тут надо бы поаккуратнее обрабатывать случай нулевой/единичной действительной или мнимой части
    return out;
}

// Недостающие функции и операторы допишите сами (см. задачу в домашнем задании).
