#include "Connector.h"
#include "SocketOps.h"
#include "Log.h"
using namespace hhp::net;

Connector::Connector(InetAddr& addr) :
	m_pSocket(new Socket(::createNonblockingOrDir())),
	m_addr(addr),
	m_connfd(-1)
{
}

int Connector::getConnectFd()
{
	int ret = m_pSocket->Connect_nonb(m_addr,0);
	if (ret == 0) {
	 m_connfd =  m_pSocket->getfd();
	
	}
	return m_connfd;
}
