#pragma once

#include <initializer_list>
#include <unordered_map>
#include <functional>
#include <utility>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "../syringa.hpp"
#include "debug.hpp"

/*************************************************************************/
template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;
/*************************************************************************/
struct SymbolInfo;
struct Node;

typedef std::vector<Node*>     NodeVector;
typedef NodeVector::iterator   NodeVecIter;
/*************************************************************************/
class SymbolTable;
class Object;

typedef Weak<Object> WeakObject;
typedef Ptr<Object>  ObjectPtr;
/*************************************************************************/
typedef std::vector<ObjectPtr>                  ObjectVector;
typedef ObjectVector::iterator                  ObjectVecIter;
typedef std::initializer_list<ObjectPtr>        ObjectList;
/*************************************************************************/
class                                           BaseFunction;
typedef Ptr<BaseFunction>                       BaseFunctionPtr;
typedef std::function<ObjectPtr(ObjectVector&)> FunctionType;
/*************************************************************************/