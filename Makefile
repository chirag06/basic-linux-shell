blazersh: basic_shell.c
	gcc basic_shell.c jobLinkList.c -o blazersh

clean:
	rm *.o blazersh *.log
