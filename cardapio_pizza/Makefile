CC = gcc
ifdef ComSpec
  SEP2=\\
  MD = mkdirp.bat
  RM = del /F /Q
  RD = rd /S /Q
  CREATE = @(echo ^@echo off & echo ^IF EXIST %%1 GOTO END & echo ^MKDIR %%1 & echo ^:END)> mkdirp.bat
else
  SEP2=/
  MD = mkdir -p
  RM = rm -rf
  RD = rm -rf
  CREATE = 
endif

SEP=$(strip $(SEP2))


ODIR = .$(SEP)build
IDIR = .

CFLAGS=-I$(IDIR)


_DEPS = arvore_b_mais.h pizza.h lista_pizzas.h metadados.h no_folha.h no_interno.h lista_nos_folhas.h lista_nos_internos.h ufftest.h
DEPS = $(patsubst %,$(IDIR)$(SEP)%,$(_DEPS))

_OBJ = arvore_b_mais.o pizza.o lista_pizzas.o metadados.o no_folha.o no_interno.o lista_nos_folhas.o lista_nos_internos.o test_arvore_b_mais.o
OBJ = $(patsubst %,$(ODIR)$(SEP)%,$(_OBJ))

$(ODIR)$(SEP)%.o: $(IDIR)$(SEP)%.c $(DEPS)
	${CREATE}
	${MD} $(ODIR)
	${CC} -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	${CC} -o $(ODIR)$(SEP)$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	${RM} *~ mkdirp.bat
	${RD} $(ODIR)$(SEP)
