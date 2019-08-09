FILE_SYSTEM_NAME = FSFS
INIT_FILE_NAME = ./startUp
SHELL_FILE_NAME = ./sh

LINK_OPTIONS = ./funSizeSys/readWriteSys.c ./sysStructs/iNode.c ./sysStructs/superBoulder.c ./sysStructs/mapping.c
run :
	touch $(FILE_SYSTEM_NAME)
	gcc -o $(INIT_FILE_NAME) $(INIT_FILE_NAME).c $(LINK_OPTIONS)
	$(INIT_FILE_NAME)
	$(RM) $(INIT_FILE_NAME)
	$(RM) $(INIT_FILE_NAME).o

	gcc -o PORTAL $(SHELL_FILE_NAME).c $(LINK_OPTIONS)
