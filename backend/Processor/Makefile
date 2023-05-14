OUTPUT_FILE_NAME_ASM = assembler
OUTPUT_FILE_NAME_CPU = CPU

CC = g++
CFLAGS = -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -D_DEBUG -D_EJUDGE_CLIENT_SIDE


SRC_DIR_ASM = ./src/assembler/
OBJ_DIR_ASM = ./obj/assembler/

SRC_ASM = assembler.cpp
OBJ_ASM = $(patsubst %.cpp, $(OBJ_DIR_ASM)%.o, $(SRC_ASM))

SRC_DIR_CPU = ./src/CPU/
OBJ_DIR_CPU = ./obj/CPU/

SRC_CPU = CPU.cpp stack.cpp
OBJ_CPU = $(patsubst %.cpp, $(OBJ_DIR_CPU)%.o, $(SRC_CPU))

all: ASM CPU

ASM: $(OBJ_ASM)
				$(CC) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME_ASM)
				./assembler
				

$(OBJ_DIR_ASM)%.o: $(SRC_DIR_ASM)%.cpp
				mkdir -p o$(OBJ_DIR_ASM)
				$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@


HE_CPU: $(OBJ_CPU)
				$(CC) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME_CPU)
				./CPU


$(OBJ_DIR_CPU)%.o: $(SRC_DIR_CPU)%.cpp
				mkdir -p o$(OBJ_DIR_CPU)
				$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@



clear:
				rm -f $(OBJ_DIR_ASM)*.o
				rm -f $(OBJ_DIR_CPU)*.o
