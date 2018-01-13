PROGRAM=dfs.exe
OBJ=main.o stack.o void_stack.o linked_list.o

# -ansi: tells the compiler to implement the ANSI language option. This turns off certain "features" of GCC which are incompatible with the ANSI standard
# -Wall: enables all the warnings about constructions that some users consider questionable
# -Wextra: enables some extra warning flags that are not enabled by -Wall
# -Werror: every warning is treated as an error
# -pedantic: used in conjunction with -ansi, this tells the compiler to be adhere strictly to the ANSI standard, rejecting any code which is not compliant
# 
OPT=-ansi -Wall -Wextra -Werror -pedantic

CC=gcc

#$(PROGRAM): $(OBJ)
#	${CC} $(OBJ) -o $(PROGRAM)

$(PROGRAM): $(OBJ)
	${CC} ${OPT} $(OBJ) -o $(PROGRAM)

# pattern rule for translating .c files, i.e. C modules, to .o object files
# http://www.gnu.org/software/make/manual/make.html#Pattern-Rules
%.o : %.c
	${CC} -c $<
 
clean:
	del $(OBJ) $(PROGRAM) 
