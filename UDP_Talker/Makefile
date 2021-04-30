# @Author: sanjo heron
# @Date:   2018-07-13 Fri
# @Email:  sanjo.heron@gmail.com
# @Filename: Makefile
# @Last modified by:   sanjo heron
# @Last modified time: 2018-08-10 Fri


PRO_GCC  =  gcc

OUTPUT   =  ./output/

PRO_NAME =  ../bin/feiq

MY_INC   =  ./inc/

MY_SRC   =  ./src/

MY_LIB   =  -lncurses -pthread

ADDINC   =  -I ./inc/

OBJ      =  $(OUTPUT)main.o \
            $(OUTPUT)MySocket.o $(OUTPUT)Mylist.o \
            $(OUTPUT)User.o $(OUTPUT)Display.o

$(PRO_NAME):$(OBJ)
	$(PRO_GCC) $(OBJ) $(ADDINC) $(MY_LIB) -o $(PRO_NAME) -Wall

$(OUTPUT)MySocket.o : $(MY_SRC)MySocket.c $(MY_INC)MySocket.h $(MY_INC)includes.h
	$(PRO_GCC) $(MY_SRC)MySocket.c -c -o $(OUTPUT)MySocket.o $(ADDINC) $(MY_LIB)

$(OUTPUT)User.o : $(MY_SRC)User.c $(MY_INC)User.h $(MY_INC)includes.h
	$(PRO_GCC) $(MY_SRC)User.c -c -o $(OUTPUT)User.o $(ADDINC) $(MY_LIB)

$(OUTPUT)Mylist.o : $(MY_SRC)Mylist.c $(MY_INC)Mylist.h $(MY_INC)includes.h
	$(PRO_GCC) $(MY_SRC)Mylist.c -c -o $(OUTPUT)Mylist.o $(ADDINC) $(MY_LIB)

$(OUTPUT)Display.o : $(MY_SRC)Display.c $(MY_INC)Display.h $(MY_INC)includes.h
	$(PRO_GCC) $(MY_SRC)Display.c -c -o $(OUTPUT)Display.o $(ADDINC) $(MY_LIB)

$(OUTPUT)main.o : main.c $(MY_INC)includes.h
	$(PRO_GCC) main.c -c -o $(OUTPUT)main.o $(ADDINC) $(MY_LIB)

clean :
	@rm $(PRO_NAME) $(OBJ)

##   <-----------------------   end of file   ----------------------->   ##
