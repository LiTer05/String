#include "MyString.hpp"
#include <cstring>
#include <iostream>
#include <mutex>

// Constructors and destructors
String::String(const char* pStr)
	: m_size(strlen(pStr))

{
	m_cap = defineCap(m_size);
	m_buf = m_alloc.allocate(m_cap+1);
	strncpy(m_buf,pStr,m_size);	
}

String::String(const char* pStr,size_t length) 		
	: m_size(length)
{
	m_cap = defineCap(m_size);
	m_buf = m_alloc.allocate(m_cap+1);
	strncpy(m_buf,pStr,m_size);	
}

String::String(const String& other) 
	: m_cap (other.m_cap)
	, m_size (other.m_size)  
{
	if (m_cap == 0) {
		m_buf = nullptr;
	} else {
		m_buf = m_alloc.allocate(m_cap);
		strncpy(m_buf,other.m_buf,m_size); 
	}
}

String::String(String&& other)	
	: String()
{
	swap(other);
}

String::~String()
{
	m_alloc.deallocate(m_buf, m_cap);
	m_buf = nullptr;
	m_size = 0;
	m_cap = 0;
}

///// Accessors /////
size_t String::capacity() const
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	return m_cap;
}

size_t String::size() const
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	return m_size;
}

bool String::empty() const
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	return m_size == 0;
}

size_t String::find(const char *pSubStr) const 		
{

	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	size_t sublen = strlen(pSubStr);
	for (size_t i = 0; i <= m_size - sublen; ++i) {
      		size_t j;
      	 	for (j = 0; j < sublen ; ++j) {
      	     		if (m_buf[i + j] != pSubStr[j]) {
      	        		break;
      	      		}
		}		

      		if (j == sublen) {
      	      		return i;
      		}	
	}
	return std::string::npos;

}

char String::operator[](size_t index) const			
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	return m_buf[index];
}
bool String::operator<(const char* pStr) const
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
	return std::strcmp(m_buf,pStr) < 0;
}
///// Accessors End /////

///// Modifiers /////
void String::swap(String& other)
{
	std::unique_lock<std::shared_timed_mutex> lock(m_mutex);	
	std::swap(m_cap,other.m_cap);
	std::swap(m_size,other.m_size);
	std::swap(m_buf,other.m_buf);
}	

String& String::operator=(const String& other)		
{
	//std::unique_lock<std::shared_timed_mutex> lock(m_mutex);	
	if (this != &other) {
		String tmp(other);
		swap(tmp);
	}
	return *this;
}

String& String::operator=(String&& other)		
{
	//std::unique_lock<std::shared_timed_mutex> lock(m_mutex);	
	if (this != &other) {
		swap(other);
		String tmp;
		other.swap(tmp);
	}
	return *this;
}

void String::append(const char *pStr) 
{
	std::unique_lock<std::shared_timed_mutex> lock(m_mutex);	
	
	size_t otherlen = strlen(pStr);
	otherlen += m_size;

	char* new_buf = m_alloc.allocate(m_size);
	strncpy(new_buf,m_buf,m_size);

	if (otherlen > m_cap) {
		m_alloc.deallocate(m_buf,m_cap);
		m_buf = m_alloc.allocate(m_cap*2);

		strncpy(m_buf, new_buf, m_size);
		m_cap *=2;

	} 
	strncat(m_buf,pStr,strlen(pStr));	
	m_size = otherlen;
}
///// Modifiers End /////


size_t defineCap(size_t size)
{
    	if ((size & (size - 1)) == 0) {
        	return size;
    	} 
   	while (size & (size - 1)) {
  		size = size & (size - 1);
	}
	return size << 1;
}

std::ostream& operator<<(std::ostream& os, const String& s) 
{
	os << s.m_buf;
	return os;
}
