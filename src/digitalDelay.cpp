#include "digitalDelay.h"

DigitalDelay::DigitalDelay(uint32_t turnOnDelay, uint32_t turnOffDelay, bool initialState) 
{
  this->onDelay = turnOnDelay;
  this->offDelay = turnOffDelay;
  this->lastInputState = this->outputState = initialState;
  this->lastChangeTime = 0;
}

void DigitalDelay::setDelays(uint32_t turnOnDelay, uint32_t turnOffDelay) 
{
  onDelay = turnOnDelay;
  offDelay = turnOffDelay;
}

/**
 * Processes the current input and returns the delayed output state.
 * 
 * @param input       The current raw digital input
 * @param currentTime The current system time in milliseconds
 * @return            The filtered/delayed state
 */
bool DigitalDelay::update(bool input, uint32_t currentTime) 
{
  // 1. If the raw input just changed, reset our debounce timer
  if (input != lastInputState) {
    lastChangeTime = currentTime;
    lastInputState = input;
  }

  // 2. If the input is different from our confirmed output, evaluate the timer
  if (input != outputState) 
  {
    // Pick the correct delay threshold depending on the direction of the change
    uint32_t requiredDelay = input ? onDelay : offDelay;

    // Check if enough time has elapsed. 
    // Note: (currentTime - lastChangeTime) naturally handles uint32_t overflow!
    if ((currentTime - lastChangeTime) >= requiredDelay) 
    {
        outputState = input; // Timer expired, commit the new state
    }
  }

  return outputState;
}
