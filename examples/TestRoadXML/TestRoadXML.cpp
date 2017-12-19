#include "TestRoadXML.h"
#include <stdio.h>

#include "RoadXML/Element.h"
#include "RoadXML/RoadXMLVersion.h"
#include "RoadXML/RoadXMLElements.h"


int main(int argc, char *argv[])
{
	// Get input file path
	char * sInputFilePath = NULL;
	char * sOutputFilePath = NULL;
	if( argc < 2 )
	{
		//input file path undefined
		std::cout << "ERROR : you must define a valid input file path. Program will close now." << std::endl;
		return 0;
	}
	else
	{
		sInputFilePath = argv[1];
		if( argc > 2 )
			sOutputFilePath = argv[2];
	}

	//Load Network from .rnd file name
	CountedPtr<RoadXML::Element> pLoadedRoadXML = RoadXML::Load( sInputFilePath );
	if( !pLoadedRoadXML )
	{
		//can't load file
		std::cout << "ERROR : can't parse file " << sInputFilePath << ". Program will end now." << std::endl;
		return 0;
	}

	// Print a brief description of the file content
	CountedPtr<RoadXML::NetworkElement> pNetwork = dynamic_cast<RoadXML::NetworkElement*>(pLoadedRoadXML.Get());
	if( pNetwork )
	{
		std::cout << "The RoadXML Network " << pNetwork->GetName() << " is loaded." << std::endl;

		const size_t subNetworkCount = pNetwork->mSubNetworkArray.size();
		std::cout << "	The network contains " << subNetworkCount << " sub-network." << std::endl;
		for( size_t iSub=0 ; iSub<subNetworkCount ; iSub++ )
		{
			RoadXML::SubNetworkElement* curSubNetwork = pNetwork->mSubNetworkArray[iSub];
			const size_t trackCount = curSubNetwork->mTrackArray.size();
			std::cout << "	Road Network " << curSubNetwork->GetName() << " contains:" << std::endl;
			std::cout << "		" << trackCount << " tracks" << std::endl;
			std::cout << "		" << curSubNetwork->mIntersectionArray.size() << " intersections" << std::endl;
			for( size_t iTrack=0 ; iTrack<trackCount ; iTrack++ )
			{
				RoadXML::TrackElement* curTrack = curSubNetwork->mTrackArray[iTrack];
				const size_t portionCount = curTrack->mPortions.size();
				std::cout << "		Track " << curTrack->GetName() << " is made of " << portionCount << " road portions." << std::endl;
				for( size_t iPortion=0 ; iPortion<portionCount ; iPortion++ )
				{
					RoadXML::PortionElement* curPortion = curTrack->mPortions[iPortion];
					std::cout << "			Portion " << curPortion->GetName() << " :" << std::endl;
					std::cout << "				Start with road profile " << curPortion->GetStartProfileName() << std::endl;
					std::cout << "				End at abscissa " << curPortion->mEndDistance << " with road profile " << curPortion->GetEndProfileName() << std::endl;
				}
			}

			const size_t profileCount = curSubNetwork->mProfileArray.size();
			std::cout << "	The road network contains " << profileCount << " road cross profiles." << std::endl;
			for( size_t iProf=0 ; iProf<profileCount ; iProf++ )
			{
				RoadXML::ProfileElement* curProfile = curSubNetwork->mProfileArray[iProf];
				std::cout << "	Profile " << curProfile->GetName() << " contains " << curProfile->mLaneArray.size() << " lanes." << std::endl;
			}
		}
	}

	//Save loaded Network into new output file
	if( sOutputFilePath )
	{
		std::cout << "Saving the network in " << sOutputFilePath << std::endl;
		if( !RoadXML::Save( pLoadedRoadXML, sOutputFilePath ) )
		{
			//can't save file
			std::cout << "ERROR : can't save the file." << std::endl;
		}
	}

	return 1;
}

