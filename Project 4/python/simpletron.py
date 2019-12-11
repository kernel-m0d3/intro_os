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
    print("to execute from file: python3 simpletron.py [file]")
    print("enter \"exit\" to quit")
    print("enter \"input\" to enter instruction codes")
    print("enter \"end\" to save instruction codes into memory")
    print("enter \"run\" to execute instructions saved in memory\n")

def saveline(line):
    memory[programCounter] = line

def execute_line(instruction):
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
        execute_line(memory[programCounter])

def execute_instructions():
    global memory
    global programCounter
    programCounter = 0
    while memory[programCounter] != 0:
        print("executing {:02d}# ".format(programCounter), memory[programCounter])
        execute_line(memory[programCounter])
        programCounter = programCounter + 1

def interactive():
    global programCounter
    isReadingInstructions = False
    isInstructionSaved = False
    print("\nEntering Simpletron in interactive mode...\n")
    printHelp()
    while 1:
        userIn = input("{:02d}# ".format(programCounter) if isReadingInstructions else ">>>")
        try:
            userInput = int(userIn)
            if isReadingInstructions and not isInstructionSaved:
                saveline(userInput)
                programCounter = programCounter + 1
        except ValueError:
            if userIn == "exit":
                sys.exit()
            elif userIn == "status":
                printStatus()
            elif userIn == "input":
                isReadingInstructions = True
            elif userIn == "end":
                isInstructionSaved = True
                isReadingInstructions = False
            elif userIn == "run":
                execute_instructions()
            else:
                printHelp()

def load_script(filename):
    global memory
    global programCounter
    lineNumber = 0
    sml_script = open(filename, 'r')

    for line in sml_script:
        line = line.strip()
        memory[lineNumber] = int(line)
        lineNumber = lineNumber + 1

    execute_instructions()

def main():
    arr = sys.argv[1:]
    if len(arr) == 0:
        interactive()
    else:
        load_script(arr[0])

if __name__ == "__main__" : main()
