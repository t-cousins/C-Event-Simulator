/*
 * main.cpp
 *
 *  Created on: 14 May 2021
 *      Author: t-cousins
 */

#include "simulation.h"

using namespace std;

int main(int argc, const char * argv[]) {

    vector<double> const ratesPerMin({5, 2.5});
    vector<int> const durationMins({1, 3});

    MultiThreadedSimulation* m_sim = new MultiThreadedSimulation("Sim", ratesPerMin, durationMins);
    m_sim->run_all();

    delete m_sim;
    return 0;
}
