#
# Les variables d'environnement libG2X, incG2X
# sont definies dans le fichier ~/.bashrc par le script ../install.sh
#
#compilateur
CC = gcc

CCEXT=
ifeq ($(CC),g++) #en mode debug
  CCEXT=++
endif

#compil en mode 'debug' ou optmisée (-O2)
DBG = no

ifeq ($(DBG),yes) #en mode debug
  CFLAGS = -g -Wpointer-arith -Wall  -std=c99
else               #en mode normal
  CFLAGS = -O2  -std=c99
endif

# assemblage des infos de lib. et inc.
lib =   $(libG2X)$(CCEXT) $(libG3X)$(CCEXT)
# fichiers *.c locaux
src = src/
# fichiers *.h locaux et lib.
inc = -I./include $(incG2X) ./include $(incG3X) 


exec = main g2x_example oscillator balistic
all : $(exec) clean

PartSyst : PMat.o Link.o PartSyst.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"

PartSyst2 : PMat.o Link.o PartSyst2.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"

td : tdrope.o link.o pmat.o motor.o mathg2x.o
	$(CC) $^ $(lib) -o $@

flag : tdflag.o link3d.o pmat3d.o motor3d.o math3x.o
	$(CC) $^ $(lib) -o $@

ghost : ghost.o link3d.o pmat3d.o motor3d.o math3x.o
	$(CC) $^ $(lib) -o $@

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.c
	@echo "module $@"
	@$(CC) $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable xxx à partir de src/xxx.c (1 seul module)
% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"
	
.PHONY : clean cleanall ?

# informations sur les paramètres de compilation       
? :
	@echo "---------compilation informations----------"
	@echo "  processor      : $(PROCBIT)"
	@echo "  compiler       : $(CC)"
	@echo "  options        : $(CFLAGS)"
	@echo "  lib g2x/OpenGl : $(lib)"
	@echo "  headers        : $(incG2X)"
clean : 
	@rm -f *.o
cleanall :
	@rm -f *.o $(exec) */*~ *~
	
