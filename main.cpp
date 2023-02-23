//практическая №1 вариант 7

#include <iostream>
#include <cmath>
using namespace std;

class Rectangle{ //класс прямоугольников со сторонами, параллельными осям OX и OY
    int point_x = 0;
    int point_y = 0;
    int side_vert = 0;
    int side_hor = 0;
public:
    void move();
    void size_chng();
    void min_rectangle(Rectangle & first, Rectangle & second);
    void intersection(Rectangle & first, Rectangle & second);
    void set_info() {
        cout << "Введите координаты нижней левой точки прямоугольника - x и y, а также его две стороны a - вертикальную и b - горизонтальную\n";
        cout << "X: ";
        cin >> point_x;
        cout << "Y: ";
        cin >> point_y;
        cout << "A: ";
        cin >> side_vert;
        cout << "B: ";
        cin >> side_hor;
        cout << endl;
    }
    void get_info() const {
        cout << "Rectangle is: " << '\n';
        cout << "X: " << point_x << endl;
        cout << "Y: " << point_y << endl;
        cout << "A: " << side_vert << endl;
        cout << "B: " << side_hor << endl;
    }
};

void Rectangle::move(){
    int x, y;
    cout << "Введите новые координаты нижней левой точки x и y: \n";
    cin >> x >> y;
    point_x = x;
    point_y = y;
}

void Rectangle::size_chng() {
    int a, b;
    cout << "Введите размеры новых сторон прямоугольника a и b: \n";
    cin >> a >> b;
    side_vert = a;
    side_hor = b;
}

void Rectangle::min_rectangle(Rectangle & first, Rectangle & second){
    int min_x, min_y, min_a, min_b, max_x, max_y, max_a, max_b;
    if (first.point_x <= second.point_x){
        min_x = first.point_x;
        min_a = first.side_vert;
        max_x = second.point_x;
        max_a = second.side_vert;
    } else {
        min_x = second.point_x;
        min_a = second.side_vert;
        max_x = first.point_x;
        max_a = first.side_vert;
    }
    if (first.point_y <= second.point_y){
        min_y = first.point_y;
        min_b = first.side_hor;
        max_y = second.point_y;
        max_b = second.side_hor;
    } else {
        min_y = second.point_y;
        min_b = second.side_hor;
        max_y = second.point_y;
        max_b = second.side_hor;
    }
    if (min_y + min_b >= max_y + max_b){
        side_vert = min_b;
    } else if (min_y + min_b >= max_y) {
        side_vert = min_b + abs(min_b - max_b);
    } else {
        side_vert = abs(max_y + max_b - min_y);
    }

    if (min_x + min_a >= max_x + max_a){
        side_hor = min_a;
    } else if (min_x + min_a >= max_x) {
        side_hor = min_a + abs(min_a - max_a);
    } else {
        side_hor = abs(max_x + max_a - min_x);
    }
    point_x = min_x;
    point_y = min_y;
}

void Rectangle::intersection(Rectangle & first, Rectangle & second){
    int min_x, min_y, min_a, min_b, max_x, max_y;
    if(first.point_y + first.side_vert <= second.point_y ||
            first.point_x + first.side_hor <= second.point_x || second.point_y + second.side_vert <= first.point_y ||
                second.point_x + second.side_hor <= first.point_x){ //нет пересечения
        point_x = 0;
        point_y = 0;
        side_vert = 0;
        side_hor = 0;
        return;
    }
    //прямоугольники вложены в друг друга
    if(first.point_y >= second.point_y && first.point_y + first.side_vert <= second.point_y + second.side_vert &&
       first.point_x + first.side_hor <= second.point_x + second.side_hor && first.point_x >= second.point_x){
        point_x = first.point_x;
        point_y = first.point_y;
        side_vert = first.side_vert;
        side_hor = first.side_hor;
        return;
    }
    if(second.point_y >= first.point_y && second.point_y + second.side_vert <= first.point_y + first.side_vert &&
       second.point_x + second.side_hor <= first.point_x + first.side_hor && second.point_x >= first.point_x){
        point_x = second.point_x;
        point_y = second.point_y;
        side_vert = second.side_vert;
        side_hor = second.side_hor;
        return;
    }
    if (first.point_x <= second.point_x){
        min_x = first.point_x;
        min_a = first.side_vert;
        max_x = second.point_x;
    } else {
        min_x = second.point_x;
        min_a = second.side_vert;
        max_x = first.point_x;
    }
    if (first.point_y <= second.point_y){
        min_y = first.point_y;
        min_b = first.side_hor;
        max_y = second.point_y;
    } else {
        min_y = second.point_y;
        min_b = second.side_hor;
        max_y = second.point_y;
    }

    point_x = max_x;
    point_y = max_y;
    side_vert = abs(min_a - abs(max_y - min_y));
    side_hor = abs(min_b - abs(max_x - min_x));
}

int main() {
    int mode;
    Rectangle p, q;
    Rectangle obj;
    cout << "Выберите режим работы: 1 - перемещение прямоугольника, 2 - изменение размеров прямоугольника, 3 - построение наименьшего прямоугольника, "
            "содержащего два других, 4 - построение прямоугольника на пересечении\n";
    cin >> mode;
    cout << '\n';
    switch(mode){
        case 1:
            obj.set_info();
            obj.move();
            obj.get_info();
            break;
        case 2:
            obj.set_info();
            obj.size_chng();
            obj.get_info();
            break;
        case 3:
            p.set_info();
            q.set_info();
            obj.min_rectangle(p, q);
            obj.get_info();
            break;
        case 4:
            p.set_info();
            q.set_info();
            obj.intersection(p, q);
            obj.get_info();
            break;
        default:
            return 1;
    }
    return 0;
}
