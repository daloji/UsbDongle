CC=         gcc
SHARED_LIBS = libUsbDongle.so
SRC_FILE = usbdongle.c\
		command.c \
		hexa.c \
		sms_pdu.c
		   
help:
	@echo 'Makefile only for JNI                                                 '
	@echo '                                                                        '
	@echo 'Usage:                                                                  '
	@echo '   make jni-compile                 compile native C code               '
	@echo '   make clean                       remove generated shared library     '
	@echo '                                                                        '
	@echo "Note: To work from Java IDE you'll probably need only jni-compile target"

jni-compile:
	$(CC) $(SRC_FILE) -I $(JAVA_HOME)/include/ -o $(SHARED_LIBS) -shared 
	mv $(SHARED_LIBS) ../lib
     
all : clean  jni-compile

clean : 
	rm -f *.so
	rm -f ../lib/*
