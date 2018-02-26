# CS3201 Group 6 SIMPLE SPA

This is an SPA prototype, which is a fully operational mini-SPA. This prototype allows any user to enter a source program, written in SIMPLE, and some queries, written as a subset of PQL. It parses the source program, builds some of the design abstractions in a Program Knowledge Base (PKB), evaluate the queries and displays the query results.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Software needed to run this SPA :

* Microsoft Visual Studio 2015

### Installing and Running the SPA

Install Microsoft Visual Studio 2015 on your machine.

Next, download a ZIP of this repository onto your machine, or clone it onto your machine using the following command on the command prompt.

```
git clone https://github.com/keloysiusmak/cs3201-group6-spa
```

Once the files have been downloaded, navigate into to the folder, and open the file *EmptyGeneralTesting.sln*.

The file should open a new Microsoft Visual Studio 2015 window. Simply click *Build > Build Solution* to generate the file *Autotester.exe*.

The file *Autotester.exe* can be found from within the *\Debug* folder. You can also access the folder using the following command from the command prompt.

```
cd C:\CS3201\Debug
```
Once you are in the *\Debug* folder, enter the folllowing command from the command prompt.

```
AutoTester Sample-Source.txt Sample-Queries.txt out.xml
```

The AutoTester accepts three arguments. The first is the name of the file containing the SIMPLE source. The second is the name of the file containing the queries. The third is the output file to store the results of the testing.

## Running Autotester with additional arguments

*-f Query-ID* runs AutoTester from a specific Query-ID onwards from the queries file.

```
AutoTester Sample-Source.txt Sample-Queries.txt out.xml -f Query-ID
```

*-n Query-ID -t Time-Allowed* runs AutoTester for a specific Query-ID in the queries file. The query evaluation is stopped after *Time-Allowed* milliseconds.

```
AutoTester Sample-Source.txt Sample-Queries.txt out.xml -n Query-ID -t Time-Allowed
```
Once the testing is complete, you can view the results of the testing by opening out.xml in Firefox.

## Built With

* [C++](http://www.cplusplus.com/) - The development language used
* [Asana](https://www.asana.com/) - Project Management
* [GitHub](https://github.com/) - Version Control

## Authors

* **CS3201 Teaching Staff** - *Initial work*
* **Keloysius Mak** - *Team Leader* - [GitHub](https://github.com/keloysiusmak)
* **Quek Kai Yu** - *Testing Head* - [GitHub](https://github.com/kaiyu92)
* **Shi Tian** - *Query Evaluator IC* - [GitHub](https://github.com/shitian007)
* **Manh Tranh** - *Parser IC* - [GitHub](https://github.com/manhtrangh)
* **Jason Chau** - *Developer* - [GitHub](https://github.com/chauvansang)
* **Tiyyagura Hyma** - *Developer* - [GitHub](https://github.com/hymss)

## Acknowledgments

* We would like to thank the CS3201 teaching staff for the guidance and support throughout this module, and for the huge amount of experience and knowledge we have gained thus far.

