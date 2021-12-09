run : receiver.c sender.c test.c
	gcc receiver.c -o receiver
	gcc sender.c -o sender
	gcc test.c  -o test

clean:
	rm receiver sender test
	rm A B C
