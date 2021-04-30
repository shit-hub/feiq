# @Author: sanjo heron
# @Date:   2018-07-13 Fri
# @Email:  1003261954@qq.com
# @Filename: Makefile
# @Last modified by:   sanjo heron
# @Last modified time: 2018-08-08 Wed


PRO_GCC  =  gcc

OUTPUT   =  ./output/

PRO_NAME =  feiq

MY_LIB   =  -pthread

MY_INC   =  ./inc/

MY_SRC   =  ./src/

MY_PRO_S =  ./server/

MY_PRO_C =  ./client/

MY_DST_P =  ../bin/

ADDINC   =  -I./inc/

SERVER_PRO_NAME =  Downloader

CLIENT_PRO_NAME =  Uploader

PUBLIC_OBJ  =  $(OUTPUT)MySocket.o

SERVER_OBJ  =  $(OUTPUT)server.o $(OUTPUT)server_fun.o

CLIENT_OBJ  =  $(OUTPUT)client.o $(OUTPUT)client_fun.o

$(PRO_NAME):$(MY_DST_P)$(SERVER_PRO_NAME) $(MY_DST_P)$(CLIENT_PRO_NAME)

$(MY_DST_P)$(SERVER_PRO_NAME) : $(SERVER_OBJ) $(PUBLIC_OBJ)
	$(PRO_GCC) $(SERVER_OBJ) $(PUBLIC_OBJ) $(ADDINC) $(MY_LIB) -o $(MY_DST_P)$(SERVER_PRO_NAME) -Wall

$(MY_DST_P)$(CLIENT_PRO_NAME) : $(CLIENT_OBJ) $(PUBLIC_OBJ)
	$(PRO_GCC) $(CLIENT_OBJ) $(PUBLIC_OBJ) $(ADDINC) -o $(MY_DST_P)$(CLIENT_PRO_NAME) -Wall

$(OUTPUT)server.o : $(MY_PRO_S)server.c $(MY_INC)MySocket.h
	$(PRO_GCC) $(MY_PRO_S)server.c -c -o $(OUTPUT)server.o $(ADDINC) $(MY_LIB)

$(OUTPUT)client.o : $(MY_PRO_C)client.c $(MY_INC)MySocket.h
	$(PRO_GCC) $(MY_PRO_C)client.c -c -o $(OUTPUT)client.o $(ADDINC)

$(OUTPUT)MySocket.o : $(MY_SRC)MySocket.c $(MY_INC)MySocket.h
	$(PRO_GCC) $(MY_SRC)MySocket.c -c -o $(OUTPUT)MySocket.o $(ADDINC)

$(OUTPUT)server_fun.o : $(MY_SRC)server_fun.c $(MY_INC)server_fun.h $(MY_INC)MySocket.h
	$(PRO_GCC) $(MY_SRC)server_fun.c -c -o $(OUTPUT)server_fun.o $(ADDINC)

$(OUTPUT)client_fun.o : $(MY_SRC)client_fun.c $(MY_INC)client_fun.h $(MY_INC)MySocket.h
	$(PRO_GCC) $(MY_SRC)client_fun.c -c -o $(OUTPUT)client_fun.o $(ADDINC)

clean :
	@rm $(MY_DST_P)$(SERVER_PRO_NAME) $(MY_DST_P)$(CLIENT_PRO_NAME) $(SERVER_OBJ) $(CLIENT_OBJ) $(PUBLIC_OBJ)

##   <-----------------------   end of file   ----------------------->   ##
