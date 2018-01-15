#ifndef _MYSTRING_HPP_
#define _MYSTRING_HPP_

/*---------------------------------------------------------------------------*/

class MyString
{

	/*---------------------------------------------------------------------------*/

private:

	/*---------------------------------------------------------------------------*/

	static const int MAX_STATIC_SIZE = 16;

	union
	{
		char* m_EndOfStorage;
		char m_StaticBuffer[MAX_STATIC_SIZE];
	};

	enum StorageType { Large = 0, Small };

	char* m_DataStart;
	char* m_DataFinish;

	// m_EndOfStorage allocated memory capacity
	// !!! not an symbols count
	int dynamicCapacity = 0;

	/*---------------------------------------------------------------------------*/

private:

	/*---------------------------------------------------------------------------*/

	void dynamicStorageGrow(int _size);

	void beginEndPointersInit(StorageType _type, int _endPos);

	void moveOperatorHelper(StorageType _type, MyString & _source, const int _length);

	void charConstructorHelper(StorageType _type, const char * _source);

	void copyToBuffer(StorageType _type, const char * _source);

	void largeConcatOperation(
			const char * _insert
		,	const int _insertSymbolsCount
		,	const int _size
	);

	void shareToLocal(char * _start,char * _bef, char * _af, int _pos);

	void shareToLocalFixedLenght(char * _start, char * _bef, char * _af, int _pos, int _len);

	void storageInsertGluing(
		StorageType _type
		,	char * _bef
		,	char * _af
		,	const char * _data
		,	int _size_need
	);

	void storageSubstringGluing(char * _start, char * target, int _pos, int _len) const;

	/*---------------------------------------------------------------------------*/

public:

	/*---------------------------------------------------------------------------*/

	MyString();

	MyString(const char * _s);

	MyString(const int _N);

	MyString(const MyString & _copy);

	MyString(MyString && _move);

	/*---------------------------------------------------------------------------*/

	MyString & operator=(const MyString & _transfer);

	MyString & operator=(MyString && _move);

	MyString & operator+=(const char * _insert);

	MyString operator+(const char * _gluing);
	
	/*---------------------------------------------------------------------------*/

	char & operator[](int _pos);

	char operator[](int _pos) const;

	char *begin();

	char *end();

	/*---------------------------------------------------------------------------*/

	const int length() const;

	const int capacity() const;

	const char * c_str () const;
	
	MyString substring(long _pos, long _len) const;

	/*---------------------------------------------------------------------------*/

	void clear();

	bool empty() const;

	void reserve(long _x);

	void insert(long _pos, const char *_data);

	void erase(long _pos, long _len);

	/*---------------------------------------------------------------------------*/

	~MyString();

	/*---------------------------------------------------------------------------*/

};

/*---------------------------------------------------------------------------*/

//user literal
MyString operator""_s(const char * _str, size_t);

/*---------------------------------------------------------------------------*/

#endif // !_MYSTRING_HPP_
