SRC_DIRECTORY = src/com/daloji/dongle
BIN_DIRECTORY = com/daloji/dongle
JAR_FILE = usbDongle.jar

help:
	@echo ' global Makefile for this project                                                   '
	@echo '                                                                        '
	@echo 'Usage:                                                                  '
	@echo '   make all                 compile  java and native C code               '
	@echo '   make clean               remove generated .classes ans shared library     '




all:clean
	#javac -d  .  $(SRC_DIRECTORY)/*.java
	javah -d jni -jni -classpath  src com.daloji.dongle.Core
	make -C jni  all
	jar -cvf $(JAR_FILE)  $(BIN_DIRECTORY) lib
	
	
   
clean:
	#rm -f $(BIN_DIRECTORY)/*.class
	rm -f $(JAR_FILE)
	
