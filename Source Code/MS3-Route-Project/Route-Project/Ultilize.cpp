#include <stdio.h>
#include "mapping.h"
#include "truck.h"

int findTruckForShipment(const struct Map* m, const struct Truck* trucks, int numTrucks, const struct Package* shipment) {
    int selectedTruckIndex = -1;
    double maxCapacityScore = 0.0;

    for (int i = 0; i < numTrucks; i++) {
        const struct Truck* currentTruck = &trucks[i];

        int availableWeight = LIMIT_WEIGHT - currentTruck->currentWeight;
        int availableVolume = LIMIT_VOLUME - currentTruck->currentVolume;

        if (shipment->p_weight <= availableWeight && shipment->p_size <= availableVolume) {
            struct Point truckLocation = currentTruck->route.points[currentTruck->route.numPoints - 1];
            double distanceToDestination = distance(&truckLocation, &shipment->dest);

            double capacityScore = ((double)availableWeight / LIMIT_WEIGHT) +
                ((double)availableVolume / LIMIT_VOLUME) -
                (distanceToDestination / 1000.0);

            if (selectedTruckIndex == -1 || capacityScore > maxCapacityScore) {
                selectedTruckIndex = i;
                maxCapacityScore = capacityScore;
            }
        }
    }

    return selectedTruckIndex;
}

void addPackage(struct Truck* tr, struct Package pkg)
{
    if (tr->n_package >= LIMIT_PACKAGE ||
        tr->currentWeight + pkg.p_weight > LIMIT_WEIGHT ||
        tr->currentVolume + pkg.p_size > LIMIT_VOLUME)
    {
        printf("Cannot add! Truck with color ID %d is full\n", tr->m_truckColor);
        return;
    }

    tr->p[tr->n_package++] = pkg; 
    tr->currentWeight += pkg.p_weight;
    tr->currentVolume += pkg.p_size;
}

double capacityLeft(const struct Truck* tr)
{
    double weightLeft = (double)(LIMIT_WEIGHT - tr->currentWeight) / LIMIT_WEIGHT * 100;
    double volumeLeft = (double)(LIMIT_VOLUME - tr->currentVolume) / LIMIT_VOLUME * 100;

    if (weightLeft < volumeLeft)
    {
        return weightLeft;
    }
    else
    {
        return volumeLeft;
    }
}

struct Path pathCalculation(const struct Map* m, struct Point begin, struct Point dest)
{
    struct Path path = { 0 };
    struct Point temp = begin;

    if (begin.row == dest.row && begin.col == dest.col)
    {
        path.po[path.p_length++] = begin;
        return path;
    }

    while (temp.col != dest.col)
    {
        if (temp.col < dest.col)
        {
            temp.col++;
        }
        else
        {
            temp.col--;
        }
        path.po[path.p_length++] = temp;
    }

    while (temp.row != dest.row)
    {
        if (temp.row < dest.row)
        {
            temp.row++;
        }
        else
        {
            temp.row--;
        }
        path.po[path.p_length++] = temp;
    }

    return path; 
}

int canAddPackage(const struct Truck* tr, const struct Package* pkg)
{
    if (pkg->p_weight < 0 || pkg->p_size < 0)
    {
        return 0; 
    }

    if (pkg->p_weight > LIMIT_WEIGHT - tr->currentWeight)
    {
        return 0; 
    }

    if (pkg->p_size > LIMIT_VOLUME - tr->currentVolume)
    {
        return 0; 
    }

    return 1; 
}

int countPackages(const struct Truck* tr)
{
    return tr->n_package;
}
