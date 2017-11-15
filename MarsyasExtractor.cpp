#include "stdafx.h"

#include "MarsyasExtractor.h"

#include <iostream> //keep
#include <fstream>  //keep
#include <algorithm> //keep

MarsyasExtractor::MarsyasExtractor(string sfName1) {

	arffFileName = "out.arff";

	//cout << "extracting some features into a single vector" << endl;

	net = mng.create("Series", "net");
	net->addMarSystem(mng.create("SoundFileSource", "src"));

	MarSystem* spectrumOperation = mng.create("Series", "spectrumOperation");

	spectrumOperation->addMarSystem(mng.create("Windowing", "ham"));
	spectrumOperation->addMarSystem(mng.create("Spectrum", "spec"));
	spectrumOperation->addMarSystem(mng.create("PowerSpectrum", "pspk"));

	net->addMarSystem(spectrumOperation);

	MarSystem* featureFanout = mng.create("Fanout", "featureFanout");
	featureFanout->addMarSystem(mng.create("Centroid", "centroid"));
	featureFanout->addMarSystem(mng.create("Rolloff", "rolloff"));
	featureFanout->addMarSystem(mng.create("MFCC", "mfcc"));
	featureFanout->addMarSystem(mng.create("Kurtosis", "kurtosis"));
	featureFanout->addMarSystem(mng.create("Skewness", "skewness"));
	//featureFanout->addMarSystem(mng.create("SFM", "sfm"));
	//featureFanout->addMarSystem(mng.create("SCF", "scf"));

	net->addMarSystem(featureFanout);

	MarSystem* acc = mng.create("Accumulator", "acc");
	acc->addMarSystem(net);

	total = mng.create("Series", "total");
	total->addMarSystem(acc);
	total->updControl("Accumulator/acc/mrs_natural/nTimes", 300);//should it be 20?
	total->addMarSystem(mng.create("Mean", "mn"));               //or leave as it is?

	total->addMarSystem(mng.create("Annotator", "ann"));
	total->addMarSystem(mng.create("WekaSink", "wsink"));

	total->updControl("WekaSink/wsink/mrs_natural/nLabels", 3);
	total->updControl("WekaSink/wsink/mrs_natural/downsample", 1);
	total->updControl("WekaSink/wsink/mrs_string/labelNames", "x,y,z");

	total->updControl("WekaSink/wsink/mrs_string/filename", arffFileName);

	net->updControl("SoundFileSource/src/mrs_string/filename", sfName1);
	net->linkControl("mrs_bool/hasData", "SoundFileSource/src/mrs_bool/hasData");

	total->updControl("mrs_natural/inSamples", 512);// how many samples is in our things? enough?
}

MarsyasExtractor::~MarsyasExtractor() {
}

//needs to become realtime so filename not needed.
//change the source into a audio source.
//might need a different class :( :\

/*void MarsyasExtractor::extractSingleVector() {
MarControlPtr ctrl_hasData = net->getctrl("mrs_bool/hasData");

while (ctrl_hasData->to<mrs_bool>())
{
total->tick();
const mrs_realvec& src_data =
total->getctrl("mrs_realvec/processedData")->to<mrs_realvec>();
cout << src_data << endl;
}
}*/

void MarsyasExtractor::extractFeatures(string colName1, string colName2, string colName3)
{
	centerCollection = colName1;//"center.mf";
	halfEdgeCollection = colName2;//"halfedge.mf";
	rimshotCollection = colName3;//"rimshot.mf";

	Collection center;
	center.read(centerCollection);

	total->updControl("Annotator/ann/mrs_real/label", (mrs_real)0);
	for (mrs_natural i = 0; i < center.size(); ++i)
	{
		total->updControl("Accumulator/acc/Series/net/SoundFileSource/src/mrs_string/filename", center.entry(i));
		cout << "Processing " << center.entry(i) << endl;
		total->tick();
		cout << "i = " << i << endl;

	}

	Collection halfedge;
	halfedge.read(halfEdgeCollection);

	total->updControl("Annotator/ann/mrs_real/label", (mrs_real)1);

	for (mrs_natural i = 0; i < halfedge.size(); ++i)
	{
		total->updControl("Accumulator/acc/Series/net/SoundFileSource/src/mrs_string/filename", halfedge.entry(i));
		cout << "Processing " << halfedge.entry(i) << endl;
		total->tick();
		cout << "i=" << i << endl;
	}


	Collection rimshot;
	rimshot.read(rimshotCollection);

	total->updControl("Annotator/ann/mrs_real/label", (mrs_real)2);

	for (mrs_natural i = 0; i < rimshot.size(); ++i)
	{
		total->updControl("Accumulator/acc/Series/net/SoundFileSource/src/mrs_string/filename", rimshot.entry(i));
		cout << "Processing " << rimshot.entry(i) << endl;
		total->tick();
		cout << "i=" << i << endl;
	}


	cout << "Done Processing" << endl;
	cin.get();
}

void MarsyasExtractor::fixFile()
{
	//replace x by 1,0,0, y by 0,1,0 , z by 0,0,1
	const char* filename = arffFileName.c_str();
	const char* temp_name = "data.dat";

	std::ifstream  src(filename, std::ifstream::in);
	std::ofstream  dst(temp_name, std::ofstream::out);

	string line;

	while (!src.eof()) {
		getline(src, line);

		if (line.length() == 0 || line[0] == '%' || line[0] == '@') {
			//cout << "IGNORE LINE\n";
		}
		else {
			int pos;
			std::string first("x");
			while ((pos = line.find(first)) != std::string::npos)
				line.replace(pos, first.length(), "1,0,0");
			std::string second("y");
			while ((pos = line.find(second)) != std::string::npos)
				line.replace(pos, second.length(), "0,1,0");
			std::string third("z");
			while ((pos = line.find(third)) != std::string::npos)
				line.replace(pos, third.length(), "0,0,1");

			dst << line << "\n";
		}
	}

}