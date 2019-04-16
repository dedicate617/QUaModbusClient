#include "quamodbustcpclient.h"

QUaModbusTcpClient::QUaModbusTcpClient(QUaServer *server)
	: QUaModbusClient(server)
{
	// set defaults
	type          ()->setValue("Tcp");
	networkAddress()->setValue("127.0.0.1");
	networkPort   ()->setDataType(QMetaType::UShort);
	networkPort   ()->setValue(502);
	// set initial conditions
	networkAddress()->setWriteAccess(true);
	networkPort   ()->setWriteAccess(true);
	// instantiate client
	m_workerThread.execInThread([this]() {
		// instantiate in thread so it runs on the thread
		m_modbusClient.reset(new QModbusTcpClient(nullptr));
		// defaults
		m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "127.0.0.1");
		m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter   , 502);
		// setup client (call base class method)
		this->setupModbusClient();
	});
	// handle changes
	QObject::connect(state()         , &QUaBaseVariable::valueChanged, this, &QUaModbusTcpClient::on_stateChanged         , Qt::QueuedConnection);
	QObject::connect(networkAddress(), &QUaBaseVariable::valueChanged, this, &QUaModbusTcpClient::on_networkAddressChanged, Qt::QueuedConnection);
	QObject::connect(networkPort()   , &QUaBaseVariable::valueChanged, this, &QUaModbusTcpClient::on_networkPortChanged   , Qt::QueuedConnection);
}

QUaProperty * QUaModbusTcpClient::networkAddress()
{
	return this->browseChild<QUaProperty>("networkAddress");
}

QUaProperty * QUaModbusTcpClient::networkPort()
{
	return this->browseChild<QUaProperty>("networkPort");
}

void QUaModbusTcpClient::on_stateChanged(const QVariant &value)
{
	QModbusDevice::State state = value.value<QModbusDevice::State>();
	// only allow to write connection params if not connected
	if (state == QModbusDevice::State::UnconnectedState)
	{
		networkAddress()->setWriteAccess(true);
		networkPort   ()->setWriteAccess(true);
	}
	else
	{
		networkAddress()->setWriteAccess(false);
		networkPort   ()->setWriteAccess(false);
	}
}

void QUaModbusTcpClient::on_networkAddressChanged(const QVariant & value)
{
	Q_ASSERT_X(this->getState() == QModbusDevice::State::UnconnectedState,
		"QUaModbusTcpClient::on_networkAddressChanged", 
		"Cannot change network address while connected.");
	QString strNetworkAddress = value.toString();
	// set in thread, for thread-safety
	m_workerThread.execInThread([this, strNetworkAddress]() {
		m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, strNetworkAddress);
	});
}

void QUaModbusTcpClient::on_networkPortChanged(const QVariant & value)
{
	Q_ASSERT_X(this->getState() == QModbusDevice::State::UnconnectedState,
		"QUaModbusTcpClient::on_networkPortChanged",
		"Cannot change network port while connected.");
	quint16 uiPort = value.value<quint16>();
	// set in thread, for thread-safety
	m_workerThread.execInThread([this, uiPort]() {
		m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, uiPort);
	});

}