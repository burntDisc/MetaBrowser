#pragma once

class Updatable
{
public:
	virtual void Update(double time)
	{

		prevTime =
			prevTime == 0.0 ?
			time - 0.0001 :
			currentTime;
		currentTime = time;

		timeDelta = currentTime - prevTime;
	};
protected:
	double currentTime;
	double prevTime = 0.0;
	double timeDelta;
};

