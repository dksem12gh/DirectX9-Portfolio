#include "stdafx.h"
#include "logManager.h"

HRESULT logManager::init(void)
{
	FONTMANAGER->addFont("∞ÌµÒ√º", "∞ÌµÒ√º", 16);
	m_maxOutput = 30;
	m_logList.clear();
	return S_OK;
}

void logManager::release(void)
{
	m_logList.clear();
}

void logManager::addLog(string str)
{
	char logNumber[sizeof(double)];
	m_logList.push_back((string)_itoa(m_logList.size(), logNumber, 10) + ". " + str);
}

void logManager::renderLog(void)
{
	if (!m_logList.size()) return;

	int prov = 0;
	int line = WINSIZEY - (14 * m_maxOutput);
	if (m_logList.size() > m_maxOutput)
	{
		prov = m_logList.size() - m_maxOutput;
	}

	for (int i = prov; i < m_logList.size(); i++)
	{
		FONTMANAGER->fontOut("∞ÌµÒ√º", m_logList[i], 10, line, 0xffffffff);
		line += 12;
	}
}