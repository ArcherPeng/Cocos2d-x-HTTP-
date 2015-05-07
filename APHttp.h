#ifndef __C_HTTP_H__
#define __C_HTTP_H__

#include "DataUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "curl.h"
#else
#include "curl\curl.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma comment(lib,"libcurl_imp.lib")
#endif
#include "cocos2d.h"
#include <thread>
USING_NS_CC;
class APHttp:public Ref
{
public:
    static void send(const char *url, DataOutputUtil *out);
private:
	DataInputUtil in;
    void sendByThread(const char *url, DataOutputUtil *out);
    void execute( DataOutputUtil *out);
    const char * url;
	static size_t callback(char *data, size_t size, size_t nmemb, DataInputUtil *in);
    void dispatcherMsg();
};

#endif