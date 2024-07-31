#include "AppModule.h"

// 初始化静态成员变量
AppModule* AppModule::myInstance = nullptr;
std::mutex AppModule::myMutex;

