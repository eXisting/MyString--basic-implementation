#include "MyString.hpp"

#include "Messages.hpp"

#include <memory>
#include <cassert>
#include <iostream>

/*---------------------------------------------------------------------------*/

void MyString::dynamicStorageGrow(int _size)
{
	char * temp = nullptr;

	int newCapacity = MAX_STATIC_SIZE * 2;

	if (dynamicCapacity > 0)
	{
		temp = new char[dynamicCapacity];

		strncpy(temp, m_EndOfStorage, sizeof(char) * dynamicCapacity);
		delete[] m_EndOfStorage;

		newCapacity = dynamicCapacity;
	}

	while (true)
	{
		if (newCapacity >= _size)
			break;
		newCapacity *= 2;
	}

	dynamicCapacity = newCapacity;

	m_EndOfStorage = new char[dynamicCapacity];

	// if nothing remains in temp
	// clear dynamic buffer from garbage
	if ( temp != nullptr )
		strncpy(m_EndOfStorage, temp, sizeof(char) * strlen( temp ) );
	else
		m_EndOfStorage[0] = '\0';
}

/*---------------------------------------------------------------------------*/

void MyString::beginEndPointersInit(StorageType _type, int _endPos)
{
	if ( _type == StorageType::Small )
	{
		m_DataStart = m_StaticBuffer;
		m_DataFinish = m_StaticBuffer + _endPos;
		return;
	}

	m_DataStart = m_EndOfStorage;
	m_DataFinish = m_EndOfStorage + _endPos;	
}

/*---------------------------------------------------------------------------*/

void MyString::moveOperatorHelper(
		StorageType _type
	,	MyString & _source
	,	const int _length
)
{
	if ( _type == Small )
		std::swap(m_StaticBuffer, _source.m_StaticBuffer);
	else
		std::swap(m_EndOfStorage, _source.m_EndOfStorage);

	beginEndPointersInit(_type, _length);

	_source.m_EndOfStorage = _source.m_DataFinish = _source.m_DataStart = nullptr;
}

/*---------------------------------------------------------------------------*/

void MyString::charConstructorHelper(StorageType _type, const char * _source)
{	
	copyToBuffer(_type, _source);

	const int endPosition = static_cast<int>( strlen(_source) );

	beginEndPointersInit( _type,  endPosition);

	// erasing all garbage after end of usefull chars
	m_DataStart[endPosition] = '\0';
}

/*---------------------------------------------------------------------------*/

void MyString::copyToBuffer(StorageType _type, const char * _source)
{
	if ( _source == nullptr )
		throw std::logic_error(Messages::IncorrectPointer);

	const int symbolsCount = static_cast<int>( strlen(_source) );

	strncpy( _type == Small ?  m_StaticBuffer : m_EndOfStorage 
		,	_source
		,	sizeof(char) *  symbolsCount
	);
}

/*---------------------------------------------------------------------------*/

void MyString::largeConcatOperation(
		const char * _insert
	,	const int _insertSymbolsCount
	,	const int _size
)
{
	int dynamicSymbolsCount = 0;
	
	const int staticSymbolsCount =
		m_DataStart == m_StaticBuffer ?
		static_cast<int>(strlen(m_StaticBuffer)) : 0;

	if ( dynamicCapacity != 0 )
		dynamicSymbolsCount = static_cast<int>(strlen(m_EndOfStorage));

	char * buf = new char[_size];

	*buf = '\0';
	
	strncat(buf, m_StaticBuffer, sizeof(char) * staticSymbolsCount);
	strncat(buf + staticSymbolsCount, m_EndOfStorage, sizeof(char) * dynamicSymbolsCount);
	strncat(buf + staticSymbolsCount + dynamicSymbolsCount, _insert, sizeof(char) * _insertSymbolsCount);
		
	dynamicStorageGrow(_size);

	int resultSymbolsCount = staticSymbolsCount + dynamicSymbolsCount + _insertSymbolsCount;

	strncpy(m_EndOfStorage, buf, sizeof(char) * resultSymbolsCount);
	
	// remove garbage after usefull chars
	m_EndOfStorage[resultSymbolsCount] = '\0';

	beginEndPointersInit(Large, strlen(m_EndOfStorage));
}

/*---------------------------------------------------------------------------*/

