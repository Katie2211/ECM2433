gcc -ansi -c ${1}.c -o ${1}.o
gcc ${1}.o -lgsl -lgslcblas -o ${1}