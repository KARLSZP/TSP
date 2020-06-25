#include<iostream>
#include<bits/stdc++.h>
#include "SimulatedAnnealing.h"

using namespace std;

MAP_TYPE load(string file_path);

void welcome();

vector<double> getParams();

void checkInput(string s);

int main(int argc, char const* argv[])
{
    srand(time(NULL));
    if (argc != 2) {
        cout << "Invalid Usage." << endl;
        cout << ">> ./local_search [TSP_FILE_NAME]" << endl;
        cout << "e.g.:" << endl;
        cout << ">> ./local_search a280" << endl;
        exit(0);
    }
    string file_path = ".tsp";
    file_path = argv[1] + file_path;
    file_path = "../testcases/" + file_path;
    MAP_TYPE citys = load(file_path);

    welcome();

    // for (auto i : citys) {
    //     cout << i.first << ": " << i.second.first << " " << i.second.second << endl;
    // }

    auto params = getParams();
    double TEMP_INIT = params[0];
    double TEMP_END = params[1];
    int LOOPS_PER_TEMP = params[2];
    double LOOSE_COEF = params[3];
    double ANNEALING_COEF = params[4];
    int VERBOSE = params[5];

    SimulatedAnnealing SA(citys, LOOSE_COEF, \
                          TEMP_INIT, TEMP_END, \
                          LOOPS_PER_TEMP, ANNEALING_COEF, VERBOSE);

    cout << "=================================" << endl;
    cout << "|  User Input:                  |" << endl;
    cout << "|       TEMP_INIT : " << left << setw(12) << params[0] << "|" << endl;
    cout << "|        TEMP_END : " << left << setw(12) << params[1] << "|" << endl;
    cout << "|  LOOPS_PER_TEMP : " << left << setw(12) << params[2] << "|" << endl;
    cout << "|      LOOSE_COEF : " << left << setw(12) << params[3] << "|" << endl;
    cout << "|  ANNEALING_COEF : " << left << setw(12) << params[4] << "|" << endl;
    cout << "|         VERBOSE : " << left << setw(12) << params[5] << "|" << endl;
    cout << "=================================" << endl;
    cout << ">> Press [Enter] to run on file: " << file_path << ": ";
    getchar();

    auto t = clock();
    SA.run();
    cout << ">> Search is done in " << double(clock() - t) / CLOCKS_PER_SEC << "s." << endl;
    cout << ">> Best solution found: " << SA.getOptCost() << "." << endl;
    return 0;
}


MAP_TYPE load(string file_path)
{
    MAP_TYPE res;
    ifstream is;
    is.open(file_path, ios::in);
    string buffer;
    while (getline(is, buffer)) {
        if (buffer == "NODE_COORD_SECTION") {
            break;
        }
    }
    int id;
    double x, y;
    while (is >> id >> x >> y) {
        res[id] = make_pair(x, y);
    }
    return res;
}

void welcome()
{
    cout << "==================================================================" << endl;
    cout << "|                                                                |" << endl;
    cout << "|                      Simulated  Annealing                      |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|             a technique simulating annealing process,          |" << endl;
    cout << "|               to approach TSP's optimal solution.              |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|                        Author : Karl                           |" << endl;
    cout << "|                          Date : June 11, 2020                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
    cout << "|                                                                |" << endl;
    cout << "|      To get started, please ensure that your dataset is in     |" << endl;
    cout << "|    TSPLIB style, with EUC2D EDGE_WEIGHT_TYPE.                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|    @Parameters:                                                |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (1) TEMP_INIT: the initial temperature                     |" << endl;
    cout << "|     ... (Default: 50000)                                       |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (2) TEMP_END: the end temperature                          |" << endl;
    cout << "|     ... (Default: 0.00001, [0.01~0.00001] suggested)           |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (3) LOOPS_PER_TEMP: the loop times under                   |" << endl;
    cout << "|     ...                 a certain temperature                  |" << endl;
    cout << "|     ... (Default: 10000)                                       |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (4) LOOSE_COEF: the coefficient that controls the degree   |" << endl;
    cout << "|     ...             those active citys will slow down at.      |" << endl;
    cout << "|     ... (Default: 0.99, [0.9~0.99] suggested)                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (5) ANNEALING_COEF:  the coefficient that controls the     |" << endl;
    cout << "|     ...                  speed the temperature cools down at.  |" << endl;
    cout << "|     ... (Default: 0.99)                                        |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (6) VERBOSE: whether to print out debug message            |" << endl;
    cout << "|     ... (Default: 1)                                           |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|    NOTE: Press [ENTER] to use default value.                   |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
}

vector<double> getParams()
{
    vector<double> params(6, 0);
    string opt;
    cout << ">> TEMP_INIT[50000]:";
    getline(cin, opt);
    checkInput(opt);
    params[0] = opt.length() == 0 ? 50000 : atof(opt.c_str());

    cout << ">> TEMP_END[0.00001]:";
    getline(cin, opt);
    checkInput(opt);
    params[1] = opt.length() == 0 ? 0.00001 : atof(opt.c_str());

    cout << ">> LOOPS_PER_TEMP[10000]:";
    getline(cin, opt);
    checkInput(opt);
    params[2] = opt.length() == 0 ? 10000 : atoi(opt.c_str());

    cout << ">> LOOSE_COEF[0.99]:";
    getline(cin, opt);
    checkInput(opt);
    params[3] = opt.length() == 0 ? 0.99 : atof(opt.c_str());

    cout << ">> ANNEALING_COEF[0.99]:";
    getline(cin, opt);
    checkInput(opt);
    params[4] = opt.length() == 0 ? 0.99 : atof(opt.c_str());

    cout << ">> VERBOSE[1]:";
    getline(cin, opt);
    checkInput(opt);
    params[5] = opt.length() == 0 ? 1 : atoi(opt.c_str());

    return params;
}

void checkInput(string s)
{
    for (auto ch : s) {
        if (!isdigit(ch) && ch != '.') {
            cout << "Invalid Input." << endl;
            exit(0);
        }
    }
}