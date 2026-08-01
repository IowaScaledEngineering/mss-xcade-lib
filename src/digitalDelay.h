#ifndef _DIGITAL_DELAY_H
#define _DIGITAL_DELAY_H

#include <stdint.h>
#include <stdbool.h>

class DigitalDelay 
{
  public:
    /**
     * @param turnOnDelay  Time in ms the input must remain true before output becomes true
     * @param turnOffDelay Time in ms the input must remain false before output becomes false
     * @param initialState The assumed starting state of the signal (default: false)
     */
    DigitalDelay(uint32_t turnOnDelay = 100, uint32_t turnOffDelay = 2000, bool initialState = false);
    void setDelays(uint32_t turnOnDelay, uint32_t turnOffDelay);
    bool update(bool input, uint32_t currentTime);


  private:
    uint32_t onDelay;
    uint32_t offDelay;
    bool outputState;
    bool lastInputState;
    uint32_t lastChangeTime;
};

#endif
