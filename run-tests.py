import os

# SYSTEM SOURCES
i = 1
while (i <= 23):
    print "./Release/AutoTester AutoTester/Test-Cases/sources/testcase"+str(i)+"-source.txt AutoTester/Test-Cases/queries/testcase"+str(i)+"-queries.txt AutoTester/Test-Cases/output/testcase"+str(i)+"-out.xml"
    i = i + 1

i = 1
while (i <= 2):
    print "./Release/AutoTester AutoTester/Test-Cases/sources/blackbox"+str(i)+"-source.txt AutoTester/Test-Cases/queries/blackbox"+str(i)+"-queries.txt AutoTester/Test-Cases/output/blackbox"+str(i)+"-out.xml"
    i = i + 1

i = 1
while (i <= 2):
    print "./Release/AutoTester AutoTester/Test-Cases/sources/extension"+str(i)+"-source.txt AutoTester/Test-Cases/queries/extension"+str(i)+"-queries.txt AutoTester/Test-Cases/output/extension"+str(i)+"-out.xml"
    i = i + 1
