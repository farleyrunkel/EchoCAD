#ifndef ECHOCAD_APPMODULE_H
#define ECHOCAD_APPMODULE_H

// singlelon class to manage the application

#include <iostream>
#include <mutex>

#include "application.h"

class AppModule {
public:
	// 获取单例实例的公共静态方法
	static AppModule* instance() {
		if (!myInstance) {
			std::lock_guard<std::mutex> guard(myMutex); // 线程安全
			if (!myInstance) {
				myInstance = new AppModule();
			}
		}
		return myInstance;
	}

	// Application object
	const ApplicationPtr& application() const { return m_application; }

	// 删除拷贝构造函数和赋值运算符
	AppModule(const AppModule&) = delete;
	AppModule& operator=(const AppModule&) = delete;

	// 示例方法
	void showMessage() {
		std::cout << "Hello, AppModule!" << std::endl;
	}

private:
	AppModule() {} // 私有构造函数

	static AppModule* myInstance; // 单例实例
	static std::mutex myMutex; // 互斥锁，确保线程安全

	ApplicationPtr m_application; // Application object
};


#endif // ECHOCAD_APPMODULE_H