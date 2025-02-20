#pragma once

#include "object.hpp"

ObjectPtr AssignObjects(const ObjectPtr& obj);
inline bool check_type(const ObjectPtr& obj, ObjectType type) { return (*obj) == type; }
inline void AssignObjects(ObjectPtr& obj1, const ObjectPtr& obj2) { obj1 = AssignObjects(obj2); };
/******************************/
ObjectPtr make_function(const std::string& fun_name, const FunctionType& function, uint64_t params_number = 0, uint64_t co_argcount = 0, const ObjectVector& defaults = {});
ObjectPtr make_function(const std::string& fun_name, const std::vector<Fun::Argument>& parameters, uint64_t co_argcount, uint64_t default_count, const NodeVector& body);
ObjectPtr make_module(const SymbolTable& symbol_table, const std::string& module_name, const std::string& module_path);
ObjectPtr make_error(const Position& start, const Position& end, ErrorType type, std::string details);
ObjectPtr make_class(const std::string& class_name, SymbolTable* parent = nullptr);
ObjectPtr make_file_stream(const std::string& file_name, const std::string& mode);
ObjectPtr make_array(const ObjectVector& elements);
ObjectPtr make_tuple(const ObjectVector& elements);
ObjectPtr make_list(const ObjectVector& elements);
ObjectPtr make_instance(const ObjectPtr& _class);
ObjectPtr make_pointer(const WeakObject& object);
ObjectPtr make_pointer(const ObjectPtr& object);
ObjectPtr make_float(const long double& num);
ObjectPtr make_str(const std::string& str);
ObjectPtr make_bool(bool data);
ObjectPtr make_int(int64_t num);
ObjectPtr make_none();
