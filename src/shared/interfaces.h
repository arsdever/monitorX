#include "macros.h"

class QString;

class Base
{
};

class DataRequestInterface : public Base
{
public:
	virtual void* requestData() = 0;
};

class FlushToFileInterface : public Base
{
public:
	virtual void setFile(QString const& filename) = 0;
	virtual void flushToFile() = 0;
};