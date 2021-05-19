/*
 * simulation.h
 *
 *  Created on: 14 May 2021
 *      Author: t-cousins
 */

#ifndef CMAKE_SIMULATION_H
#define CMAKE_SIMULATION_H

#include <list>
#include <random>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

class Event {
//  Each Simulation produces multiple Events spaced apart by timeDeltaMins
public:
    Event(string name, double timeDeltaMins) : _name(name), _timeDeltaMins(timeDeltaMins){};
    ~Event(){};
    chrono::microseconds timeDeltaMicros() const {
        return chrono::microseconds(int(round(_timeDeltaMins*60*1000000)));
    }

public:
    const string _name;
    const double _timeDeltaMins;
};


class Listener {
//  The Listener is notified each time there is an Event and the
//  the eventTime at which it occured. One Listener per Simulation
public:
    virtual void onEvent(Event const& event, chrono::microseconds eventTime) const = 0;
    virtual ~Listener(){};
};

class testListener: public Listener {
public:
    void onEvent(Event const& event, chrono::microseconds eventTime) const {
        printf("Event Triggered: %s @ %.2fs\n", event._name.c_str(), double(eventTime.count())/1000000);
    }
    ~testListener(){};
};


class Simulation {
//  Generates a queue of Events upon instantiation & pops them to the listener when run()
public:
    Simulation(string name, double ratePerMin, int durationMins, int seed);
    Simulation(string name, double ratePerMin, int durationMins) : Simulation(name, ratePerMin, durationMins, random_device{}()){};
	void run(Listener& listener);
	list<Event*> const& getEvents(){return events;};
    ~Simulation();

private:
	double eventTimeDeltaMins();
    chrono::microseconds durationMicros() const;
    chrono::microseconds elapsedMicros(chrono::time_point<chrono::system_clock> const& startTime) const;

private:
    const string _name;
    const double _ratePerMin;
    const int _durationMins;
    mt19937 gen;
	uniform_real_distribution<double> rand;
    //  Events are stored on the heap.
    //  Pointers stored in the queue and references are passed to Listeners
    list<Event*> events;
};


class MultiThreadedSimulation {
public:
    MultiThreadedSimulation(string name, std::vector<double> const& ratesPerMin, std::vector<int> const& durationMins);
    void run_all();
    ~MultiThreadedSimulation();

private:
    vector<Simulation*> simulations;
    vector<Listener*> listeners;
    vector<thread*> threads;
};

#endif
