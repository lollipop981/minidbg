"# rm_dbg" 

[+] write main loop to get command and run a function
[+] write helper functions to print long number of bytes, number in hex
[+] write dummy program to debug

[+] write function to get regs - matar
- write function to pause pid - roey
- write function to continue pid



rmdbg 877
>>> regs
    eax: 0x83848347
    ebx: 0x834023478
ptrace(GET_REGS, ....)
>>> p
ptrace(attach)
>>> c
ptrace(continue)

>>> mem 0x832749732498

process_vm_readv(....)
>>> stack 40
    sdfkljdfjsl  lkjlkjsdfjk sdflkjsdflk sdlfksldfjk
get_regs
process_vm_readv(regs->esp, ...)


if we are super good:
>>> br *0x9394394
>>> br main

>>> wmem 0x939439493 abcd