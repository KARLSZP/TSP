#include<iostream>
#include<bits/stdc++.h>
#include "SimulatedAnnealing.h"

using namespace std;


SimulatedAnnealing::SimulatedAnnealing(MAP_TYPE& origin, double LOOSE_COEF, \
                                       double TEMP_INIT, double TEMP_END, \
                                       int LOOPS_PER_TEMP, double ANNEALING_COEF, \
                                       int VERBOSE): LocalSearch(origin, 0, LOOSE_COEF, 0, VERBOSE)
{
    this->TEMP_INIT = TEMP_INIT;
    this->TEMP_END = TEMP_END;
    this->LOOPS_PER_TEMP = LOOPS_PER_TEMP;
    this->ANNEALING_COEF = ANNEALING_COEF;
}

void SimulatedAnnealing::runSA()
{
    double temp_cur = TEMP_INIT;
    while (temp_cur > TEMP_END) {
        if (VERBOSE) {
            cout << "\n>> Current temperature: " << temp_cur << " > " << TEMP_END \
                 << "  |  Optimal cost: " << opt_cost << endl << endl;
        }
        for (int i = 0; i < LOOPS_PER_TEMP; i++) {
            backUp();
            double pre_dist = getOptCost();
            search();
            updateNodeWeight();
            double new_dist = currentCost();
            double delta = new_dist - pre_dist;
            if (delta < 0) {
                updateOptCost(new_dist);
                if (opt_hist.size() > 400000) {
                    vector<Individual> tmp;
                    for (size_t i = 0; i < opt_hist.size(); i += 2) {
                        tmp.push_back(opt_hist[i]);
                        if (i + 1 < opt_hist.size()) {
                            delete[] opt_hist[i + 1].second;
                        }
                    }
                    opt_hist = tmp;
                }
                saveHistory(new_dist);
            }
            else if (Metropolis(delta, temp_cur)) {
                updateOptCost(new_dist);
            }
            else {
                recover();
            }
        }
        temp_cur *= ANNEALING_COEF;
    }
    if (!checkPath()) {
        cout << "Invalid!" << endl;
        for (int i = 0; i < N; i++) {
            cout << path_bak[i] << " ";
        }
        for (int i = 0; i < N; i++) {
            cout << path[i] << " ";
        }
    }
    report();
}

void SimulatedAnnealing::report()
{
    cout << ">> press [Enter] to save results.";
    getchar();
    ofstream output;
    string save_path = SAVEPATH;
    save_path += "result.sa.tour";
    output.open(save_path, ios::out);
    for (int i = 0; i < N; i++) {
        output << path[i] << " ";
    }
    output << -1 << endl;
    output.close();

    save_path = SAVEPATH;
    save_path += "result.sa.hist";
    output.open(save_path, ios::out);
    output << "best_cost_hist" << endl;
    for (auto i : opt_hist) {
        output << i.first << " ";
    }
    output << endl;
    output << "best_route_hist" << endl;
    int step = 1;
    // int step = (opt_hist.size() - 200) / 400;
    for (size_t i = 0; i < opt_hist.size() - 200; i += step) {
        output << opt_hist[i].first << endl;
        for (int j = 0; j < N; j++) {
            output << opt_hist[i].second[j] << " ";
        }
        output << endl;
    }
    for (auto i = opt_hist.size() - 200; i < opt_hist.size(); i++) {
        output << opt_hist[i].first << endl;
        for (int j = 0; j < N; j++) {
            output << opt_hist[i].second[j] << " ";
        }
        output << endl;
    }
    output << EOF << endl;
    output.close();
}


bool SimulatedAnnealing::Metropolis(double delta, double temp_cur)
{
    return exp(-delta / temp_cur) > roulette();
}