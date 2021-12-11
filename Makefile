pid : receiver_pid.c sender_pid.c
	gcc receiver_pid.c -o receiver
	gcc sender_pid.c -o sender

msg : receiver_msg.c sender_msg.c
	gcc receiver_msg.c -o receiver
	gcc sender_msg.c -o sender

at: receiver_at.c sender_at.c
	gcc receiver_at.c -o receiver
	gcc sender_at.c -o sender

clean:
	@-rm receiver sender
	@-rm handshake sync
	@-rm out_text
