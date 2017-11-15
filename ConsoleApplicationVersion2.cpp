// ConsoleVersionMLADrums.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "MarsyasAgent.h"
//#include "NetworkAgent.h"

//using namespace OpenNN;

int main()
{
	//avoid the creation of the collections programmatically - .mf files.

	MarsyasAgent* marsyasAgent = new MarsyasAgent();
	marsyasAgent->extractor->extractFeatures("center.mf", "halfedge.mf", "rimshot.mf");
	marsyasAgent->extractor->fixFile(); //separator will be ''comma'' for the OpenNN

	//train the network
	//NetworkAgent* networkAgent = new NetworkAgent();
	//networkAgent->trainer->trainNetwork();

								 //if necessary load it
								 //--------------------------loadNetwork()
								 // cout << ready for real time - proceed? ---- cin.get()

								 /*MARSYAS AND OPEN NN work together here.
								 //result = the index of the max activation for the input vector
								 //and the activation quantities
								 // use a threshold to evaluate when to play a sound or not.
								 //OPENN NN - CLASSIFIES - Verify if the value passed the threshold
								 //if yes - MARSYAS - PLAYS AN AUDIO FILE CORRESPONDING TO THAT
								 //from an SFPLAY - which is like a soundfilesource to a soundfilesink?*/

								 //--------------------------startClassifying()
								 //probably break down into sub methods
								 // -------- initDetection => marsyas initAudio for the audio source and does single vector extraction
								 //  (could do peak checking here - and tick only when above threshold, or just do the thing every N samples)
								 // -------- convertVector => convert from (MARSYAS) mrs_realvec to (OPENNN) Vector<float> or Vector<double>
								 // -------- evaluateVector => every time that marsyas is done processing N samples, OpenNN can evaluate vector
								 // (returns the vector w activation values, and an index for the highest.) (or just the index...)
								 // -------- checkThreshold => compare the value at max activation index - return true or false (pass?)
								 // -------------------------playSound() => Marsyas sets the audiofilesource to correct filename
								 // and plays it back.
								 // if(pass){playSound()}
								 //need some variables to store the names of the three audio files used.

								 //when do we stop?

	return 0;
}

