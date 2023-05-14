FRONT_PATH = ./frontend
MIDDLE_PATH = ./middlend
BACK_PATH = ./backend/Processor


all :   
	make -C $(FRONT_PATH) -f Makefile
	make -C $(MIDDLE_PATH) -f Makefile
	make -C $(BACK_PATH) -f Makefile ASM
	make -C $(BACK_PATH) -f Makefile HE_CPU

clear:
	make -C $(FRONT_PATH) -f Makefile clear
	make -C $(MIDDLE_PATH) -f Makefile clear
	make -C $(BACK_PATH) -f Makefile clear
