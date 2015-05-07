#ifndef __DATA_UTIL_H__
#define __DATA_UTIL_H__

//#include "StringUtil.h"
#include <list>
#include "cocos2d.h"
USING_NS_CC;
typedef std::list<unsigned char> DATALIST;

class DataInputUtil:public Ref
{
private:
	DATALIST in;
public:
    static DataInputUtil * create()
    {
        DataInputUtil * diu = new DataInputUtil();
        diu->autorelease();
        return diu;
    }
	int readByte();
	int readInt();
	bool readBoolean();
	int readShort();
	unsigned long long readLong();
	std::string readUTF();
	void readFully(char c[], int off, int len);
	DATALIST* getDatas();
	void putDatas(const char c[], int len);
};

class DataOutputUtil:public Ref
{
private:
	DATALIST out;
public:
    static DataOutputUtil * create()
    {
        DataOutputUtil * dou = new DataOutputUtil();
        dou->autorelease();
        return dou;
    }
	void write(unsigned char* v, unsigned long size);
	void writeByte(const int v);
	void writeInt(const int v);
	void writeBoolean(const bool v);
	void writeShort(const int v);
	void writeLong(const unsigned long long v);
	void writeUTF(const std::string &v, bool c = true);
	DATALIST* getDatas();
	void clear();
};

#endif