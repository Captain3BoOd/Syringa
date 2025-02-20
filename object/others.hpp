#pragma once

#include <unordered_map>
#include <string>

#include "symboltable.hpp"
#include "object.hpp"
#include "types.hpp"
#include "error.hpp"
#include "token.hpp"
#include "node.hpp"

/*******************************************************************************************/
/*******************************************(Class)*****************************************/
/*******************************************************************************************/

class Class: public Object
{
public:
	Class(const std::string& class_name = "");
	inline void set(const std::string& class_name) { this->class_name = class_name; }
	inline const std::string& get() const { return this->class_name; }
	bool operator==(const Class& other) { return this->class_name == other.class_name; }

	bool __bool__() const override { return true; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::string class_name;
};

/*******************************************************************************************/
/***************************************(Instance)***************************************/
/*******************************************************************************************/

class Instance: public Object
{
public:
	Instance(const Class& _class);
	Instance(const Instance& other);

	bool __bool__() const override { return true; }
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
};

ObjectPtr make_instance(const Instance& user_object);

/*******************************************************************************************/
/********************************************(INT)******************************************/
/*******************************************************************************************/

class Int : public Object
{
public:
	Int(int64_t number = 0);

	void set(int64_t number);
	int64_t get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	int64_t number;
};

/*******************************************************************************************/
/*******************************************(FLOAT)*****************************************/
/*******************************************************************************************/

class Float : public Object
{
public:
	Float(long double number = 0.0);

	void set(const long double& number);
	long double get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	long double number;
};

/*******************************************************************************************/
/*******************************************(STRING)****************************************/
/*******************************************************************************************/

class Str : public Object
{
public:
	Str(const std::string& str = "");

	friend inline std::ostream& operator<<(std::ostream& out, const Str& str);
	std::string get() const;
	/*************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::string str;
};

/*******************************************************************************************/
/********************************************(BOOL)*****************************************/
/*******************************************************************************************/

class Bool : public Object
{
public:
	Bool(bool data);

	bool get() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	bool data;
};

/*******************************************************************************************/
/********************************************(LIST)*****************************************/
/*******************************************************************************************/

class List: public Object
{
public:
	List(const ObjectVector& objects);
	/************************************************************************/
	void append(ObjectPtr& obj);
	void append(ObjectPtr obj);
	void remove(uint64_t index);
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/*******************************************(TUPLE)*****************************************/
/*******************************************************************************************/

class Tuple: public Object
{
public:
	Tuple(const ObjectVector& elements);
	inline const ObjectVector& get() const { return this->objects; }
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/*******************************************(ARRAY)*****************************************/
/*******************************************************************************************/

class Array : public Object
{
public:
	Array(const ObjectVector& objects);
	/************************************************************************/
	const ObjectVector& get_data() const;
	/************************************************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	ObjectVector objects;
};

/*******************************************************************************************/
/********************************************(NONE)*****************************************/
/*******************************************************************************************/

class None : public Object
{
public:
	None();
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
};

/*******************************************************************************************/
/*******************************************(ERROR)*****************************************/
/*******************************************************************************************/

class Error : public Object, public Errorr
{
public:
	Error(Position start, Position end, ErrorType type, std::string details);
	/************************************/
	Position get_start_pos() const { return this->pos_start; };
	Position get_end_pos() const { return this->pos_end; };
	std::string get_details() const { return this->details; };
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
};

/*******************************************************************************************/
/****************************************(FILE STREAM)**************************************/
/*******************************************************************************************/

#include <fstream>

class FileStream : public Object
{
public:
	FileStream(const std::string& file_name, const std::string& mode);
	/************************************/
	inline std::string get_file_name() const { return this->file_name; };
	inline std::string get_mode() const { return this->mode; };
	/************************************/
	bool __bool__() const override;
	std::string __info__() const override;
	std::string __str__() const override;
	void init_class_built_in() override;
private:
	std::fstream file;
	std::string file_name;
	std::string mode;
	static const std::unordered_map<std::string, std::ios_base::openmode> mode_map;
};
