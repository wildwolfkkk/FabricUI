#pragma once

#include "BaseViewItem.h"

class QWidget;
class QLineEdit;

class StringViewItem : public BaseViewItem
{
	Q_OBJECT

public:

  static BaseViewItem *CreateItem(
    QString const &name,
    QVariant const &value,
    ItemMetadata* metaData
    );
  static const int Priority;

	StringViewItem(
    QString const &name,
    QVariant const &value
    );
	~StringViewItem();

	virtual QWidget *getWidget() /*override*/;
  
	virtual void onModelValueChanged( QVariant const &value ) /*override*/;

  void deleteMe() { delete this; }

private:

	QLineEdit* m_edit;
  QWidget* m_widget;

protected slots:
	void OnTextEdited( const QString& text );
	void OnEditFinished();
  void onInspect();
};