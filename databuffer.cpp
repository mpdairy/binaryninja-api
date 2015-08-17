#include "binaryninjaapi.h"

using namespace BinaryNinja;
using namespace std;


DataBuffer::DataBuffer()
{
	m_buffer = BNCreateDataBuffer(nullptr, 0);
}


DataBuffer::DataBuffer(size_t len)
{
	m_buffer = BNCreateDataBuffer(nullptr, len);
}


DataBuffer::DataBuffer(const void* data, size_t len)
{
	m_buffer = BNCreateDataBuffer(data, len);
}


DataBuffer::DataBuffer(const DataBuffer& buf)
{
	m_buffer = BNDuplicateDataBuffer(buf.m_buffer);
}


DataBuffer::DataBuffer(BNDataBuffer* buf)
{
	m_buffer = buf;
}


DataBuffer::~DataBuffer()
{
	BNFreeDataBuffer(m_buffer);
}


DataBuffer& DataBuffer::operator=(const DataBuffer& buf)
{
	BNFreeDataBuffer(m_buffer);
	m_buffer = BNDuplicateDataBuffer(buf.m_buffer);
	return *this;
}


void* DataBuffer::GetData()
{
	return BNGetDataBufferContents(m_buffer);
}


const void* DataBuffer::GetData() const
{
	return BNGetDataBufferContents(m_buffer);
}


void* DataBuffer::GetDataAt(size_t offset)
{
	return BNGetDataBufferContentsAt(m_buffer, offset);
}


const void* DataBuffer::GetDataAt(size_t offset) const
{
	return BNGetDataBufferContentsAt(m_buffer, offset);
}


size_t DataBuffer::GetLength() const
{
	return BNGetDataBufferLength(m_buffer);
}


void DataBuffer::SetSize(size_t len)
{
	BNSetDataBufferLength(m_buffer, len);
}


void DataBuffer::Clear()
{
	BNClearDataBuffer(m_buffer);
}


void DataBuffer::Append(const void* data, size_t len)
{
	BNAppendDataBufferContents(m_buffer, data, len);
}


void DataBuffer::Append(const DataBuffer& buf)
{
	BNAppendDataBuffer(m_buffer, buf.m_buffer);
}


void DataBuffer::AppendByte(uint8_t val)
{
	Append(&val, 1);
}


DataBuffer DataBuffer::GetSlice(size_t start, size_t len)
{
	BNDataBuffer* result = BNGetDataBufferSlice(m_buffer, start, len);
	return DataBuffer(result);
}


uint8_t& DataBuffer::operator[](size_t offset)
{
	return ((uint8_t*)GetData())[offset];
}


const uint8_t& DataBuffer::operator[](size_t offset) const
{
	return ((const uint8_t*)GetData())[offset];
}


string DataBuffer::ToEscapedString() const
{
	char* str = BNDataBufferToEscapedString(m_buffer);
	string result = str;
	BNFreeString(str);
	return result;
}


DataBuffer DataBuffer::FromEscapedString(const string& src)
{
	return DataBuffer(BNDecodeEscapedString(src.c_str()));
}


string DataBuffer::ToBase64() const
{
	char* str = BNDataBufferToBase64(m_buffer);
	string result = str;
	BNFreeString(str);
	return result;
}


DataBuffer DataBuffer::FromBase64(const string& src)
{
	return DataBuffer(BNDecodeBase64(src.c_str()));
}


bool DataBuffer::ZlibCompress(DataBuffer& output) const
{
	BNDataBuffer* result = BNZlibCompress(output.m_buffer);
	if (!result)
		return false;
	output = DataBuffer(result);
	return true;
}


bool DataBuffer::ZlibDecompress(DataBuffer& output) const
{
	BNDataBuffer* result = BNZlibDecompress(output.m_buffer);
	if (!result)
		return false;
	output = DataBuffer(result);
	return true;
}


string BinaryNinja::EscapeString(const string& s)
{
	DataBuffer buffer(s.c_str(), s.size());
	return buffer.ToEscapedString();
}


string BinaryNinja::UnescapeString(const string& s)
{
	DataBuffer buffer = DataBuffer::FromEscapedString(s);
	return string((const char*)buffer.GetData(), buffer.GetLength());
}