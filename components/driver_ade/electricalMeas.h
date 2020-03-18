#ifndef ELECTRICAL_MEAS_H
#define ELECTRICAL_MEAS_H


typedef struct electricalPhase
{
    float A;
    float B;
    float C;
    float N;
} electricalPhase;

typedef struct rmsValues
{
    float voltage;
    float current;
} rmsValues;

typedef struct powerTypes
{
    float active;
    float reactive;
    float apparent;
} powerTypes;

typedef struct powerMeas
{
    powerTypes total;
    powerTypes fundamental;
    float powerFactor;
} powerMeas;

struct electricalMeas
{
    /* data */
};


#endif /** ELECTICAL_MEAS_H */