##
## sample make file (see demoguid.txt)
##

COMP = Y:\HIWARE\PROG\Chc05.exe -Or -Ot
LINK = Y:\HIWARE\PROG\linker.exe
ASM =  Y:\HIWARE\PROG\AHC05.exe

terminal.o: rs232.c delay.c dmf5001n.c terminal.c teclado.c
	  $(COMP) $? 
	  $(LINK) terminal.prm


