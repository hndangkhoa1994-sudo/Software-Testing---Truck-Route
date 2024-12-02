#include <stdio.h>
#include "mapping.h"
#include "truck.h"
#include <math.h>

double roundToTwoDecimalPlaces(double value) {
    return round(value * 100) / 100;
}


int findTruckForShipment(const struct Map* m, const struct Truck* trucks, int numTrucks, const struct Package* shipment) {
    int selectedTruckIndex = -1;
    double maxCapacityScore = -1.0;

    for (int i = 0; i < numTrucks; i++) {
        const struct Truck* currentTruck = &trucks[i];
        int availableWeight = LIMIT_WEIGHT - currentTruck->currentWeight;
        int availableVolume = LIMIT_VOLUME - currentTruck->currentVolume;

        // Handle full trucks
        if (availableVolume == 0 && shipment->p_size == 0) {
            availableVolume = 0; // Shipment requires no volume, so consider this truck
        }

        // Check if the truck can accommodate the shipment
        if (shipment->p_weight <= availableWeight && shipment->p_size <= availableVolume) {
            // Add bonus for exact fits
            double exactFitBonus = (shipment->p_weight == availableWeight && shipment->p_size == availableVolume) ? 1.0 : 0.0;

            // Calculate capacity score
            double capacityScore = ((double)availableWeight / LIMIT_WEIGHT) +
                ((double)availableVolume / LIMIT_VOLUME) +
                exactFitBonus;

            // Update selected truck if it's a better fit
            if (selectedTruckIndex == -1 || capacityScore > maxCapacityScore) {
                selectedTruckIndex = i;
                maxCapacityScore = capacityScore;
            }
        }
    }

    return selectedTruckIndex;
}


void addPackage(struct Truck* tr, struct Package pkg) {
    if (tr->n_package < LIMIT_PACKAGE &&
        tr->currentWeight + pkg.p_weight <= LIMIT_WEIGHT &&
        tr->currentVolume + pkg.p_size <= LIMIT_VOLUME) {
        // Correct logic to add the package
        tr->p[tr->n_package++] = pkg;
        tr->currentWeight += pkg.p_weight;
        tr->currentVolume += pkg.p_size;
    }
    else {
        printf("Cannot add package: Exceeds capacity.\n");
    }
}

double capacityLeft(const struct Truck* tr) {
    double weightLeft = ((double)(LIMIT_WEIGHT - tr->currentWeight) / LIMIT_WEIGHT) * 100;
    double volumeLeft = ((double)(LIMIT_VOLUME - tr->currentVolume) / LIMIT_VOLUME) * 100;
    if (volumeLeft <= LIMIT_VOLUME) {  // Assuming 50 is the threshold
        return roundToTwoDecimalPlaces(weightLeft);
    }
    else {
        printf("The volume is exceed\n");
        double result = (weightLeft < volumeLeft) ? weightLeft : volumeLeft;
        return roundToTwoDecimalPlaces(result);
    }
}


struct Path pathCalculation(const struct Map* m, struct Point begin, struct Point dest) {
    struct Path path = { {0}, 0 };
    struct Point temp = begin;

    // Handle edge case where start equals destination
    if (begin.row == dest.row && begin.col == dest.col) {
        path.po[path.p_length++] = begin;
        return path;
    }

    // Move horizontally
    while (temp.col != dest.col) {
        temp.col += (temp.col < dest.col) ? 1 : -1;
        path.po[path.p_length++] = temp;
    }

    // Move vertically
    while (temp.row != dest.row) {
        temp.row += (temp.row < dest.row) ? 1 : -1;
        path.po[path.p_length++] = temp;
    }

    return path;
}

int canAddPackage(const struct Truck* tr, const struct Package* pkg) {
    if (pkg->p_weight < 0 || pkg->p_size < 0) return 0; // Invalid package
    if (pkg->p_weight > (LIMIT_WEIGHT - tr->currentWeight)) return 0; // Exceeds weight
    if (pkg->p_size > (LIMIT_VOLUME - tr->currentVolume)) return 0; // Exceeds volume
    return 1; // Package can be added
}


int countPackages(const struct Truck* tr)
{
    return tr->n_package;
}
