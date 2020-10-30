/*Волков Матвей Андреевич
Разработать шаблоны классов согласно варианту задания.
Параметром шаблона должен являться скалярный тип данных задающий тип данных для оси координат.
Классы должны иметь только публичные поля. В классах не должно быть методов, только поля. Ф
игуры являются фигурами вращения (равнобедренными), за исключением трапеции и прямоугольника.
Для хранения координат фигур необходимо использовать шаблон  std::pair.
Например:
template <class T>
struct Square{
    using vertex_t = std::pair<T,T>;
    vertex_t a,b,c,d;
};


Необходимо реализовать две шаблонных функции:
1.	Функция print печати фигур на экран std::cout  (печататься должны координаты вершин фигур).
    Функция должна принимать на вход std::tuple с фигурами, согласно варианту задания (минимум по одной каждого класса).
2.	Функция square вычисления суммарной площади фигур.
    Функция должна принимать на вход std::tuple с фигурами, согласно варианту задания (минимум по одной каждого класса).

Создать программу, которая позволяет:
•	Создает набор фигур согласно варианту задания (как минимум по одной фигуре каждого типа с координатами типа int и координатоми типа double).
•	Сохраняет фигуры в std::tuple
•	Печатает на экран содержимое std::tuple с помощью шаблонной функции print.
•	Вычисляет суммарную площадь фигур в std::tuple и выводит значение на экран.

При реализации шаблонных функций допускается использование вспомогательных шаблонов std::enable_if, std::tuple_size, std::is_same.

 Вариант: Прямоугольник, трапеция, ромб
*/

#include <iostream>
#include <type_traits>
#include <tuple>
#include <cmath>

template<class... Ts>
struct is_tuple : std::false_type {
};

template<class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {
};

template<class T>
class Rectangle {
public:
    using type = T;
    //левая нижняя вершина и 2 стороны
    std::pair<T, T> point;
    T a, b;

    Rectangle(T x, T y, T A, T B) : point(x, y), a(A), b(B) {
        if (a <= 0 || b <= 0) {
            throw std::invalid_argument("Invalid Rectangle parameters");
        }
    }
};


template<class T>
class Rhombus {
public:
    using type = T;
    //левая вершина и 2 диагонали, причем ромб расположен так, что диагонали параллельны осям XOY
    std::pair<T, T> point;
    T d1, d2;

    Rhombus(T x, T y, T D1, T D2) : point(x, y), d1(D1), d2(D2) {
        if (d1 <= 0 || d2 <= 0) {
            throw std::invalid_argument("Invalid Rhombus parameters");
        }
    }
};


template<class T>
class Trapeze {
public:
    using type = T;
    //левая нижняя вершина и основания
    std::pair<T, T> point;
    T a, b, c;

    Trapeze(T x, T y, T A, T B, T C) : point(x, y), a(A), b(B), c(C) {
        if (a <= 0 || b <= 0) {
            throw std::invalid_argument("Invalid Trapeze parameters");
        }
    }
};


template<class T, size_t index = 0>
typename std::enable_if<is_tuple<T>::value, void>::type print(T &tup) {
    if constexpr (index < std::tuple_size<T>::value) {
        print_fig(std::get<index>(tup));
        print<T, index + 1>(tup);
    }
    else {
        return;
    }
}

template<class T>
typename std::enable_if<std::is_same<T, Rectangle<typename T::type>>::value, void>::type print_fig(T &req) {
    std::cout << "Point coord: (" << req.point.first << ", " << req.point.second << ')' << std::endl
              << "Edges: a=" << req.a << " b=" << req.b << '\n' << std::endl;
}

template<class T>
typename std::enable_if<std::is_same<T, Rhombus<typename T::type>>::value, void>::type print_fig(T &rho) {
    std::cout << "Point coord: (" << rho.point.first << ", " << rho.point.second << ')' << std::endl
              << "Edges: d1=" << rho.d1 << " d2=" << rho.d2 << '\n' << std::endl;
}

template<class T>
typename std::enable_if<std::is_same<T, Trapeze<typename T::type>>::value, void>::type print_fig(T &tra) {
    std::cout << "Point coord: (" << tra.point.first << ", " << tra.point.second << ')' << std::endl
              << "Edges: a=" << tra.a << " b=" << tra.b << " c=" << tra.c << '\n' << std::endl;
}


template <class T, size_t index = 0>
typename std::enable_if<is_tuple<T>::value, double>::type area(T &tup) {
    if constexpr (index < std::tuple_size<T>::value) {
        double value = area_fig(std::get<index>(tup));
        value += area<T, index+1>(tup);
        return value;
    } else {
        return 0;
    }
}

template <class T>
typename std::enable_if<std::is_same<T, Rectangle<typename T::type>>::value, double>::type area_fig(const T &req){
    return req.a * req.b;
}

template <class T>
typename std::enable_if<std::is_same<T, Rhombus<typename T::type>>::value, double>::type area_fig(const T &rum){
    return rum.d1 * rum.d2 / 2;
}

template <class T>
typename std::enable_if<std::is_same<T, Trapeze<typename T::type>>::value, double>::type area_fig(const T &tra){
    double diff = (std::fabs(tra.a - tra.b)) / 2.0;
    double height = std::sqrt(tra.c * tra.c - diff * diff);
    return height * (tra.a + tra.b) / 2.0;
}


int main() {
    int ix = 0, iy = 0, ia = 0, ib = 0, ic = 0;
    double dx = 0, dy = 0, da = 0, db = 0, dc = 0;

    try{
        std::cout << "Rectangle<int>. There are 4 parameters:\n1 coord point\n2 edges" << std::endl;
        std::cin >> ix >> iy >> ia >> ib;
        Rectangle<int> req1(ix, iy, ia, ib);
        ix = iy = ia = ib = 0;

        std::cout << "Rhombus<int>. There are 4 parameters:\ncoord point\n2 edges" << std::endl;
        std::cin >> ix >> iy >> ia >> ib;
        Rhombus<int> rum1(ix, iy, ia, ib);
        ix = iy = ia = ib = 0;

        std::cout << "Trapeze<int>. There are 5 parameters:\ncoord points" << std::endl;
        std::cin >> ix >> iy >> ia >> ib >> ic;
        Trapeze<int> tra1(ix, iy, ia, ib, ic);

        std::cout << "Rectangle<double>. There are 4 parameters:\ncoord point\n2 edges" << std::endl;
        std::cin >> dx >> dy >> da >> db;
        Rectangle<double> req2(dx, dy, da, db);
        dx = dy = da = db = 0;

        std::cout << "Rhombus<double>. There are 4 parameters:\ncoord point\n2 edges" << std::endl;
        std::cin >> dx >> dy >> da >> db;
        Rhombus<double> rum2(dx, dy, da, db);
        dx = dy = da = db = 0;

        std::cout << "Trapeze<double>. There are 5 parameters:\ncoord points\n3 edges" << std::endl;
        std::cin >> dx >> dy >> da >> db >> dc;
        Trapeze<double> tra2(dx, dy, da, db, dc);

        std::tuple< Rectangle<int>, Rhombus<int>, Trapeze<int>,
                    Rectangle<double>, Rhombus<double>, Trapeze<double> >
                    tup{req1, rum1, tra1, req2, rum2, tra2};

        std::cout << "Coord:" << std::endl;
        print(tup);
        double ts = area(tup);
        std::cout << "Total area: " << ts << std::endl;
    }
    catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
