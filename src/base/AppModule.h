#ifndef ECHOCAD_APPMODULE_H
#define ECHOCAD_APPMODULE_H

// singlelon class to manage the application

#include <iostream>
#include <mutex>

#include "application.h"

class AppModule {
public:
	// ��ȡ����ʵ���Ĺ�����̬����
	static AppModule* instance() {
		if (!myInstance) {
			std::lock_guard<std::mutex> guard(myMutex); // �̰߳�ȫ
			if (!myInstance) {
				myInstance = new AppModule();
			}
		}
		return myInstance;
	}

	// Application object
	const ApplicationPtr& application() const { return m_application; }

	// ɾ���������캯���͸�ֵ�����
	AppModule(const AppModule&) = delete;
	AppModule& operator=(const AppModule&) = delete;

	// ʾ������
	void showMessage() {
		std::cout << "Hello, AppModule!" << std::endl;
	}

private:
	AppModule() {} // ˽�й��캯��

	static AppModule* myInstance; // ����ʵ��
	static std::mutex myMutex; // ��������ȷ���̰߳�ȫ

	ApplicationPtr m_application; // Application object
};


#endif // ECHOCAD_APPMODULE_H