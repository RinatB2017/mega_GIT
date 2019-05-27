#include "config_file_parser.h"
#include "../emulator/input.h"
#include "famicom.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>


CFGParser::CFGParser()
{
    cfg = new CONFIG_FILE;
    memset(cfg, 0, sizeof(CONFIG_FILE));

}

CFGParser::~CFGParser()
{
    delete cfg->ROM_PATH;
    delete cfg;
    cfg = 0;


}

int CFGParser::Load()
{
    memset(cfg, 0, sizeof(CONFIG_FILE));
    cfg->Control1_keybindings = new controller_bindings;
    cfg->Control2_keybindings = new controller_bindings;




    FILE *fp = fopen("famique.cfg", "rb");
    if(!fp)
        return FILE_OPEN_ERROR;
    fread(&cfg->header, sizeof(CFG_HEADER), 1, fp);
    cfg->ROM_PATH = new char [cfg->header.rom_path_len];
    memset(cfg->ROM_PATH, 0, cfg->header.rom_path_len);
    fread(cfg->Control1_keybindings, cfg->header.control1_len, 1, fp);
    fread(cfg->Control2_keybindings, cfg->header.control2_len, 1, fp);
    fread(&cfg->audioenabled, sizeof(byte), 1, fp);
    fread(cfg->ROM_PATH, cfg->header.rom_path_len, 1, fp);
    fclose(fp);
    return 0;

}

void CFGParser::Save()
{
    FILE *fp = fopen("famique.cfg", "wb");
    fwrite(&cfg->header, sizeof(CFG_HEADER), 1, fp);
    fwrite(cfg->Control1_keybindings, cfg->header.control1_len, 1, fp);
    fwrite(cfg->Control2_keybindings, cfg->header.control2_len, 1, fp);
    fwrite(&cfg->audioenabled, sizeof(byte), 1, fp);
    fwrite(cfg->ROM_PATH, cfg->header.rom_path_len, 1, fp);
    fclose(fp);


}

void CFGParser::UpdateConfigData(controller_bindings *c1, controller_bindings *c2, unsigned char audio_enabled, char *rom_path)
{
    memset(cfg, 0, sizeof(CONFIG_FILE));

    cfg->header.control1_len = cfg->header.control2_len =sizeof(controller_bindings);


        cfg->header.rom_path_len = (dword)strlen(rom_path);

    if(!cfg->ROM_PATH)
         cfg->ROM_PATH = (char *)malloc(strlen(rom_path));



    memcpy(cfg->ROM_PATH, rom_path, strlen(rom_path));
    cfg->Control1_keybindings = c1;
    cfg->Control2_keybindings = c2;



}
