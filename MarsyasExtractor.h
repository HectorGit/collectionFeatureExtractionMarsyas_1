#pragma once

#include "stdafx.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iomanip>

#include <marsyas/common_source.h>
#include <marsyas/Collection.h>
#include <marsyas/NumericLib.h>
#include <marsyas/CommandLineOptions.h>
#include <marsyas/FileName.h>
#include <marsyas/system/MarSystemManager.h>
#include <marsyas/marsystems/MarSystemTemplateBasic.h>
#include <marsyas/marsystems/MarSystemTemplateAdvanced.h>
#include <marsyas/sched/EvValUpd.h>

#include <marsyas/marsystems/Spectrum2ACMChroma.h>

#include <time.h>

#ifdef MARSYAS_MIDIIO
#include <RtMidi.h>
#endif

#ifdef MARSYAS_AUDIOIO
#include <RtAudio.h>
#endif

#ifdef MARSYAS_PNG
#include <pngwriter.h>
#endif

using namespace std;
using namespace Marsyas;

#include "MarsyasExtractor.h"

class MarsyasExtractor {

public:
	MarsyasExtractor(string sfName1);
	~MarsyasExtractor();
	//void extractSingleVector(); //this needs to become realtime.
	void extractFeatures(string colName1, string colName2, string colName3);
	void fixFile();//will read file and try to make it a readable .dat file for OpenNN.

private:
	MarSystemManager mng;
	MarSystem* net;
	MarSystem* total;
	string centerCollection;
	string halfEdgeCollection;
	string rimshotCollection;
	bool collection;
	string arffFileName;
};