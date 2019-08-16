#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#include <glm/glm.hpp>


// a utility class with static helper methods
class Utility
{
public:

	// utilities for timing; Get() updates timers and returns time since last Get call
	static void		ResetTimer();
	static float	TickTimer();
	static float	GetDeltaTime();
	static float	GetTotalTime();

};

#endif // __UTILITIES_H_