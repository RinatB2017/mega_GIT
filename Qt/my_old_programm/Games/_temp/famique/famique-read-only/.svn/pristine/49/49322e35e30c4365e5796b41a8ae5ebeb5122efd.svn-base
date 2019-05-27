#include "input.h"
#include <memory.h>
#include <string.h>
NESInput::NESInput()
{
    c1 = new controller_bindings;
    c2 = new controller_bindings;
    memset(c1, 0, sizeof(controller_bindings));
    memset(c2, 0, sizeof(controller_bindings));
}

void NESInput::SetDefault()
{
    c1->a = 0x41;
    c1->b = 0x42;
    c1->select = 0x20;
    c1->start = 0x1000004;
    c1->up = 0x1000013;
    c1->down = 0x1000015;
    c1->left = 0x1000012;
    c1->right = 0x1000014;
}

NESInput::~NESInput()
{
    delete c1;
    delete c2;
}

void NESInput::SetControllerBinding1(controller_bindings *controller, bool first)
{
    if(first)
    {
        if(c1!=controller)
        {
            delete c1;
            c1 = controller;
        }

    }
    else
    {
        if(c2!=controller)
        {
            delete c2;
            c2 = controller;
        }
    }
}

byte NESInput::UpdateController1(dword keypress, unsigned char previous_input, bool down)
{
    byte ret = previous_input;
    if( c1->a == keypress)
    {
        if(down)
            SET_BIT(ret, 0);
        else
            CLR_BIT(ret,0);
    }


   else if(c1->b == keypress)
    {
        if(down)
            SET_BIT(ret, 1);
        else
            CLR_BIT(ret,1);

    }
    else if( c1->select== keypress)
   {
        if(down)
            SET_BIT(ret,2);
        else
            CLR_BIT(ret,2);
    }

    else if(c1->start == keypress)
    {
        if(down)
            SET_BIT(ret,3);
        else
            CLR_BIT(ret,3);
    }
    else if(c1->up ==keypress)
    {
        if(down)
            SET_BIT(ret,4);
        else
            CLR_BIT(ret,4);
    }
   else if(c1->down == keypress)
    {
        if(down)
            SET_BIT(ret,5);
        else
            CLR_BIT(ret,5);
    }
    else if(c1->left == keypress)
   {
        if(down)
            SET_BIT(ret,6);
        else
            CLR_BIT(ret,6);
    }
    else if (c1->right == keypress)
    {
        if(down)
            SET_BIT(ret, 7);
        else
            CLR_BIT(ret,7);
    }


    return ret;


}
