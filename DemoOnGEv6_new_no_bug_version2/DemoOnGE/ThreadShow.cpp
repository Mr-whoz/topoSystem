#include "ThreadShow.h"

ThreadShow::ThreadShow()
{
}

ThreadShow::~ThreadShow(void)
{
}
void ThreadShow::setFile(QString& name)
{
	deal.setTracerouteFile(name);
}
void ThreadShow::run()
{
	deal.dealTracertFile();
}