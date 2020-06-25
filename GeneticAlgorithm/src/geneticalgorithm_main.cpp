#include "GeneticAlgorithm.h"

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

    auto params = getParams();
    int POPULATION_SIZE = params[0];
    int GENERATIONS = params[1];
    int LOOPS_INIT = params[2];
    int MUTATION_TIMES = params[3];
    double CROSSOVER_PROB = params[4];
    double MUTATION_PROB = params[5];
    double OPT_COST = params[6];
    int VERBOSE = params[7];

    GeneticAlgorithm GA(citys, LOOPS_INIT, \
                        POPULATION_SIZE, GENERATIONS, \
                        MUTATION_TIMES, CROSSOVER_PROB, \
                        MUTATION_PROB, OPT_COST, VERBOSE);

    cout << "==================================" << endl;
    cout << "|  User Input:                   |" << endl;
    cout << "|       LOOPS_INIT : " << left << setw(12) << params[2] << "|" << endl;
    cout << "|  POPULATION_SIZE : " << left << setw(12) << params[0] << "|" << endl;
    cout << "|      GENERATIONS : " << left << setw(12) << params[1] << "|" << endl;
    cout << "|   MUTATION_TIMES : " << left << setw(12) << params[3] << "|" << endl;
    cout << "|   CROSSOVER_PROB : " << left << setw(12) << params[4] << "|" << endl;
    cout << "|    MUTATION_PROB : " << left << setw(12) << params[5] << "|" << endl;
    cout << "|         OPT_COST : " << left << setw(12) << params[6] << "|" << endl;
    cout << "|          VERBOSE : " << left << setw(12) << params[7] << "|" << endl;
    cout << "==================================" << endl;
    cout << ">> Press [Enter] to run on file: " << file_path << ": ";
    getchar();

    auto t = clock();
    GA.run();
    cout << ">> Search is done in " << double(clock() - t) / CLOCKS_PER_SEC << "s." << endl;
    cout << ">> Best solution found: " << GA.getElite() << "." << endl;
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
    cout << "|                       Genetic  Algorithm                       |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|             a technique simulating biological reproduction,    |" << endl;
    cout << "|               to approach TSP's optimal solution.              |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|                        Author : Karl                           |" << endl;
    cout << "|                          Date : June 20, 2020                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
    cout << "|                                                                |" << endl;
    cout << "|      To get started, please ensure that your dataset is in     |" << endl;
    cout << "|    TSPLIB style, with EUC2D EDGE_WEIGHT_TYPE.                  |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|    @Parameters:                                                |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (1) POPULATION_SIZE: the size of population in GA.         |" << endl;
    cout << "|     ... (Default: 500)                                         |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (2) GENERATIONS: the iterations GA will do.                |" << endl;
    cout << "|     ... (Default: 10000)                                       |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (3) LOOPS_INIT: the loops of local search as seeds for GA. |" << endl;
    cout << "|     ... (Default: 1000)                                        |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (4) MUTATION_TIMES: the times each generation will take.   |" << endl;
    cout << "|     ... (Default: 2)                                           |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (5) CROSSOVER_PROB: probability of crossover operator.     |" << endl;
    cout << "|     ... (Default: 0.7)                                         |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (6) MUTATION_PROB: probability of mutation operator.       |" << endl;
    cout << "|     ... (Default: 0.1)                                         |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (7) OPT_COST: optimal solution of current tsp problem.     |" << endl;
    cout << "|     ... (Default: 0.0)                                         |" << endl;
    cout << "|                                                                |" << endl;
    cout << "|     (6) VERBOSE: whether to print out debug message            |" << endl;
    cout << "|     ... (Default: 1)                                           |" << endl;
    cout << "|    NOTE: Press [ENTER] to use default value.                   |" << endl;
    cout << "|                                                                |" << endl;
    cout << "==================================================================" << endl;
}

vector<double> getParams()
{
    vector<double> params(8, 0);
    string opt;
    cout << ">> POPULATION_SIZE[500]:";
    getline(cin, opt);
    checkInput(opt);
    params[0] = opt.length() == 0 ? 500 : atoi(opt.c_str());

    cout << ">> GENERATIONS[10000]:";
    getline(cin, opt);
    checkInput(opt);
    params[1] = opt.length() == 0 ? 10000 : atoi(opt.c_str());

    cout << ">> LOOPS_INIT[1000]:";
    getline(cin, opt);
    checkInput(opt);
    params[2] = opt.length() == 0 ? 1000 : atoi(opt.c_str());

    cout << ">> MUTATION_TIMES[2]:";
    getline(cin, opt);
    checkInput(opt);
    params[3] = opt.length() == 0 ? 2 : atoi(opt.c_str());

    cout << ">> CROSSOVER_PROB[0.7]:";
    getline(cin, opt);
    checkInput(opt);
    params[4] = opt.length() == 0 ? 0.7 : atof(opt.c_str());

    cout << ">> MUTATION_PROB[0.1]:";
    getline(cin, opt);
    checkInput(opt);
    params[5] = opt.length() == 0 ? 0.1 : atof(opt.c_str());

    cout << ">> OPT_COST[0.0]:";
    getline(cin, opt);
    checkInput(opt);
    params[6] = opt.length() == 0 ? 0.0 : atof(opt.c_str());

    cout << ">> VERBOSE[1]:";
    getline(cin, opt);
    checkInput(opt);
    params[7] = opt.length() == 0 ? 1 : atoi(opt.c_str());

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