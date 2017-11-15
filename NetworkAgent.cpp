#pragma once

#include "stdafx.h"

#include "NetworkAgent.h"

NetworkAgent::NetworkAgent()
{
	trainer = new NetworkTrainer();
}

NetworkAgent::~NetworkAgent()
{
}