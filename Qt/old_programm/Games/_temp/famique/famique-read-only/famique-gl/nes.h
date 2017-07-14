#ifndef NES_H
#define NES_H

#include <QThread>

#include "nescart.h"
#include "famicom.h"
#include "cpu/cpu2.h"
#include "input.h"
#include "memory.h"
#include "config_file_parser.h"
#include "gl_draw_widget.h"



class NES : public QThread
{
public:
    NES(GLWidget *);
    ~NES();

    int OpenROM( char* filename);
    void CloseROM();

    NESInput *GetInputHandle() { return m_NESInput;};
    CFGParser *GetConfig() { return m_ConfigFile;}
    bool isInitialized() { return m_Init;}
    void Start();
    void Stop();
    bool isRunning();

    void UpdateKeyboard(MemoryManager *);
    void UpdateKeyboard(MemoryManager *, int key_pressed, bool pressed);
    M6502 *GetCPUHandle() { return m_CPU;}

    bool m_Draw;

    char *m_ROMpath;




    void Update();

    void setControllerBits(byte x){m_Controller = x;}
    byte getControllerBits() { return m_Controller;}


private:
    iNesLoader *m_NESLoader;
    NESCart *m_NESCart;
    M6502 *m_CPU;
    NESInput *m_NESInput;
    CFGParser *m_ConfigFile;

    bool m_Init;
    bool m_isRunning;
    byte m_Controller;

    void Blit();

    GLWidget *m_DrawWidget;
protected:

    virtual void run();

};

#endif // NES_H
