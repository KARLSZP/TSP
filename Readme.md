# Readme

## 1 Repo structure

This report is structured as follow:

```bash
.
├── doc
│   ├── assets
│   ├── report.md
│   └── report.pdf
├── GeneticAlgorithm
│   ├── bin
│   ├── src
│   └── testcases
├── LocalSearch
│   ├── bin
│   ├── src
│   └── testcases
└── SimulatedAnnealing
    ├── bin
    ├── src
    └── testcases

14 directories, 2 files
```

Where:

*   `doc/`

    stores the report and assets.

*   `LocalSearch/`

    LocalSearch implementation, in standard organization(src/ for source code, bin/ for binary excutable program, testcases/ for tests).

*   `SimulatedAnnealing/`

    SimulatedAnnealing implementation, in standard organization(src/ for source code, bin/ for binary excutable program, testcases/ for tests).

*   `GeneticAlgorithm/`

    GeneticAlgorithm implementation, in standard organization(src/ for source code, bin/ for binary excutable program, testcases/ for tests).

## 2 Run

Note:

*   Method : `LocalSearch`, `SimulatedAnnealing`, `GeneticAlgorithm`
*   Excutable : `local_search`, `simulated_annealing`, `genetic_algorithm`

---

1.  Copy tsp problem file and solution file to testcases

    ```bash
    $ cp ${PATH}/${TSP_NAME}.tsp ${PATH}/${TSP_NAME}.opt.tour ${Method}/testcases/
    ```

2.  Enter the directory of algorithm you'd like to run.

    ```bash
    $ cd ${Method}/bin/
    ```

    or re-build

    ```bash
    $ cd ${Method}/src/ && make clean && make && cd ../bin/
    ```

3.  Run to solve TSP problem

    ```bash
    $ ./${Excutable} ${TSP_NAME}
    # e.g. ./local_search a280
    ```

    And then, follow the instruction in command-line.

    <img src="/home/karl/.config/Typora/typora-user-images/image-20200624162956650.png" alt="image-20200624162956650" style="zoom:67%;" />

    Remember to **press [Enter]** to save the result and history, in `testcases/`.

4.  Visualization

    >   Visualization is based on Python 3.7 and matplotlib, please ensure you've installed them on your device. 

    ```bash
    $ cd ../testcases/
    $ python Benchmarker.py [ls/sa/ga] ${TSP_NAME}
    ```

    Please choose from [ls/sa/ga] according to the method you've chosen. 

    Python script will automatically create a directory, please wait for a moment.

---

## 3 Visualization

Since the result gifs are too large to sent with Email, I build a web-demo here for your convenience, enjoy it and please be patient with those gifs which may loads from server for a while, thanks.

[Click here](http://webtest.resourcemaster.top/TSP-visualization/)

---

2020/6