# SYSC4001_A3P2
SYSC 4001-L2 Assignment 3 Part 2 with Antoine

Enzo Chen, 101306299

Antoine Hickey, 101295764

## Description
<u>**This code is intended to fulfill the original requirements of SYSC 4001 Assignment 3 Part 2, before the changes made on November 18, 2025**</u>. A copy of the original requirements is included in this repository.
THe aim of this code is learn about concurrency issues by simulating a variable number of TAs who need to mark students and read exams. Each TA process can read student exams, simulated with text files containing a 4 digit ID, and a rubric, with question numbers and corresponding answers. TAs can choose to edit the rubric at random, modifying one queston at a time. Because of this, concurrency issues can occur, so a semaphore is implemented in Part B to resolve them by ensuring only one TA can access the rubric at a time.


## Usage
1. (Optional) Reset Rubric
If desired, the rubric.txt file can be reset to the starting point as specified in the requirements. Replace the existing rubric with the following text:
```
1,A
2,B
3,C
4,D
5,E
```

2. Compiling
To compile the code, run the following two commands:

Compile TA:
```
gcc ta.c -o ta
```

Compile main:
```
gcc main.c -o main
```

3. Running
To run the code, run the following command:
```
./main
```

This will create 2 TAs by default. The number of TAs created can be specified by adding a number after the command. For example:
```
./main 5
```
This command will create 5 TAs.
