#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QList>
//--------------------------------------------------------------------------------
#include "bos.hpp"
#include "brg.hpp"
#include "brg_channel.hpp"
//---------------------------------------------------------------------------------
#include "pet.h"
//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "create BOS: begin";
    Bos *bos = new Bos();
    qDebug() << "create BOS: end";

    Brg *brg = bos->get_brg(5);
    pet_brg_ctrl_t ctrl = brg->get_control();
    ctrl.tp_ctrl = 666;
    brg->set_control(ctrl);

    brg = bos->get_brg(6);
    pet_brg_state_t state = brg->get_state();
    state.state = 777;
    brg->set_state(state);

    //---
    pet_bos_state_t bos_state = bos->get_state();
    qDebug() << "bos state" << bos_state.state;
    //---
    for(int n=1; n<=48; n++)
    {
        Brg *brg = bos->get_brg(n);
        pet_brg_ctrl_t brg_ctrl = brg->get_control();
        pet_brg_state_t brg_state = brg->get_state();
        qDebug() << "brg" << n << "control" << brg_ctrl.tp_ctrl << "state" << brg_state.state;
    }
    //---

    qDebug() << "end";
    return a.exec();
}
//---------------------------------------------------------------------------------