void MyString::shareToLocal(
		char * _start
	,	char * _beforeBuffer
	,	char * _afterBuffer
	,	int _pos
)
{
	bool isSmall = _start == m_StaticBuffer;

	int afterEndPosition = static_cast<int>(
		strlen( (isSmall ? m_StaticBuffer : m_EndOfStorage) - _pos )
	);
	
	strncpy(_beforeBuffer, isSmall ? m_StaticBuffer : m_EndOfStorage, sizeof(char) * _pos);
		
	strncpy( 
			_afterBuffer
		,	(isSmall ? m_StaticBuffer : m_EndOfStorage) + _pos
		,	sizeof(char) * afterEndPosition
	);

	try
	{
		_beforeBuffer[_pos] = '\0';
		_afterBuffer[afterEndPosition] = '\0';
	}
	catch (const std::exception & )
	{}
}

/*---------------------------------------------------------------------------*/

void MyString::shareToLocalFixedLenght(
		char * _start
	,	char * _beforeBuffer
	,	char * _afterBuffer
	,	int _pos, int _len
)
{	
	strncpy( _beforeBuffer
		,	_start == m_StaticBuffer ? m_StaticBuffer : m_EndOfStorage
		,	sizeof(char) * _pos
	);

	strncpy( _afterBuffer
		,	(_start == m_StaticBuffer ? 
		m_StaticBuffer : m_EndOfStorage) + _pos + _len
		,	sizeof(char) * (MAX_STATIC_SIZE - _len)
	);	

	_beforeBuffer[_pos] = '\0';
}

/*---------------------------------------------------------------------------*/

void MyString::storageInsertGluing(
		StorageType _type
	,	char * _beforeBuffer
	,	char * _afterBuffer
	,	const char * _data
	,	int _size_need
)
{
	const int beforeSize = static_cast<int>( strlen(_beforeBuffer) );

	if (_type == Small)
	{
		strncpy(m_StaticBuffer, _beforeBuffer, sizeof(char) * beforeSize);
		*(m_StaticBuffer + beforeSize) = '\0';

		strcat(m_StaticBuffer, _data);
		strcat(m_StaticBuffer, _afterBuffer);

		*(m_StaticBuffer + _size_need) = '\0';
		
		return;
	}

	strncpy(m_EndOfStorage, _beforeBuffer, sizeof(char) * beforeSize);
	*(m_EndOfStorage + beforeSize) = '\0';

	strcat(m_EndOfStorage, _data);
	strcat(m_EndOfStorage, _afterBuffer);
		
	*(m_EndOfStorage + _size_need) = '\0';	
}

/*---------------------------------------------------------------------------*/

void MyString::storageSubstringGluing(
		char * _start
	,	char * target
	,	int _pos
	,	int _len
) const
{
	
	strncpy(
			target
		,	(_start == m_StaticBuffer ? m_StaticBuffer : m_EndOfStorage) + _pos
		,	sizeof(char) * _len
	);

	*(target + _len) = '\0';	
}

/*---------------------------------------------------------------------------*/

MyString::MyString()
{
	m_DataStart = m_DataFinish = m_StaticBuffer;
	memset(m_StaticBuffer, NULL, sizeof(char)*MAX_STATIC_SIZE);
}

/*---------------------------------------------------------------------------*/

MyString::MyString(const char * _s)
{
	if ( _s == nullptr || _s == '\0' )
		throw std::logic_error(Messages::IncorrectPointer);

	int size = static_cast<int>( strlen(_s) );
	
	StorageType target = StorageType::Small;

	if ( size >= MAX_STATIC_SIZE )
	{ 
		dynamicStorageGrow(size);
		target = StorageType::Large;
	}

	charConstructorHelper(target, _s);
}

/*---------------------------------------------------------------------------*/

MyString::MyString(const int _size)
{
	if ( _size < 0 )
		throw std::logic_error(Messages::IncorrectSize);

	StorageType target = StorageType::Small;

	if ( _size >= MAX_STATIC_SIZE )
	{
		target = StorageType::Large;
		dynamicStorageGrow(_size);
	}

	// finish points to end of small buffer
	beginEndPointersInit(target, _size);

	// consider buffers are empty
	// and points to nothing at the start
	*m_DataStart = 0;
}

/*---------------------------------------------------------------------------*/

MyString::MyString(const MyString & _copy)
{
	StorageType target = StorageType::Small;
	const int length = _copy.length();

	if ( length >= MAX_STATIC_SIZE )
	{
		target = StorageType::Large;
		dynamicStorageGrow(length);
	}

	charConstructorHelper( target, _copy.c_str() );
}

