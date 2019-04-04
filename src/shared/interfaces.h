#pragma once

#include "macros.h"

class QString;

class Base
{
public:
	virtual ~Base() {}
};

class InterfaceRegistratorInterface : public Base
{
public:
	virtual void registerInterface(Base *) = 0;
};

template <typename T>
class DataConsumerInterface : public Base
{
  public:
	virtual DataConsumerInterface<T> &operator<<(T const &data) = 0;
};

struct INFO;

class InfoConsumerInterface : public DataConsumerInterface<INFO *>
{
  public:
	virtual DataConsumerInterface<INFO *> &operator<<(INFO *const &data) override = 0;
};

class DataRequestInterface : public Base
{
  public:
	virtual void *requestData() = 0;
};

class FlushToFileInterface : public Base
{
  public:
	virtual void setFile(QString const &filename) = 0;
	virtual void flushToFile() = 0;
};