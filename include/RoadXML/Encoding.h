#pragma once

#include "RoadXML/RoadXML.h"

namespace RoadXML {
	//! 
	class ROADXML_API Encoding {
	public:
		// Encoding type for ALL strings of RoadXML
		enum Mode { LOCAL8 = 0, UTF8 = 1 };

		static void SetMode(Mode mode);
		static Mode GetMode();

	private:
		static Mode mMode;
	};
}
