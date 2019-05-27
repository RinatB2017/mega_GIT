#ifndef INPUT_H
#define INPUT_H

#include "famicom.h"

typedef struct _controller_bindings
{
    dword up;
    dword down;
    dword left;
    dword right;
    dword a;
    dword b;
    dword start;
    dword select;
}controller_bindings;

class NESInput
{
public:
    NESInput();
    ~NESInput();
    void SetDefault();
    void SetControllerBinding1(controller_bindings *controller, bool first=true);

    byte UpdateController1(dword keypress, byte previous_input,  bool down);
    controller_bindings *GetController(byte cont) { if(cont==0) return c1; return c2;}
private:

    controller_bindings *c1;
    controller_bindings *c2;

};

#endif // INPUT_H
