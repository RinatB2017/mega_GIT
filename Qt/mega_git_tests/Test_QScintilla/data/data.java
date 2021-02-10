class Box {
    int width; // ширина коробки
    int height; // высота коробки
    int depth; // глубина коробки

    // Конструктор
    Box(int a, int b) {
        width = a;
        height = b;
        depth = 10;
    }

    // вычисляем объём коробки
    int getVolume() {
        return width * height * depth;