/*---------------------------------------------------------------------------*/

MyString::MyString(MyString && _move)
{
	StorageType target = StorageType::Small;
	const int length = _move.length();

	if ( length >= MAX_STATIC_SIZE )
	{
		target = Large;
		dynamicStorageGrow(length);
	}

	moveOperatorHelper(target, _move, length);
}

/*---------------------------------------------------------------------------*/

MyString & MyString::operator=(const MyString & _transfer)
{
	if (&_transfer == this)
		return *this;

	StorageType target = StorageType::Small;
	const int transferLength = _transfer.length();
	const char * source = _transfer.m_StaticBuffer;

	if ( transferLength >= MAX_STATIC_SIZE)
	{
		target = Large;

		if (dynamicCapacity > 0)
			delete[] m_EndOfStorage;

		dynamicStorageGrow(transferLength);
		source = _transfer.m_EndOfStorage;
	}

	charConstructorHelper(target, source);
	return *this;
}

/*---------------------------------------------------------------------------*/

MyString & MyString::operator=(MyString && _move)
{
	if (&_move == this)
		return *this;

	StorageType target = StorageType::Small;
	const int length = _move.length();
	
	if (_move.length() >= MAX_STATIC_SIZE)
	{
		target = Large;
		dynamicStorageGrow(_move.length());
	}

	moveOperatorHelper(target, _move, length);
	beginEndPointersInit(target, length);
	
	return *this;
}

/*---------------------------------------------------------------------------*/

MyString & MyString::operator+=(const char * _insert)
{
	assert(_insert != nullptr);

	const int currentSymbolsCount = length();
	const int insertSymbolsCount = static_cast<int>( strlen(_insert) );
	
	const int insertSize = insertSymbolsCount + currentSymbolsCount;

	if ( insertSize < MAX_STATIC_SIZE )
	{
		if ( currentSymbolsCount < MAX_STATIC_SIZE )
			strncpy(m_StaticBuffer + currentSymbolsCount, _insert, sizeof(char) * insertSymbolsCount);

		beginEndPointersInit(Small, insertSize);

		return *this;
	}	

	const int allocateMemory = insertSize + dynamicCapacity;
	largeConcatOperation(_insert, insertSymbolsCount, allocateMemory);

	return *this;
}

/*---------------------------------------------------------------------------*/

MyString MyString::operator+(const char * _gluing)
{
	MyString result = *this;

	result += _gluing;

	return result;
}

/*---------------------------------------------------------------------------*/

char & MyString::operator[](int _pos)
{
	if (_pos < 0 || _pos >= length())
		throw std::out_of_range(Messages::OutOfRange);

	return m_DataStart[_pos];
}

/*---------------------------------------------------------------------------*/

char MyString::operator[](int _pos) const
{
	if (_pos < 0 || _pos >= length())
		throw std::logic_error(Messages::OutOfRange);

	return m_DataStart[_pos];
}

/*---------------------------------------------------------------------------*/

const int MyString::length() const
{
	if (m_DataStart == m_DataFinish)
		return 0;

	if (m_DataStart == m_StaticBuffer)
		return static_cast<int>( strlen(m_StaticBuffer) );

	return static_cast<int>( strlen(m_EndOfStorage) );
}

/*---------------------------------------------------------------------------*/

const int MyString::capacity() const
{
	return m_DataStart == m_StaticBuffer ? 
		MAX_STATIC_SIZE : dynamicCapacity;
}

/*---------------------------------------------------------------------------*/

const char * MyString::c_str() const
{
	if ( empty() )
		return "";

	return m_DataStart == m_EndOfStorage ? 
		m_EndOfStorage : m_StaticBuffer;
}

/*---------------------------------------------------------------------------*/

void MyString::clear()
{
	if ( m_EndOfStorage && dynamicCapacity != 0 )
	{
		*m_EndOfStorage = '\0';
		dynamicCapacity = 0;
	}

	memset(m_StaticBuffer, NULL, sizeof(char) * MAX_STATIC_SIZE);

	// clearing pointers
	beginEndPointersInit(StorageType::Small, 0);
}

/*---------------------------------------------------------------------------*/

bool MyString::empty() const
{
	return length() == 0;
}

/*---------------------------------------------------------------------------*/

