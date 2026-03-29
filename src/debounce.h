#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_

#include <stdint.h>
#include <stdbool.h>

template <typename T> class Debouncer {
	public:
		Debouncer(T initValue = 0)
		{
			this->init(initValue);
		} 

		void init(T initValue)
		{
			this->clock_A = this->clock_B = 0;
			this->debouncedState = initValue;
		}

		T debounce(T rawInputs)
		{
			this->clock_A ^= this->clock_B;                     //Increment the counters
			this->clock_B  = ~this->clock_B;
		
			T delta = rawInputs ^ this->debouncedState;   //Find all of the changes
			this->clock_A &= delta;                       //Reset the counters if no changes
			this->clock_B &= delta;                       //were detected.
		
			T changes = ~((~delta) | this->clock_A | this->clock_B);
			this->debouncedState ^= changes;
			return(changes & ~(this->debouncedState));
		}

		T getDebouncedState()
		{
			return this->debouncedState;
		}

	private:
		T clock_A;
		T clock_B;
		T debouncedState;
};

#endif