#pragma once
#include "singletonBase.h"
class logManager :
	public singletonBase<logManager>
{
private:
	int m_maxOutput;
	vector<string> m_logList;
public:
	HRESULT init(void);
	void release(void);
	void addLog(string str);
	void renderLog(void);

	void setOutputCount(int count) { m_maxOutput = count; }
	logManager() {};
	~logManager() {};
};

