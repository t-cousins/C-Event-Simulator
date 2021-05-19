/*
 * simulator_tests.cpp
 *
 *  Created on: 14 May 2021
 *      Author: t-cousins
 */

#include <simulation.h>
#include "gtest/gtest.h"

using namespace std;

class SimulationTest : public ::testing::Test {
protected:
    double EPSILON = 1e-2;
    int seed = 0;
    vector<double> timeDeltaMins = {0.3594, 0.7438, 0.7806};

    virtual void SetUp() {
    };

    virtual void TearDown() {
    };

    virtual void assertZeroEvents(double ratePerMin, int durationMins) {
	Simulation sim = Simulation("testSim", ratePerMin, durationMins);
	EXPECT_EQ(0, sim.getEvents().size());
    }
};


TEST_F(SimulationTest, Zero_Duration) {
    //if duration = 0 => there should be no events
    assertZeroEvents(1.2, 0);
}


TEST_F(SimulationTest, Zero_Rate) {
    //if rate = 0 => there should be no events
    assertZeroEvents(0, 1);
}


TEST_F(SimulationTest, Seeded_Events) {
    //with the above seed, the following simulation produces
    //3 events with the above _timeDeltaMins

    Simulation sim = Simulation("testSim", 2.5, 2, seed);
    list<Event*> const& events = sim.getEvents();
    EXPECT_EQ(timeDeltaMins.size(), events.size());

    int ctr = 1;
    double totalDeltaMins = 0;
    for(Event* e : events){
	EXPECT_EQ(string("testSim:event") + to_string(ctr), e->_name);
	EXPECT_NEAR(timeDeltaMins[ctr-1], e->_timeDeltaMins, EPSILON);
	totalDeltaMins += e->_timeDeltaMins;
	ctr++;
    }

    //totalDeltas must sum to less than the duration of the simulation
    EXPECT_TRUE(totalDeltaMins <= 2);
}


TEST_F(SimulationTest, MultiSim_Exception) {
	EXPECT_THROW(MultiThreadedSimulation("testMultiSim", {2.5, 3.5}, {2}), domain_error);
}
