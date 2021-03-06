#ifndef QUAMODBUSDATABLOCKWIDGET_H
#define QUAMODBUSDATABLOCKWIDGET_H

#include <QWidget>

#ifdef QUA_ACCESS_CONTROL
#include <QSortFilterProxyModel>
class QUaUser;
#endif // QUA_ACCESS_CONTROL

class QUaModbusDataBlock;
class QUaModbusClientDialog;

namespace Ui {
class QUaModbusDataBlockWidget;
}

class QUaModbusDataBlockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QUaModbusDataBlockWidget(QWidget *parent = nullptr);
    ~QUaModbusDataBlockWidget();

	void bindBlock(QUaModbusDataBlock * block);

	void clear();

#ifdef QUA_ACCESS_CONTROL
	void setupPermissionsModel(QSortFilterProxyModel * proxyPerms);
signals:
	// NOTE : internal signal
	void loggedUserChanged();
public slots:
	void on_loggedUserChanged(QUaUser * user);
#endif // QUA_ACCESS_CONTROL

private:
    Ui::QUaModbusDataBlockWidget *ui;

	QList<QMetaObject::Connection> m_connections;

	void bindBlockWidgetEdit   (QUaModbusDataBlock * block );
	void bindBlockWidgetStatus (QUaModbusDataBlock * block );

	void showNewValueDialog(QUaModbusDataBlock * block, QUaModbusClientDialog & dialog);

#ifdef QUA_ACCESS_CONTROL
	QUaUser * m_loggedUser;
	QSortFilterProxyModel * m_proxyPerms;
#endif // QUA_ACCESS_CONTROL

};

#endif // QUAMODBUSDATABLOCKWIDGET_H
