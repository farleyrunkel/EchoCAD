#include "AppModule.h"

// ��ʼ����̬��Ա����
AppModule* AppModule::myInstance = nullptr;
std::mutex AppModule::myMutex;

