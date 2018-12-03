#!/usr/bin/env python3

from datetime import datetime
import subprocess

with open("exampleTests.txt", "r") as inFile:
    outFile = open("testlog.txt", "w")

    lineNo = 1 # line/test counter
    fails = 0  # FAIL counter
    for line in inFile:
        # respond only to lines marked with a '$', all else assumed comments
        if(line[0] != "$"):
            continue

        # for each line (which is also a single test case):
        print("Test", lineNo, "start")

        # parse the line
        # lines should be in a "$ function arg1 [...] argN expectedOutput" format
        splitLine = line.split()
        functionName = splitLine[1]    # function to test
        functionArgs = splitLine[2:-1] # arguments/inputs to the function
        expectedOutput = splitLine[-1] # what we expect the function to return

        startTime = datetime.now()      # start time measuring
        # run the test!
        out = subprocess.run(["./a.out", functionName, *functionArgs], stdout=subprocess.PIPE, universal_newlines=True)
        response = out.stdout.split()

        endTime = datetime.now()        # complete time measurement
        deltaTime = endTime - startTime # total time elapsed during the test (includes subprocess spool-up, but not worth more accuracy)

        # report test in outfile
        if(response[-1] == expectedOutput):
            passFail = "PASS"
        else:
            passFail = "FAIL"

        outFile.write("Test " + lineNo.__str__() + ": " + passFail + "\n")
        outFile.write("Time of test: " + startTime.__str__() + "\n")
        outFile.write("Function: " + functionName.__str__() + "\n")
        outFile.write("Input: " + functionArgs.__str__() + "\n")
        outFile.write("Expected Output: " + expectedOutput.__str__() + "\n")
        outFile.write("Actual Output: " + response[-1].__str__() + "\n")
        outFile.write("Time elapsed: " + deltaTime.__str__() + "\n\n\n")

        print("Test", lineNo, "complete\n")
        lineNo += 1 # increment line/test counter

inFile.close()
outFile.close()

# add PASS/FAIL counter
with open("testlog.txt", 'r+') as outFile:
    content = outFile.read() # store all text in outfile

with open("testlog.txt", 'w+') as outFile:
    outFile.write("---TESTS COMPLETE---\n")
    outFile.write("FAILED " + fails.__str__() + "/" + (lineNo-1).__str__() + " CASES\n\n")
    outFile.write(content)
