#ifndef ELECTRICAL_MEAS_H
#define ELECTRICAL_MEAS_H
/**
 * @struct  phaseWire   Values for each of the phases and the neutral wire (which might have current)
 */
typedef struct phaseWire
{
    float A;
    float B;
    float C;
    float N;
} phaseWire;

/**
 * @struct  rmsValues   RMS values (voltage and current) for each of the phases.
 */
typedef struct rmsValues
{
    phaseWire voltage; /**< RMS voltages for each of the phases */
    phaseWire current; /**< RMS currents for each of the phases and the neutral. */
} rmsValues;

/** 
 * @struct  totalFundamental    The quantity is either for all the components of only for the fundamental component.
 *                              e.g Fundamental (only the first harmonic, 50-60 Hz) active power vs total (including
 *                              all of the higher order harmonics in the 2.8 kHz band) active power.
*/
typedef struct totalFundamental
{
    phaseWire total;
    phaseWire fundamental;
} totalFundamental;

/**
 * @struct  powerTypes  Active, reactive or apparent power.
*/
typedef struct powerTypes
{
    totalFundamental active;
    totalFundamental reactive;
    totalFundamental apparent;
} powerTypes;

/** 
 * @struct  powerMeas   Power measurement quantities.
*/
typedef struct powerMeas
{
    powerTypes typeOfPower;
    phaseWire powerFactor;
} powerMeas;

/**
 * @struct  singleHarmonicMonitor   Hold measurements for a single harmonic.
 */
typedef struct singleHarmonicMonitor
{
    uint8_t harmN; /**< Harmonic to be monitored */ 
    float rms; /**< rms value of the harmonic */
    float HD; /**< Harmonic distortion */
    float aPower; /**< Apparent power of the harmonic */
    float PF; /**< Power factor of the harmonic */
} singleHarmonicMonitor;

/**
 * @struct  harmonicMonitor     values for the harmonics. We can only monitor one phase and 3 harmonics (at choice)
 *                              at a time. The values for the fundamental can be also monitored but can be extracted from
 *                              other registers. 
 */
typedef struct harmonicMonitor
{
    uint8_t monitoredPhase; /**< Phase to be monitored  */
    singleHarmonicMonitor harmX;
    singleHarmonicMonitor harmY;
    singleHarmonicMonitor harmZ;
    float THD;              /**< Total harmonic distortion */
} harmonicMonitor;

/** 
 * @struct  singlePQ    Single power quality quantity.
*/
typedef struct singlePQ
{
    bool detected; /**< Whether or not the event was detected*/
    uint8_t phase; /**< On which phase it was detected*/
    float val; /**< Value for the peaks */ 
} singlePQ;

/**
 * @struct  powerQuality    Power quality related quantities.
*/
typedef struct powerQuality
{
    singlePQ peakCurrent; /**< Current peak */
    singlePQ peakVoltage; /**< Voltage peak*/
    singlePQ overCurrent; /**< overcurrent*/
    singlePQ overVoltage; /**< overvoltage*/

    singlePQ sagVoltage; /**< Sag voltage on one of the phases */
    singlePQ zeroCrossingMiss; /**< missed one zero-crossing on one of the phases */
    bool neutralMismatch; /**< Neutral  current mismatch*/

} powerQuality;



/**
 * @brief Structure to hold all of the quantities.
 */
typedef struct electricalMeas
{
    rmsValues rms;
    powerMeas power;
    harmonicMonitor harmonics;
    powerQuality PQ;

} electricalMeas;

#endif /** ELECTICAL_MEAS_H */