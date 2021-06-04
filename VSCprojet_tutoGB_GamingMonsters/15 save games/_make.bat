REM c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c
REM c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o main.gb main.o

c:\gbdk\bin\lcc -Wa-l -Wf-ba0 -c -o savestate.o savestate.c
c:\gbdk\bin\lcc -Wa-l -c -o main.o main.c
c:\gbdk\bin\lcc -Wl-yt3 -Wl-yo4 -Wl-ya4 -o main.gb main.o savestate.o

TIMEOUT /T 0
main.gb