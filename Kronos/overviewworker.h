#pragma once

#include <QObject>

class overviewworker : public QObject
{
	Q_OBJECT

public:
	overviewworker(QObject *parent);
	~overviewworker();

private:
	DEV_HANDLE dev_handle;
	int buf_size;
	unsigned char * pImageArray;
private slots:
	void overview_init(DEV_HANDLE device_handle);
	void alloc_buffer();
	void free_buffer();
	void get_newest_img();
	void stop_worker();
signals:
	void overview_new_image(long * img, int minVal, int maxVal);
};
