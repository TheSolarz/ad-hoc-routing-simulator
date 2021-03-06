#ifndef STATISTICSHANDLER_H
#define STATISTICSHANDLER_H

#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

struct StatisticsHandler {
    unsigned packetsSent;
    unsigned dataPacketsSent;
    unsigned dataPacketsArrived;
    unsigned routingPacketsSent; // Will be 0 for GPSR
    unsigned dataPacketsDropped;
    unsigned avgDelay;
    double avgThroughput;

    string toString() {
        stringstream string;
        string << "Packets sent: " << packetsSent << "\n";
        string << "Data packets sent: " << dataPacketsSent << "\n";
        string << "Routing packets sent: " << routingPacketsSent << "\n";
        string << "Data packets arrived: " << dataPacketsArrived << "\n";
        string << "Data packets delivery ratio: " << fixed << setprecision(2)<< 100 * ((double) dataPacketsArrived) / ((double) dataPacketsSent) << "%\n";
        string << "Average end-to-end packet delay: " << avgDelay << "\n";
        string << "Average throughput: " << fixed << setprecision(10) << avgThroughput << "\n\n";
        
        string << "Actual metrics, in order:" << "\n";
        string << fixed << setprecision(2) << 100 * ((double) dataPacketsArrived) / ((double) dataPacketsSent) << "\n";
        string << fixed << setprecision(10) << avgThroughput << "\n";
        string << fixed << setprecision(2) << 100 * ((double) routingPacketsSent) / (double) packetsSent << "\n";
        string << avgDelay << "\n";
        
        return string.str();
    }

    /**
     * Called upon when a data packet has arrived, re-calculating the average delay and the average throughput 
     */ 
    void addPacketArrival(int delay) {
        unsigned prevDelaySum = avgDelay * dataPacketsArrived;
        double prevThroughputSum = avgThroughput * dataPacketsArrived;
        dataPacketsArrived++;
        avgDelay = (double) (prevDelaySum + delay) / (double) dataPacketsArrived;
        avgThroughput = (prevThroughputSum + 1.0 / (double) delay) / (double) dataPacketsArrived;
    }

    /**
     * Add 'amount' of packets to both amount of routing packets sent as well as total amount of packets sent
     */ 
    void addRoutingPackets(int amount) {
        packetsSent += amount;
        routingPacketsSent += amount;
    }

    /**
     * Incremeny the amount of data packets dropped
     */
    void dropDataPacket() {
        dataPacketsDropped++;
    }
};

#endif
