#include "MyString.hpp"
#include "Messages.hpp"

#include "testslib.hpp"

#include <iostream>

DECLARE_OOP_TEST(MyString_default_constructoror) 
{
	// environment initialize 

	MyString a;

	// results checking
	assert(a.length() == 0);
	assert(a.begin() == a.end());
	assert(a.empty() == true);
	assert(a.capacity() == 16);
}

DECLARE_OOP_TEST(MyString_correct_const_char_constructor) 
{
	// environment initialize 

	MyString a("Hello");
	MyString z("Seems good and legit");

	// results checking

	assert(a.length() == 5);
	assert(!strcmp(a.c_str(), "Hello"));
	assert(a.capacity() == 16);

	assert(z.length() == 20);
	assert(!strcmp(z.c_str(), "Seems good and legit"));
	assert(z.capacity() == 32);
}

DECLARE_OOP_TEST(MyString_incorrect_const_char_constructor)
{
	try
	{
		MyString a(nullptr);
		assert(!"Exception must have been thrown");
	}
	catch (const std::exception & e)
	{
		assert(!strcmp(e.what(), Messages::IncorrectPointer));
	}
}

DECLARE_OOP_TEST(MyString_correct_const_fixed_size_constructor)
{
	// environment initialize 

	MyString a(10);
	MyString b(30);
	MyString c(1);

	// results checking

	assert(a.capacity() == 16);
	assert(a.length() == 0);
	assert(c.capacity() == 16);
	assert(c.length() == 0);
	assert(b.capacity() == 32);
	assert(b.length() == 0);
}

DECLARE_OOP_TEST(MyString_incorrect_const_fixed_size_constructor)
{
	try
	{
		MyString test(-5);
		assert(!"Exception must have been thrown");
	}
	catch (const std::exception & e)
	{
		assert(!strcmp(e.what(), "Incorrect size"));
	}
}

DECLARE_OOP_TEST(MyString_const_copy_constructor)
{
	MyString source("Intel Core");
	MyString * target = new MyString(source);
	
	assert(target->length() == 10);
	assert(!strcmp(target->c_str(), "Intel Core"));

	delete target;

	assert(source.length() == 10);
	assert(!strcmp(source.c_str(), "Intel Core"));
}

DECLARE_OOP_TEST(MyString_move_constructor)
{
	// environment initialize 

	MyString source("Somthing different and long");
	MyString target = std::move(source);

	// results checking

	assert(target.length() == 27);
	assert(target.capacity() == 32);
	assert(!strcmp(target.c_str(), "Somthing different and long"));
	assert(source.empty() == true);
	assert(source.length() == 0);
}

DECLARE_OOP_TEST(MyString_copy_operator_test)
{
	// environment initialize 

	MyString target(10);
	MyString target2(10);
	MyString source("To copy");
	MyString source2("Biggest copy ever");

	// actions

	target = source;
	target2 = source2;

	// results checking

	assert(target.length() == 7);
	assert(source.length() == 7);
	assert(target.capacity() == 16);
	assert(!strcmp(target.c_str(), "To copy"));

	assert(target2.length() == 17);
	assert(source2.length() == 17);
	assert(target2.capacity() == 32);
	assert(!strcmp(target2.c_str(), "Biggest copy ever"));
}

DECLARE_OOP_TEST(MyString_move_operator_test)
{
	// environment initialize 

	char sentence[] = "Trying to test move operator";
	char short_sentence[] = "Word";

	MyString a(30);
	MyString b(1);

	// actions

	a = MyString(sentence);
	b = MyString(short_sentence);

	// results checking

	assert(a.length() == 28);
	assert(!strcmp(a.c_str(), "Trying to test move operator"));

	assert(b.length() == 4);
	assert(!strcmp(b.c_str(), "Word"));
}

DECLARE_OOP_TEST(MyString_only_read_value_test)
{
	// environment initialize 

	MyString a("Testing");

	// actions

	char g = a[6];
	char t = a[0];
	char i = a[4];

	// results checking

	assert(i == 'i');
	assert(g == 'g');
	assert(t == 'T');
}

DECLARE_OOP_TEST(MyString_only_read_value_incorrect_pos_test) 
{
	try
	{
		MyString a("Testing");
		char b = a[-1];
		assert(!"Exception must have been thrown");
	}
	catch (const std::exception & e)
	{
		assert(!strcmp(e.what(), "Out of range"));
	}
}

DECLARE_OOP_TEST(MyString_read_and_change_value_test)
{
	// environment initialize 

	MyString a("Testing");

	// actions

	a[6] = 'T';
	a[0] = 'g';
	a[4] = '-';

	// results checking

	assert(a[4] == '-');
	assert(a[6] == 'T');
	assert(a[0] == 'g');
}

DECLARE_OOP_TEST(MyString_read_and_change_position_value_incorrect_pos_test)
{
	try
	{
		MyString a("Testing");
		a[-10] = 'G';
		assert(!"Exception must have been thrown");
	}
	catch (const std::exception & e)
	{
		assert(!strcmp(e.what(), "Out of range"));
	}
}

