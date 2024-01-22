#pragma once

//TBD:: Write doxigen comments
#include <iostream>
#include <shared_mutex>

class String {
	// TBD: define iterators instead if have time...
friend std::ostream& operator<<(std::ostream& os, const String& s); 

public:
	// Constructors and destructor
	String() = default;
	String(const char *pStr);
	String(const char *pStr, size_t length);
	String(const String& other); 
	String(String&& other); 
	~String();
	
	///// Accessors /////
	size_t capacity() const;
	size_t size() const;	
	bool empty() const;
	size_t find(const char *pSubStr) const; 
	size_t findany(const char& c, size_t threadCount) const;

	char operator[](size_t index) const;
	bool operator<(const char* pStr) const;
	///// Accessors /////
	
	///// Modifiers /////
	void append(const char *pStr);
	void swap(String& other);
	
	String& operator=(const String& other);
	String& operator=(String&& other);
	///// Modifiers /////

private:
	char* m_buf = nullptr;
	size_t m_size = 0;
	size_t m_cap = 0;
	std::allocator<char> m_alloc;
	mutable std::shared_timed_mutex m_mutex;

};
