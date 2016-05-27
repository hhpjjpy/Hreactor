#include "TcpClient.h"

TcpClient::TcpClient(EventLoop * loop, hhp::net::InetAddr&  addr) :
	m_loop(loop),
	m_Connector(addr),
	m_pTcpConn(NULL),
	m_isConn(m_Connector.getConnectFd()>0)
{
	if (m_isConn) {
		m_pTcpConn.reset(new TcpConnection(m_loop, m_Connector.getConnectFd()));
		m_pTcpConn->SetConnectionCallBack(std::bind(&TcpClient::handConnect, this, std::placeholders::_1));
		m_pTcpConn->SetReadCallBack(std::bind(&TcpClient::handRead, this, std::placeholders::_1, std::placeholders::_2));
		m_pTcpConn->SetCloseCallBack(std::bind(&TcpClient::handClose, this, std::placeholders::_1));
	}
}

int TcpClient::handRead(TcpConnPtr conn, std::string&& buf)
{
	m_funReadCallBack(conn,std::move(buf));
	return buf.size();
}

void TcpClient::handConnect(TcpConnPtr conn)
{
	m_funConnCallBack(conn);
}

void TcpClient::handClose(TcpConnPtr conn)
{
	m_isConn = false;
	m_pTcpConn.reset();
}