DECLARE_OOP_TEST(MyString_empty_test)
{
	MyString a;
	MyString b(10);
	MyString c("Money");

	assert(c.empty() == false);
	assert(a.empty() == true);
	assert(b.empty() == true);
}

DECLARE_OOP_TEST(MyString_clear_test)
{
	// environment initialize 

	MyString a("Existing");

	// actions

	a.clear();

	// results checking

	assert(a.begin() == a.end());
	assert(a.capacity() == 16);
	assert(a.length() == 0);
	assert(a.empty() == true);
}

DECLARE_OOP_TEST(MyString_reserving_test)
{
	// environment initialize 

	MyString a;
	MyString b("Google maps guide");
	
	// actions

	b.reserve(100);
	a.reserve(100);
	
	// results checking

	assert(a.capacity() == 100);
	assert(a.empty() == true);

	assert(b.capacity() == 132);
	assert(b.length() == 17);
}

DECLARE_OOP_TEST(MyString_begin_end_pointers_test)
{
	// environment initialize 

	MyString a("Hello world");

	char * temp = new char[16];
	*temp = '\0';

	// actions

	int count = 0;
	for (char i : a)
		temp[count++] = i;
	
	// results checking

	assert(!strcmp(temp, a.c_str()));
	assert(a.capacity() == 16);
	assert(a.length() == 11);

	delete[] temp;
}

DECLARE_OOP_TEST(MyString_iterator_write_test)
{
	// environment initialize 

	MyString a("Hello world");

	int count = 0;
	char sentence[] = "MyStringON";

	// actions

	for (char & i : a)
		i = sentence[count++];

	// results checking

	assert(!strcmp(a.c_str(), "MyStringON"));
	assert(a.capacity() == 16);
	assert(a.length() == 10);
}

DECLARE_OOP_TEST(MyString_insert_test)
{
	MyString a("It's the true");

	a.insert(13, " ending");

	assert(a.length() == 20);
	assert(a.capacity() == 32);
	assert(!strcmp(a.c_str(), "It's the true ending"));

	MyString c("The true");

	c.insert(3, " end");

	assert(c.length() == 12);
	assert(c.capacity() == 16);
	assert(!strcmp(c.c_str(), "The end true"));

	MyString b("Very long sentence for");

	b.insert(22, " m_EndOfStorage");

	assert(b.length() == 37);
	assert(b.capacity() == 64);
	assert(!strcmp(b.c_str(), "Very long sentence for m_EndOfStorage"));

	MyString d;

	d.insert(1, "John");

	assert(d.length() == 4);
	assert(d.capacity() == 16);
	assert(!strcmp(d.c_str(), "John"));
}

DECLARE_OOP_TEST(MyString_erase_test)
{
	MyString a("It's the");

	a.erase(0, 8);

	assert(a.empty() == true);
	assert(a.capacity() == 16);
	
	MyString b("It's a new begining");

	b.erase(4, 10);

	assert(b.length() == 9);
	assert(b.capacity() == 16);
	assert(!strcmp(b.c_str(), "It'sining"));
	
	MyString c("Static");

	c.erase(2, 2);

	assert(c.length() == 4);
	assert(c.capacity() == 16);
	assert(!strcmp(c.c_str(), "Stic"));
}

DECLARE_OOP_TEST(MyString_substring_test)
{
	MyString a("It's the true ending");

	MyString b = a.substring(999, -1);

	assert(b.length() == 20);
	assert(b.capacity() == 32);
	assert(!strcmp(b.c_str(), "It's the true ending"));

	MyString a_a("Linux system");

	MyString c = a_a.substring(6, 6);

	assert(c.length() == 6);
	assert(c.capacity() == 16);
	assert(!strcmp(c.c_str(), "system"));

	MyString a_b{"Something new"};

	MyString d = a_b.substring(0, 9);

	assert(d.length() == 9);
	assert(d.capacity() == 16);
	assert(!strcmp(d.c_str(), "Something"));
}

DECLARE_OOP_TEST(MyString_add_substract_test)
{
	MyString j;
	char text[] = "Windows 10";
	MyString x = j + text;

	assert(x.length() == 10);
	assert(x.capacity() == 16);
	assert(!strcmp(x.c_str(), "Windows 10"));
	assert(j.empty() == true);

	MyString g("The best OS is");
	char sentence[] = " Windows 98";
	MyString u = g + sentence;

	assert(u.length() == 25);
	assert(u.capacity() == 32);
	assert(!strcmp(u.c_str(), "The best OS is Windows 98"));

	MyString a("Table");
	char insert[] = " in room";
	a += insert;

	assert(a.length() == 13);
	assert(a.capacity() == 16);
	assert(!strcmp(a.c_str(), "Table in room"));

	MyString a_second("Biggest table you have ever seen -");
	a_second += insert;

	assert(a_second.length() == 42);
	assert(a_second.capacity() == 128);
	assert(!strcmp(a_second.c_str(), "Biggest table you have ever seen - in room"));
}

DECLARE_OOP_TEST(MyString_user_iterals_test)
{
	MyString b = "text"_s;

	assert(b.capacity() == 16);
	assert(b.empty() == false);
	assert(b.length() == 4);
	assert(!strcmp(b.c_str(), "text"));
}