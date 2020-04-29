/**
 * ElectricalMeas Class
 */

/**
 * @struct  EM_Wire
 */
typedef struct EM_Wire
{
    float A, B, C, N; /**< Values for each phase */
} EM_Wire;

//* ***************************** STRUCTS FOR RMS *******************************************/

/**
 * @struct  EM_RMS  RMS values (voltage and current) for each of the phases. The union makes the memory addressable
 *                  via an array (using the 'a' prefix) or the name (using the 's' prefix).    
 */
typedef struct EM_RMS
{
    union /**< RMS Currents which can be indexed by an array (suitable for a 'for' loop) or the names to be more specific. */
    {
        EM_Wire sCurrent;
        float aCurrent[4];
    };
    union 
    {
        EM_Wire sVoltage;
        float aVoltage[4];
    };
} EM_RMS;

//* ***************************** STRUCTS FOR POWER *******************************************/

/**
 * @struct  EM_PowerTypes  Each kind of power in the triangle
 */
struct EM_PowerTypes
{
    EM_Wire active, reactive, apparent; /**< This order is important for array indexing */
};

/** 
 * @struct  EM_Power    The quantity is either for all the components of only for the fundamental component.
 *                      e.g Fundamental (only the first harmonic, 50-60 Hz) active power vs total (including
 *                      all of the higher order harmonics in the 2.8 kHz band) active power.
 *                      
 *                      This struct contains 3 main variables: 1. Total powers for each phase
 *                                                             2. Fundamental powers for each phase
 *                                                             3. Power factors
*/  
typedef struct EM_Power
{
    union
    {
        struct EM_PowerTypes sTotal;
        float aTotal[3][4];
    };
    union
    {
        struct EM_PowerTypes sFundamental;
        float aFundamental[3][4];
    };
    union
    {
        EM_Wire sPowerFactor;
        float aPowerFactor[4];
    };    
} EM_Power;

//* ***************************** STRUCTS FOR GENERAL MEASUREMENTS *******************************************/

typedef struct EM_Meas {
    EM_RMS RMS;
    EM_Power Power;
} EM_Meas;