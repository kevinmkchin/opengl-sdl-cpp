#include <profileapi.h>

/** Retrieves the frequency of the performance counter. The frequency of
    the performance counter is fixed at system boot and is consistent
    across all processors.  */
INTERNAL inline int64 win64_counter_frequency()
{
    LOCAL_PERSIST int64 frequency = 0;
    if(!frequency)
    {
        LARGE_INTEGER perf_counter_frequency_result;
        QueryPerformanceFrequency(&perf_counter_frequency_result);
        frequency = perf_counter_frequency_result.QuadPart;
    }
    return frequency;
}

/** Retrieves the current value of the performance counter, which is a
    high resolution (<1us) time stamp that can be used for time-interval
    measurements.*/
INTERNAL inline int64 win64_get_ticks()
{
    // win64 version of get ticks
    LARGE_INTEGER ticks;
    if (!QueryPerformanceCounter(&ticks))
    {
        return -1;
    }
    return ticks.QuadPart;
}

/** Returns the time elapsed in seconds since the last timestamp call. */
INTERNAL inline real32 win64_global_timestamp()
{
    LOCAL_PERSIST int64 last_tick = win64_get_ticks();
    int64 this_tick = win64_get_ticks();
    int64 delta_tick = this_tick - last_tick;
    real32 deltatime_secs = (real32) delta_tick / (real32) win64_counter_frequency();
    last_tick = this_tick;
    return deltatime_secs;
}

/** function timers */