	;; This is a simple hello world in assembly for x86_64 bits computer
	;; To compile:

	;; This will generate the object file
	;; nasm -f elf64 hello_world.asm

	;; Then we need to use a linker, in linux we use the "ld", this linker will make our exectuable
	;; ld hello_world.o -o hello_world.out
	

	;; The data section of the program
	section .data

	;; Here we define a variable 'msg' that will contain the string
	;; notice that the '10' is a new line character and the, '0' is to represent the null char
	msg db "Hello, World!", 10, 0

	;; '$' return the actual address with '-' catch the difference, and with "msg", get the address of "msg"
	msg_len equ $ - msg
		
	;; Here we define the code section
	section .text

	;; Here we define the main routine, where the program will start
	global _start

	;; Here we define the code block
_start:
	;; To print the hello world, first selects the sys_write call
	mov rax, 1

	;; The file descriptor, 1, which is the stdout in linux
	mov rdi, 1

	;; Then the address of the text to print, and its length
	mov rsi, msg
	mov rdx, msg_len

	;; Run the syscall
	syscall

	
	;; To finish the program, we can't just do a simple return, it will give us segmentation fault
	;; ret

	;; We need to tell it to the kernel that we are finishing the program, so we use the sys_exit call
	mov rax, 60

	;; define the exit code 0, because it finishes successfully
	mov rdi, 0

	;; run the syscall
	syscall
	
	
