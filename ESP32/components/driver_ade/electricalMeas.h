#ifndef ELECTRICAL_MEAS_H
#define ELECTRICAL_MEAS_H
/**
 * @brief   Struct to hold values for each of the phases and the neutral wire (which might have current)
 */
typedef struct phaseWire
{
    float A;
    float B;
    float C;
    float N;
} phaseWire;

/**
 * @brief   Structure to hold rms values (voltage and current) for each of the phases.
 */
typedef struct rmsValues
{
    phaseWire voltage;
    phaseWire current;
} rmsValues;

typedef struct totalFundamental
{
    phaseWire total;
    phaseWire fundamental;
} totalFundamental;

typedef struct powerTypes
{
    totalFundamental active;
    totalFundamental reactive;
    totalFundamental apparent;
} powerTypes;

typedef struct powerMeas
{
    powerTypes typeOfPower;
    phaseWire powerFactor;
} powerMeas;

/**
 * TODO: harmonics
 * TODO: power quality
 * 
 */

typedef struct electricalMeas
{
    rmsValues rms;
    powerMeas power;
} electricalMeas;

#endif /** ELECTICAL_MEAS_H */