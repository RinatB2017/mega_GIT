#ifndef CONFIG_FILE_PARSER_H
#define CONFIG_FILE_PARSER_H
#include "famicom.h"
#include "../emulator/input.h"

typedef struct cfg_header__
{
    word control1_len;
    word control2_len;
    dword rom_path_len;
}CFG_HEADER;

typedef struct cfg__
{

    CFG_HEADER header;
    controller_bindings *Control1_keybindings;
    controller_bindings *Control2_keybindings;
    byte audioenabled;
    char *ROM_PATH;

}CONFIG_FILE;

class CFGParser
{
public:
    CFGParser();
    ~CFGParser();
    int Load();
    void Save();
    void UpdateConfigData(controller_bindings *c1, controller_bindings *c2, byte audio_enabled, char *rom_path);
    CONFIG_FILE *GetCFG() { return cfg;}
private:
    CONFIG_FILE *cfg;
};

#endif // CONFIG_FILE_PARSER_H
