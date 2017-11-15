#pragma once

#include "stdafx.h"

#include "MarsyasAgent.h"

MarsyasAgent::MarsyasAgent()
{
	extractor = new MarsyasExtractor("file.wav");
}

MarsyasAgent::~MarsyasAgent()
{
}