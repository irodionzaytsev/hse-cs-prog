// Пусть задан массив чисел

#include <iostream>

int main() {
    const size_t N = 10;
    int a[N] = {1, 4, 5, 2, 6, -2, 4, 0, 2, 5};
}

// Давайте рассмотрим два способа печати его элементов:

// Первый - хорошо нам знакомый:
for (size_t i = 0; i != N; ++i)
    std::cout << a[i] << " ";
std::cout << "\n";

// А вот - второй:
for (const int * p = a; p != a + N; ++p)
    std::cout << *p << " ";
std::cout << "\n";

// Здесь мы вместо индекса i сдвигаем в цикле указатель p на очередной элемент массива.
// Заметим, что сам массив a автоматически преобразуется компилятором к указателю на его начальный элемент

// Обратите также внимание, что a + N - указатель, который указывает ЗА последний элемент массива.
// Нельзя брать значение, лежащее по адресу a + N - его просто нет, и это считается неопределенным поведением.
// Этот указатель используется только для сравнения с другими указателями.

// Конструкцию *p, возвращающую элемент, лежащий в ячейке, на которую указывает p, мы ещё называем "разыменованием указателя".


// Вопрос из зала: а с вектором так можно сделать?
// Ответ: да. Вот способы напечатать элементы вектора:

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 4, 5, 2, 6, -2, 4, 0, 2, 5};

    // цикл for по контейнеру
    for (int elem : v)
        std::cout << v << " ";
    std::cout << "\n";

    // аналог первого способа для массива
    for (size_t i = 0; i != v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << "\n";

    // аналог второго способа для массива тоже возможен, так как вектор хранит свои элементы непрерывным куском:
    for (const int * p = &v[0]; p != &v[0] + v.size(); ++p) // & - адрес переменной
        std::cout << *p << " ";
    std::cout << "\n";

    // а вот - способ вывести элементы через итераторы вектора:
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
}

// Итераторы - аналоги "указателей" для сложных контейнеров.
// Для вектора, впрочем, они могут совпадать просто с указателями.
// Для более сложных контейнеров, не хранящих элементы отдельным куском, это объекты некоторых классов, "прикидывающихся" указателями.


// Рассмотрим пример такого "сложного" контейнера - std::map
// Он в чем-то похож на dict из python'а, но реализован внутри иначе (как красно-чёрное дерево, а не хеш-таблица)

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> m; // ключи имеют тип std::string, значения - int
    m["hello"] = 42;
    m["good bye"] = 17;

    std::cout << m["hi"] << "\n"; // что напечатается на экране?
    // Ответ: напечатается 0.
    // Если такого ключа нет в map'е, то он положится в словарь со значением по умолчанию (0 для типа int)
    // и вернется ссылка на это вновь созданное значение.
    
    // Выведем все пары ключ-значение, хранящиеся в map'е:
    // Первый способ: цикл по контейнеру
    for (const auto& pair : m) // перебираем пары из ключа и значения
        std::cout << pair.first << " "    // первый элемент пары - ключ
                  << pair.second << "\n"; // второй элемент пары - значение
    // Вопрос из зала: разве здесь не нужны фигурные скобки вокруг тела for?
    // Ответ: нет, не нужны: в цикле выполняется ровно один оператор (хотя я и записал его в две строчки)
    // Но всё же скобки лучше поставить для удобочитаемости.

    // Второй способ: используем итераторы:
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " "     // можно было бы написать (*it).first,
                  << it->second << "\n";  // но оператор "стрелочка" позволяет сократить эту запись до it->first
    }

    // Как проверить, есть ли ключ в map'е?
    auto it = m.find("abracadabra"); // функция find возвращает итератор
    if (it == m.end())               // который равен m.end(), если такого ключа нет,
        std::cout << "Not found\n";
    else                             // или указывает на найденную пару, если такой ключ есть.
        std::cout << it->first << " " << it->second << "\n";
}



// Решим классическую задачу - подсчитать, сколько раз встретилось каждое слово в тексте.

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> freqs; // каждому слову сопоставляем его частоту
    std::string word;
    while (std::cin >> word)
        ++freqs[word]; // пользуемся тем, что если ключа в контейнере нет, то он добавится со значением 0

    for (const auto& pair : freqs)
        std::cout << pair.first << " " << pair.second << "\n";
}

// Обратите внимание, что слова выводятся в алфавитном порядке
// (в отличие от python'овского dict'а, где порядок был не определён).


// Что если требуется отсортировать слова по убыванию частоты?
// Скопируем их для этого в вектор и отсортируем его, используя свою функцию сравнения.

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Используем псевдоним для типа, чтобы не писать это длинное название
using TPair = std::pair<std::string, int>;

bool less(const TPair& a, const TPair& b) { // функция должна вести себя как оператор "меньше"
    return a.second > b.second; // сравниваем по частотам в обратном порядке
    // В принципе, при равенстве частот следовало бы ещё сравнить сами слова лексикографически.
}

int main() {
    std::map<std::string, int> freqs;
    std::string word;
    while (std::cin >> word)
        ++freqs[word];

    // Инициализируем вектор парой итераторов, то есть, копируем все пары "ключ-значение" из freqs.
    std::vector<TPair> pairs(freqs.begin(), freqs.end());

    // Стандартная функция сортировки принимает на вход два итератора и функцию сравнения.
    std::sort(pairs.begin(), pairs.end(), less);

    for (const auto& pair : pairs)
        std::cout << pair.first << " " << pair.second << "\n";
}

// Заметим, что функция less нам потребовалась только в одном месте - для сортировки.
// Можно было бы определить её прямо в месте употребления, даже не давая ей имени.
// Это делается с помощью конструкции лямбда-функции:

#include <iostream>
#include <map>
#include <string>
#include <vector>

using TPair = std::pair<std::string, int>;

int main() {
    std::map<std::string, int> freqs;
    std::string word;
    while (std::cin >> word)
        ++freqs[word];

    std::vector<TPair> pairs(freqs.begin(), freqs.end());

    std::sort(
        pairs.begin(),
        pairs.end(),
        [](const TPair& a, const TPair& b) { // лямбда-функция - определяем её прямо в месте использования
            return a.second > b.second;
        }
    );

    for (const auto& pair : pairs)
        std::cout << pair.first << " " << pair.second << "\n";
}


// Лямбда-функции появились начиная со стандарта C++11.
// В квадратных скобках могут стоять имена переменных из контекста, которые требуется пробросить в лямбда-функцию,
// но это выходит за рамки нашей лекции, и сейчас об этом поговорить не успеем.

// Есть шутка про то, что теперь такая программа является корректной:
// Разберитесь-ка, что здесь происходит!
int main() {[](){}();}

