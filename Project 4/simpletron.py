#!/usr/bin/python3
import sys

memory = [0] * 100
accumulator = 0
programCounter = 0

def printStatus():
    print("\nAccumulator: ", accumulator)
    print("programCounter: ", str(programCounter).zfill(2))
    print("memory: ", memory)

def printHelp():
    print("enter \"status\" to print Simpletron status")
    print("enter \"exit\" to quit")

def saveline(line):
    memory[programCounter] = line

def parseLine(instruction):
    global accumulator
    global programCounter
    branch = False
    opCode = instruction // 100
    operand = instruction % 100
    if opCode == 40:
        programCounter = operand
        branch = True
    elif opCode == 41:
        if accumulator < 0:
            programCounter = operand
            branch = True
    elif opCode == 42:
        if accumulator == 0:
            programCounter = operand
            branch = True
    elif opCode == 43:
        print("Execution Halted! Exiting...")
        sys.exit()
    elif opCode == 30:
        accumulator = accumulator + memory[operand]
    elif opCode == 31:
        accumulator = accumulator - memory[operand]
    elif opCode == 32:
        accumulator = accumulator * memory[operand]
    elif opCode == 33:
        accumulator = accumulator / memory[operand]
    elif opCode == 10:
        numInput = input("Read number: ")
        try:
            read = int(numInput)
            memory[operand] = read
        except ValueError:
            print("ERROR please try again")
    elif opCode == 11:
        print(memory[operand])
    elif opCode == 20:
        accumulator = memory[operand]
    elif opCode == 21:
        memory[operand] = accumulator

    if branch:
        parseLine(memory[programCounter])

def interactive():
    global programCounter
    print("Entering Simpletron interactive mode")
    printHelp()
    while 1:
        userIn = input(">>>")
        try:
            userInput = int(userIn)
            saveline(userInput)
            parseLine(userInput)
            programCounter = programCounter + 1
        except ValueError:
            if userIn == "exit":
                sys.exit()
            elif userIn == "status":
                printStatus()
            else:
                printHelp()

def run_script(filename):
    global memory
    global programCounter
    lineNumber = 0
    sml_script = open(filename, 'r')

    for line in sml_script:
        line = line.strip()
        memory[lineNumber] = int(line)
        lineNumber = lineNumber + 1
        print(line)

    while memory[programCounter] != 0:
        #print("executing ", memory[programCounter])
        parseLine(memory[programCounter])
        programCounter = programCounter + 1

def main():
    arr = sys.argv[1:]
    if len(arr) == 0:
        interactive()
    else:
        run_script(arr[0])

if __name__ == "__main__" : main()
