Famique - an open source NES emulator

1. What is this?
2. Why?
3. Who?
4. What?
5. Usage
6. Known bugs
7. Compability
8. License
9. Source code


#kolla extra cycler:
#ifdef M6502_EXTRA_CYCLES
bool page_cross += ((long) (adress << 2) + X > 0xFF) ? 1:0; //check if we cross a page.
extra_cycles = page_cross;
#endif


vissa instruktioner dependar inte på att det ska waste:as cycler. tänk lite på hur fixa detta :-c
kanske call:a en funktion från mnemonic-callbacket som kollar ifall instruktionen INTE ska kolla för extra cycler, returna noll. om inte kolla hur många extra cycle som skall adderas (t.ex. bool check_page_cross(address, operand)


skriv en renderer som fungerar oavsett om fönster eller fullscreenritning aer paa.
dependa mindre på qt-integreringen, och inaktivera denna helt om fönsterendering aer inaktiverad.
kanske försöka dra events från SDL?


----URGENT!!!----


det aer ett fel naer man laeser fraan PPU_2007,

kanske maaste man implementera mirroring:en där med? (som i skrivrutinen?)


----

skriv ett pre-cachesystem för PPU:n.
T.ex. CHR-ROM ändrar sig aldrig, bara pre-cacha de som (konverterade) tiles och blitta med GPU.
kommer göra PPU:n 100-200% snabbare.
