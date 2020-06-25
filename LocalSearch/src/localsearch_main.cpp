#include<iostream>
#include<bits/stdc++.h>
#include "LocalSearch.h"

using namespace std;

MAP_TYPE load(string file_path);

void welcome();

vector<double> getParams();

void checkInput(string s);

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        cout << "Invalid Usage." << endl;
        cout << ">> ./local_search [TSP_FILE_NAME]" << endl;
        cout << "e.g.:" << endl;
        cout << ">> ./local_search a280" << endl;
        exit(0);
    }
    // srand(unsigned(time(NULL)));
    srand(unsigned(time(NULL)));
    string file_path = ".tsp";
    file_path = argv[1] + file_path;
    file_path = "../testcases/" + file_path;
    MAP_TYPE citys = load(file_path);

    welcome();

    auto params = getParams();
    int LOOP_LIMITS = params[0];
    double LOOSE_COEF = params[1];
    int EARLY_STOP = params[2];
    int VERBOSE = params[3];

    LocalSearch LS(citys, LOOP_LIMITS, LOOSE_COEF, EARLY_STOP, VERBOSE);

    cout << "==============================" << endl;
    cout << "|  User Input:               |" << endl;
    cout << "|  LOOP_LIMITS : " << left << setw(12) << params[0] << "|" << endl;
    cout << "|   LOOSE_COEF : " << left << setw(12) << params[1] << "|" << endl;
    cout << "|   EARLY_STOP : " << left << setw(12) << params[2] << "|" << endl;
    cout << "|      VERBOSE : " << left << setw(12) << params[3] << "|" << endl;
    cout << "==============================" << endl;
    cout << ">> Press [Enter] to run on file: " << file_path << ": ";
    getchar();

    auto t = clock();
    LS.run();
    cout << ">> Search is done in " << double(clock() - t) / CLOCKS_PER_SEC << "s." << endl;
    cout << ">> Best solution found: " << LS.getOptCost() << "." << endl;
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
    cout << "|                         Local  Search                          |" << endl;
    cout << "|         a technique to approach TSP's optimal solution         |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|                        Author : Karl                           |" << endl;
    cout << "|                          Date : June 9, 2020                   |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
    cout << "|                                                                |" << endl;
    cout << "|      To get started, please ensure that your dataset is in     |" << endl;
    cout << "|    TSPLIB style, with EUC2D EDGE_WEIGHT_TYPE.                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|    @Parameters:                                                |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (1) LOOP_LIMITS: the maximum times that local search run   |" << endl;
    cout << "|     ... (Default: 1000000)                                     |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (2) LOOSE_COEF: the coefficient that controls the degree   |" << endl;
    cout << "|     ...             those active citys will slow down at.      |" << endl;
    cout << "|     ... (Default: 0.99, [0.9~0.99] suggested)                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (3) EARLY_STOP: the loops after which when no better       |" << endl;
    cout << "|     ...              solutions have been found                 |" << endl;
    cout << "|     ... (Default: 500000)                                      |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (4) VERBOSE: whether to print out debug message            |" << endl;
    cout << "|     ... (Default: 1)                                           |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|    NOTE: Press [ENTER] to use default value.                   |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
}

vector<double> getParams()
{
    vector<double> params(4, 0);
    string opt;
    cout << ">> LOOP_LIMITS[1000000]:";
    getline(cin, opt);
    checkInput(opt);
    params[0] = opt.length() == 0 ? 1000000 : atoi(opt.c_str());

    cout << ">> LOOSE_COEF[0.99]:";
    getline(cin, opt);
    checkInput(opt);
    params[1] = opt.length() == 0 ? 0.99 : atof(opt.c_str());

    cout << ">> EARLY_STOP[500000]:";
    getline(cin, opt);
    checkInput(opt);
    params[2] = opt.length() == 0 ? 500000 : atoi(opt.c_str());

    cout << ">> VERBOSE[1]:";
    getline(cin, opt);
    checkInput(opt);
    params[3] = opt.length() == 0 ? 1 : atoi(opt.c_str());

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