void MyString::reserve(long _newMemory)
{
	assert(_newMemory > 0);
	
	int allocateMemory = _newMemory;
	char * buffer = nullptr;

	//if memory already reserved
	//save previous and change memory capacity
	if (dynamicCapacity > 0)
	{
		buffer = new char[dynamicCapacity];
		strncpy(buffer, m_EndOfStorage, sizeof(char) * dynamicCapacity);

		delete[] m_EndOfStorage;

		allocateMemory += dynamicCapacity;
	}

	m_EndOfStorage = new char[allocateMemory];

	// if nothing to copy
	// ensure that dynamic buffer will not have garbage inside
	if ( buffer == nullptr )
		*m_EndOfStorage = 0;
	else
		strncpy(m_EndOfStorage, buffer, sizeof(char) * dynamicCapacity);
	
	dynamicCapacity = allocateMemory;

	beginEndPointersInit(Large, static_cast<int>( strlen(m_EndOfStorage) ) );

	delete[] buffer;
}

/*---------------------------------------------------------------------------*/

char * MyString::begin()
{
	return m_DataStart;
}

/*---------------------------------------------------------------------------*/

char * MyString::end()
{
	return m_DataFinish;
}

/*---------------------------------------------------------------------------*/

void MyString::insert(long _pos, const char * _data)
{
	assert(_pos > 0);
	assert(_data != nullptr);

	if ( empty() )
	{
		*this += _data;
		return;
	}

	StorageType target = Small;

	char *beforePositionBuffer = new char[_pos];
	char *afterPositionBuffer = new char[length() - _pos];
	
	shareToLocal(m_DataStart, beforePositionBuffer, afterPositionBuffer, _pos);

	const int resultSize = static_cast<int>( strlen(_data) ) + length();

	
	if (resultSize >= MAX_STATIC_SIZE)
	{
		target = Large;

		dynamicStorageGrow(resultSize);
	}

	storageInsertGluing(target, beforePositionBuffer, afterPositionBuffer, _data, resultSize);	

	beginEndPointersInit(target, resultSize);
}

/*---------------------------------------------------------------------------*/

void MyString::erase(long _pos, long _len)
{
	assert(!empty());
	assert(_pos >= 0 && _pos < length());
	assert(_len > 0 && _len <= length());
	assert(_pos + _len <= length());

	if (_pos == 0 && _pos + _len == length())
	{
		clear();
		return;
	}		
	
	StorageType target = Small;
	char * beforePositionBuffer = new char[_pos];
	char * afterPositionBuffer = nullptr;

	int afterSize = MAX_STATIC_SIZE - _len;

	if ( m_DataStart == m_EndOfStorage )
	{
		afterSize = static_cast<int>( strlen(m_EndOfStorage) ) - _len - _pos;
		target = Large;
	}
	
	afterPositionBuffer = new char[afterSize];

	shareToLocalFixedLenght(
			m_DataStart
		,	beforePositionBuffer
		,	afterPositionBuffer
		,	_pos, _len
	);

	char * result = new char[afterSize + _pos];

	// remove garbage after initializing
	*result = '\0';

	strncat(result, beforePositionBuffer, sizeof(char) * _pos);
	strncat(result, afterPositionBuffer, sizeof(char) * afterSize);

	int resultSize = static_cast<int>( strlen( result ) );

	if ( resultSize < MAX_STATIC_SIZE )
	{
		clear();
		target = Small;
	}
	else
		m_EndOfStorage = '\0';
	
	strncat( target == Small ? 
			m_StaticBuffer : m_EndOfStorage
		,	result
		,	sizeof(char) * resultSize
	);

	beginEndPointersInit(target, resultSize);
}

/*---------------------------------------------------------------------------*/

MyString MyString::substring(long _pos, long _len) const
{
	assert(empty() == false);
	assert(_pos >= 0);
	assert(_len == -1 || _len > 0);

	if (_len == -1)
		return *this;

	assert(_pos <= length() - _len);

	char * buf = new char[_len];

	storageSubstringGluing(m_DataStart, buf, _pos, _len);
	return MyString(buf);
}

/*---------------------------------------------------------------------------*/

MyString::~MyString()
{
	clear();

	if (dynamicCapacity != 0)
		delete[] m_EndOfStorage;
}

/*---------------------------------------------------------------------------*/

MyString operator""_s(const char * _str, size_t)
{
	return MyString(_str);
}

/*---------------------------------------------------------------------------*/
