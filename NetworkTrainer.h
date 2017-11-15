#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <time.h>
#include <algorithm>
#include <stdint.h>
#include <limits.h>
#include <stdexcept>

// OpenNN includes

#include "opennn/opennn.h"
#include <opennn/multilayer_perceptron.h>
#include <opennn/perceptron_layer.h>
//#include <opennn/evolutionary_algorithm.h>
#include "training_strategy.h"
//#include "tests/*.h"
#include "variables.h"
//#include "statistics.h"
using namespace OpenNN;
using namespace std;

class NetworkTrainer {

public:
	NetworkTrainer();
	~NetworkTrainer();
	int trainNetwork(); //would be convenient to have the network available readily. global variable?
						//can load it back from the xml anytime.
private:
};