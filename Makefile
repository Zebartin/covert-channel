run : receiver.c sender.c
	gcc receiver.c -o receiver
	gcc sender.c -o sender

clean:
	rm receiver sender
	rm A B C
