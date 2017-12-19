#include "RoadXML/Encoding.h"

#include <iostream>

namespace RoadXML {

	Encoding::Mode Encoding::mMode = Encoding::LOCAL8;

	void Encoding::SetMode(Mode mode)
	{
		mMode = mode;

		if (mode == Encoding::LOCAL8)
			std::cout << "RoadXML Warning using legacy encoding LOCAL8" << std::endl;
	}

	Encoding::Mode Encoding::GetMode()
	{
		return mMode;
	}

}
