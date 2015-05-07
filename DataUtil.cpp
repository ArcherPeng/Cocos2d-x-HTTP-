#include "DataUtil.h"
#include "cocos2d.h"

int DataInputUtil::readByte()
{
	char ch = in.front();
	in.pop_front();
	return ch;
}
int DataInputUtil::readInt()
{
	int ch1 = in.front();
	in.pop_front();
    int ch2 = in.front();
	in.pop_front();
    int ch3 = in.front();
	in.pop_front();
    int ch4 = in.front();
	in.pop_front();
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
}
bool DataInputUtil::readBoolean()
{
	int ch = in.front();
	in.pop_front();
	return (ch != 0);
}
int DataInputUtil::readShort()
{
	int ch1 = in.front();
	in.pop_front();
    int ch2 = in.front();
	in.pop_front();
    return (ch1 << 8) + (ch2 << 0);
}
unsigned long long DataInputUtil::readLong()
{
	int ch1 = in.front();
	in.pop_front();
	int ch2 = in.front();
	in.pop_front();
	int ch3 = in.front();
	in.pop_front();
	int ch4 = in.front();
	in.pop_front();
	int ch5 = in.front();
	in.pop_front();
	int ch6 = in.front();
	in.pop_front();
	int ch7 = in.front();
	in.pop_front();
	int ch8 = in.front();
	in.pop_front();
	return (((unsigned long long)ch1 << 56)
		+ ((unsigned long long)(ch2 & 255) << 48)
		+ ((unsigned long long)(ch3 & 255) << 40)
		+ ((unsigned long long)(ch4 & 255) << 32)
		+ ((unsigned long long)(ch5 & 255) << 24)
		+ ((ch6 & 255) << 16)
		+ ((ch7 & 255) <<  8)
		+ ((ch8 & 255) <<  0));
}
std::string DataInputUtil::readUTF()
{
	std::string str;
	int utflen = readShort();
	for (int i = 0; i < utflen; i++) {
		str.push_back(in.front());
		in.pop_front();
	}
	return str;
}
void DataInputUtil::readFully(char c[], int off, int len)
{
	int size = off + len;
	for (int i = off; i < size; i++) {
		c[i] = in.front();
		in.pop_front();
	}
}
DATALIST* DataInputUtil::getDatas()
{
	return &in;
}
void DataInputUtil::putDatas(const char c[], int len)
{
	for (int i = 0; i < len; i++) {
		in.push_back(c[i]);
	}
}

void DataOutputUtil::write(unsigned char* v, unsigned long size)
{
	for (unsigned long i = 0; i < size; i++) {
		out.push_back(v[i]);
	}
}
void DataOutputUtil::writeByte(const int v)
{
	out.push_back(v);
}
void DataOutputUtil::writeInt(const int v)
{
	out.push_back((v >> 24) & 0xFF);
	out.push_back((v >> 16) & 0xFF);
	out.push_back((v >>  8) & 0xFF);
	out.push_back((v >>  0) & 0xFF);
}
void DataOutputUtil::writeBoolean(const bool v)
{
	out.push_back(v ? 1 : 0);
}
void DataOutputUtil::writeShort(const int v)
{
	out.push_back((v >> 8) & 0xFF);
    out.push_back((v >> 0) & 0xFF);
}
void DataOutputUtil::writeLong(const unsigned long long v)
{
	out.push_back((v >> 56) & 0xFF);
	out.push_back((v >> 48) & 0xFF);
	out.push_back((v >> 40) & 0xFF);
	out.push_back((v >> 32) & 0xFF);
	out.push_back((v >> 24) & 0xFF);
	out.push_back((v >> 16) & 0xFF);
	out.push_back((v >>  8) & 0xFF);
	out.push_back((v >>  0) & 0xFF);
}
void DataOutputUtil::writeUTF(const std::string &v, bool c)
{
	std::string str = v;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	 if (c)
		str = StringUtil::string2UTF8(v);
#endif
	const char *arr = str.c_str();
	int utflen = static_cast<int>(strlen(arr));

	writeShort(utflen);
	for (int i = 0; i < utflen; i++) {
		out.push_back(arr[i]);
	}

}
DATALIST* DataOutputUtil::getDatas()
{
	return &out;
}
void DataOutputUtil::clear()
{
//	return out.clear();
}