# VirtualPageTableSimulation

## Default Setting
>PAGE SIZE = SECTOR SIZE = 16  
VIRTUAL MEMORY = 16k => MAX PAGEs AVAILABLE = 1000  
RAM = 1K => MAX FRAMEs AVAILABE = 64  
Victim page selection algorithm: Least Frequently Used(LFU)  

# What does the program do?
1. Ask for the number of pages in their program.
2. Ask for the number of frames allocated to the program.
3. Randomly generate a program counter instruction address (known as logical address).
4. Look for the physical address. 
5. If the physical address is not valid(the page not loaded into RAM yet), a page fault occurred.
