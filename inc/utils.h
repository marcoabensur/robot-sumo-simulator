#ifndef UTILS_H
#define UTILS_H


/**
 * @brief Returns x absolute value.
 */
#ifndef abs
#define abs(x) ((x) > 0 ? (x) : (-x))
#endif
/**
 * @brief Returns maximum value between x and y.
 */
#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif


/**
 * @brief Returns minimum value between x and y.
 */
#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif


/**
 * @brief Constrains v between x and y, returning at least x and at most y.
 */
#define constrain(v, x, y) ((v) < (x) ? (x) : ((v) > (y) ? (y) : (v)))

/**
 * @brief Returns the length of statically defined arrays.
 */
#define len(arr) (sizeof(arr) / sizeof(arr[0]))

/**
 * @brief Maps an integer value from one scale to another.
 *
 * @param former_value Value in former scale.
 * @param former_min   Former scale minimum value.
 * @param former_max   Former scale maximum value.
 * @param new_min      New scale minimum value.
 * @param new_max      New scale maximum value.
 *
 * @return Value in new scale
 */
inline int map(int former_value, int former_min, int former_max, int new_min, int new_max) {
    long int new_value;

    new_value = (long int) (former_value - former_min) * (new_max - new_min);
    new_value /= (former_max - former_min);
    new_value += new_min;

    return (int) new_value;
}


static inline uint16_t MAP_U16(uint16_t x, uint16_t in_min, uint16_t in_max,
                               uint16_t out_min, uint16_t out_max)
{
    return constrain((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}

static inline uint16_t MAP_I16(int16_t x, int16_t in_min, int16_t in_max,
                               int16_t out_min, int16_t out_max)
{
    return constrain((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}



#endif /* UTILS_H */
