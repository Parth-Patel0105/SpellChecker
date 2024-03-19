# Target executable for spelling checker program
spchk: spchk.c
	gcc -o spchk spchk.c

# Clean target to remove the compiled executable
clean:
	rm -f spchk
