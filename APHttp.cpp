#include "APHttp.h"

void APHttp::execute( DataOutputUtil *out)
{
	CURLcode cgi = curl_global_init( CURL_GLOBAL_ALL );
    if (cgi != CURLE_OK) {
		return;
    }
	CURL *curl = curl_easy_init();
	if (curl == NULL) {
		return;
	}
	// post data.
	int UNCOMPRESS_TAG = 1051311605;     //未压缩数据开始标志
	unsigned char UNCOMPRESS_TAGS[4] = {
        static_cast<unsigned char>(((UNCOMPRESS_TAG >> 24) & 0xff)),
        (unsigned char)((UNCOMPRESS_TAG >> 16) & 0xff),
        (unsigned char)((UNCOMPRESS_TAG >> 8) & 0xff),
        (unsigned char)((UNCOMPRESS_TAG >> 0) & 0xff)
    };
	DATALIST *datas = out->getDatas();
    
	int size = static_cast<int>(datas->size()) ;
	unsigned char *body = new unsigned char[size + 4];
	body[0] = (size >> 24) & 0xFF;
	body[1] = (size >> 16) & 0xFF;
	body[2] = (size >> 8) & 0xFF;
	body[3] = (size >> 0) & 0xFF;
	DATALIST::iterator i;
	int j = 0;
	for (i = datas->begin(); i != datas->end(); i++) {
		body[j + 4] = *i ^ UNCOMPRESS_TAGS[j % 4];
		j++;
	}
	/*curl_slist *plist = curl_slist_append(NULL, "Content-Type: multipart/form-data;");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);*/
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10); // 连接超时
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30); // 读取超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);//屏蔽其他消息(防止崩溃)
	curl_easy_setopt(curl, CURLOPT_POST, 1); // 启用POST提交
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body); // 提交的数据
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size + 4); // 数据长度
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, APHttp::callback); // 返回数据回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &in);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE); // 用来在屏幕上显示对服务器相关操作返回的信息 
	CURLcode res = curl_easy_perform( curl );
	curl_easy_cleanup( curl );
	curl_global_cleanup();

	delete []body;
	if (res == CURLE_OK) {
		in.readInt();
		datas = in.getDatas();
		j = 0;
		for (i = datas->begin(); i != datas->end(); i++) {
			*i ^= UNCOMPRESS_TAGS[j % 4];
			j++;
		}
        //事件分发  加入Cocos主线程
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([this,out]
        {
            this->dispatcherMsg();
            this->release();//等待系统回收
            out->release();
        });
		return ;
	} else {
		return ;
	}
}
size_t APHttp::callback(char *data, size_t size, size_t nmemb, DataInputUtil *in)
{
	std::string str = data;
	if (str.find("<html>") != str.npos) {
		return -1;
	}
	int len =static_cast<int>( size * nmemb);
	in->putDatas(data, len);
	return len;
}
void APHttp::sendByThread(const char *url, DataOutputUtil *out)
{
    out->retain();
    this->url = url;
    auto t = std::thread(&APHttp::execute, this,out);
    t.detach();
}
void APHttp::send(const char *url, DataOutputUtil *out)
{
    APHttp *http = new APHttp();
    http->retain();
    http->autorelease();
    http->sendByThread(url, out);
}
void APHttp::dispatcherMsg()
{
    CCLOG("将in传给lua");
    CCLOG("%d,%d,%s,%llu,%d",
          in.readByte(),
          in.readInt(),
          in.readUTF().c_str(),
          in.readLong(),
          in.readBoolean()
          );
    
}