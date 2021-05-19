/*
 * simulation.cpp
 *
 *  Created on: 14 May 2021
 *      Author: t-cousins
 */

#include "simulation.h"
#include <math.h>

using namespace std;

Simulation::Simulation(string name, double ratePerMin, int durationMins, int seed) : _name(name), _ratePerMin(ratePerMin), _durationMins(durationMins), rand(0.0, 1.0), gen(seed){

    double timeDelta = this->eventTimeDeltaMins();
    double totalTime = timeDelta;
    int ctr = 1;

    while(totalTime <= durationMins){
        string eventName = _name + string(":event") + to_string(ctr);
        events.push_back(new Event(eventName, timeDelta));
        // for next loop
        timeDelta = this->eventTimeDeltaMins();
        totalTime += timeDelta;
        ctr++;
    }
};


void Simulation::run(Listener& listener) {

    printf("Starting %s \n", _name.c_str());
    auto const& startTime = chrono::system_clock::now();
    int numEvents = 0;

    while(!events.empty()){
        Event const* e = events.front();
        this_thread::sleep_for(e->timeDeltaMicros());
        listener.onEvent(*e, this->elapsedMicros(startTime));

        //first call the Event destructor to delete the object and reclaim the memory
        //next pop the pointer off the top of the queue
        //Depending on how big the queue is and how many pointers have been popped,
        //it may also decide to shrink itself down. However, this moving & copying can degrade performance.
        delete e;
        events.pop_front();
        numEvents ++;
    };

    auto leftoverDurationMicros = this->durationMicros() - this->elapsedMicros(startTime);

    if( leftoverDurationMicros.count() > 0 )
        this_thread::sleep_for(leftoverDurationMicros);

    printf("Finished %s: %d events @ %.2f events per min for %.2fs\n", _name.c_str(), numEvents, _ratePerMin, double(this->elapsedMicros(startTime).count())/1000000);
};


double Simulation::eventTimeDeltaMins() {
    //  generates random time periods between events
    //  T = time between events ~ Exp(lambda = ratePerMin)
    //  cdf(t) = 1 - e^(-lambda*t)
    //  cdf_inv(t) = -ln(t)/lambda
    //  T ~ cdf_inv(U) where U ~ U[0,1]

    //  rand(gen) returns a value in the range 0 <= x < 1
    //  but log(0) returns an error, so use 0 < 1-x <= 1

    return -log(1 - rand(gen)) /_ratePerMin;
}


chrono::microseconds Simulation::durationMicros() const {
    return chrono::microseconds(int(round(_durationMins*60*1000000)));
}


chrono::microseconds Simulation::elapsedMicros(chrono::time_point<chrono::system_clock> const& startTime) const {
    return chrono::microseconds(chrono::system_clock::now() - startTime);
}


Simulation::~Simulation(){
    // In case a Simulation is deleted before it's been run()
    while(!events.empty()){
        delete events.front();
        events.pop_front();
    }
};


MultiThreadedSimulation::MultiThreadedSimulation(string name, std::vector<double> const& ratesPerMin, std::vector<int> const& durationsMins){

    if(ratesPerMin.size() != durationsMins.size())
        throw domain_error("Inconsistent number of simulations: ratesPerMin and durationsMins must have same length");

    for( int i=0; i < ratesPerMin.size(); i++ ){
        simulations.push_back(new Simulation(name + to_string(i+1), ratesPerMin[i], durationsMins[i]));
        listeners.push_back(new testListener());
    }
}


void MultiThreadedSimulation::run_all(){

    for( int i=0; i < simulations.size(); i++ )
        threads.push_back(new thread(&Simulation::run, simulations[i], ref(*listeners[i])));

    for(thread* t : threads)
        t->join();
}


MultiThreadedSimulation::~MultiThreadedSimulation(){

    for( int i=0; i < simulations.size(); i++ ){
        delete threads[i];
        delete listeners[i];
        delete simulations[i];
    }
};
