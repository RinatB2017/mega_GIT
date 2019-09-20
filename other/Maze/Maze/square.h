#ifndef SQUARE_H
#define SQUARE_H

class Square
{
    bool left_edge;
    bool top_edge;
    bool right_edge;
    bool bottom_edge;

public:
    Square(bool left, bool top, bool right, bool bottom);
    void set_left_edge(bool tmp) { left_edge = tmp; }
    void set_top_edge(bool tmp) { top_edge = tmp; }
    void set_right_edge(bool tmp) { right_edge = tmp; }
    void set_bottom_edge(bool tmp) { bottom_edge = tmp; }

    bool get_left_edge() { return left_edge; }
    bool get_top_edge() { return top_edge; }
    bool get_right_edge() { return right_edge; }
    bool get_bottom_edge() { return bottom_edge; }
};

#endif // SQUARE_H
