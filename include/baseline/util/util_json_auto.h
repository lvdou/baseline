
/*
* Copyright (c) 2011-2012 Promit Roy
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
//////////////////////////////////////////////////////////////
// From: http://ventspace.wordpress.com/2012/10/08/c-json-serialization/
// FileName: json_serializer_helper.hpp
// Modify: Sachin
// Date: 2013/9/22 13:41
// Description:
//
// History:
// <author> <time> <descript>
// Sachin 2013/9/22 add
//////////////////////////////////////////////////////////////
#ifndef UTIL_JSON_AUTO_H
#define UTIL_JSON_AUTO_H

#pragma once

#include "../include/common.h"
//#include <lib_json/json_lib.h>     //json2cpp
#include <boost/utility.hpp>
#define NVP(name) #name, name
#define SerializeNVP(name) Serialize(NVP(name))
#define DeSerializeNVP(name) DeSerialize(NVP(name))

namespace baseline
{
namespace util
{
	namespace json
	{
        namespace auto
        {

			class autoSerialize
			{
			private:
				//SFINAE garbage to detect whether a type has a Serialize member
				struct serialize_not_found {};
				typedef serialize_not_found SerializeNotFound;
				struct SerializeFound { char x[2]; };

				template<typename T, void (T::*)(autoSerialize&) const>
				struct SerializeTester { };

				template<typename T>
				static SerializeFound SerializeTest(SerializeTester<T, &T::Serialize>*);

				template<typename T>
				static SerializeNotFound SerializeTest(...);

				template<typename T>
				struct HasSerialize
				{
					static const bool value = sizeof(SerializeTest<T>(0)) == sizeof(SerializeFound);
				};

				//Serialize using a free function defined for the type (default fallback)
				template<typename TValue>
				void SerializeImpl(const TValue& value,typename boost::disable_if<HasSerialize<const TValue> >::type* dummy = 0)
				{
					//prototype for the serialize free function, so we will get a link error if it's missing
					//this way we don't need a header with all the serialize functions for misc types (eg math)
					void SerializeFail(const TValue&, autoSerialize&);
					SerializeFail(value, *this);
				}
				//Serialize using a member function Serialize(JsonSerializer&)
				template<typename TValue>
				void SerializeImpl(const TValue& value, typename boost::enable_if<HasSerialize<const TValue> >::type* dummy = 0)
				{
					value.Serialize(*this);
				}

			private:
				//
				struct deserialize_not_found {};
				typedef deserialize_not_found DeSerializeNotFound;
				struct DeSerializeFound { char x[2]; };

				template<typename T, void (T::*)(const autoSerialize&)>
				struct DeSerializeTester { };

				template<typename T>
				static DeSerializeFound DeSerializeTest(DeSerializeTester<T, &T::DeSerialize>*);

				template<typename T>
				static DeSerializeNotFound DeSerializeTest(...);

				template<typename T>
				struct HasDeSerialize
				{
					static const bool value = sizeof(DeSerializeTest<T>(0)) == sizeof(DeSerializeFound);
				};

				//Serialize using a free function defined for the type (default fallback)
				template<typename TValue>
				void DeSerializeImpl(TValue& value, typename boost::disable_if<HasDeSerialize<TValue> >::type* dummy = 0) const
				{
					void DeSerializeFail(TValue&, const autoSerialize&);
					DeSerializeFail(value, *this);
				}
				//Serialize using a member function Serialize(JsonSerializer&)
				template<typename TValue>
				void DeSerializeImpl(TValue& value, typename boost::enable_if<HasDeSerialize<TValue> >::type* dummy = 0) const
				{
					value.DeSerialize(*this);
				}

			public:
				autoSerialize(){ }

				template<typename TKey, typename TValue>
				void Serialize(TKey key, const TValue& value, typename boost::enable_if<boost::is_class<TValue> >::type* dummy = 0)
				{
					// class to json
					autojson subVal;
					subVal.SerializeImpl(value);
					JsonValue[key] = subVal.JsonValue;
				}

				template<typename TKey, typename TValue>
				void DeSerialize(TKey key, TValue& value, typename boost::enable_if<boost::is_class<TValue> >::type* dummy = 0) const
				{
					// json to class
					autojson subVal;
					subVal.JsonValue = JsonValue[key];
					subVal.DeSerializeImpl(value);
				}

				template<typename TKey>
				void Serialize(TKey key, const Json::Value& value)
				{
					Write(key, value);
				}

				template<typename TKey>
				void DeSerialize(TKey key, Json::Value& value) const
				{
					Read(key, value);
				}

				//Serialize a string value
				template<typename TKey>
				void Serialize(TKey key, const std::string& value)
				{
					Write(key, value);
				}

				//DeSerialize a string value
				template<typename TKey>
				void DeSerialize(TKey key, std::string& value) const
				{
					Read(key, value);
				}

				//Serialize a non class type directly using JsonCpp
				template<typename TKey, typename TValue>
				void Serialize(TKey key, const TValue& value, typename boost::enable_if<boost::is_fundamental<TValue> >::type* dummy = 0)
				{
					Write(key, value);
				}

				template<typename TKey, typename TValue>
				void DeSerialize(TKey key, TValue& value, typename boost::enable_if<boost::is_fundamental<TValue> >::type* dummy = 0) const
				{
					Read(key, value);
				}

				//Serialize an enum type to JsonCpp
				template<typename TKey, typename TEnum>
				void Serialize(TKey key, const TEnum& value, typename boost::enable_if<boost::is_enum<TEnum> >::type* dummy = 0)
				{
					int ival = (int) value;
					Write(key, ival);
				}

				template<typename TKey, typename TEnum>
				void DeSerialize(TKey key, TEnum& value, typename boost::enable_if<boost::is_enum<TEnum> >::type* dummy = 0) const
				{
					int ival = (int) value;
					Read(key, ival);
					value = (TEnum) ival;
				}

				template<typename TKey, typename TValue>
				void Serialize(TKey key, const std::vector<TValue>& vec)
				{
					Write(key, vec.begin(), vec.end());
				}

				template<typename TKey, typename TValue>
				void DeSerialize(TKey key, std::vector<TValue>& vec) const
				{
					autojson subVal;
					subVal.JsonValue = JsonValue[key];
					subVal.Read(vec);
				}

			private:
				Json::Value JsonValue;
				template<typename TKey, typename TValue>
				void Write(TKey key, const TValue& value)
				{
					JsonValue[key] = value;
				}

				template<typename TKey, typename TValue>
				void Write(TKey key, const std::vector<TValue>& vec)
				{
					autojson subVal;
					int index = 0;
					for(typename std::vector<TValue>::const_iterator it = vec.begin(); it != vec.end(); ++it)
					{
						subVal.Serialize(index, *it);
						++index;
					}
					JsonValue[key] = subVal.JsonValue;
				}

				template<typename TKey, typename TItor>
				void Write(TKey key, TItor first, TItor last)
				{
					autojson subVal;
					int index = 0;
					for(TItor it = first; it != last; ++it)
					{
						subVal.Serialize(index, *it);
						++index;
					}
					JsonValue[key] = subVal.JsonValue;
				}

				template<typename TKey, typename TValue>
				void Read(TKey key, TValue& value, typename boost::enable_if<boost::is_arithmetic<TValue> >::type* dummy = 0) const
				{
					int ival = 0 ;
					if (JsonValue[key].isNumeric())
					{
						ival = JsonValue[key].asInt();
					}
					else
					{
						//assert(false);
					}
					value = (TValue) ival;
				}

				template<typename TKey, typename TValue>
				void Read(TKey key, TValue& value) const
				{
					value = JsonValue[key];
				}

				template<typename TKey>
				void Read(TKey key, bool& value) const
				{
					bool bval = false ;
					bval = JsonValue[key].asBool();
					value = bval;
				}

				template<typename TKey>
				void Read(TKey key, int& value) const
				{
					int ival = 0 ;
					if (JsonValue[key].isNumeric())
					{
						ival = JsonValue[key].asInt();
					}
					else if (JsonValue[key].isString())
					{
						ival = atoi(JsonValue[key].asCString());
					}
					else
					{
						//assert(false);
					}
					value = ival;
				}

				template<typename TKey>
				void Read(TKey key, unsigned int& value) const
				{
					unsigned int uival = 0 ;
					if (JsonValue[key].isNumeric())
					{
						uival = JsonValue[key].asUInt();
					} else if (JsonValue[key].isString())
					{
						uival = atoi(JsonValue[key].asCString());
					} else {
						//assert(false);
					}
					value = uival;
				}

				template<typename TKey>
				void Read(TKey key, float& value) const
				{
					float fval = 0.0 ;
					if (JsonValue[key].isNumeric())
					{
						fval = JsonValue[key].asFloat();
					}
					else if (JsonValue[key].isString())
					{
						fval = atof(JsonValue[key].asCString());
					}
					else
					{
						//assert(false);
					}
					value = fval;
				}

				template<typename TKey>
				void Read(TKey key, double& value) const
				{
					double dval = 0.0 ;
					if (JsonValue[key].isNumeric())
					{
						dval = JsonValue[key].asDouble();
					}
					else if (JsonValue[key].isString())
					{
						dval = atof(JsonValue[key].asCString());
					}
					else
					{
						//assert(false);
					}
					value = dval;
				}

				template<typename TKey>
				void Read(TKey key, std::string& value) const
				{
					std::string sval = "" ;
					if (JsonValue[key].isString())
					{
						sval = JsonValue[key].asString();
					}
					else
					{
						//assert(false);
					}
					value = sval;
				}

				template<typename TValue>
				void Read(std::vector<TValue>& vec) const
				{
					if(!JsonValue.isArray())return;
					vec.clear();
					vec.reserve(vec.size() + JsonValue.size());
					for(int i = 0; i < JsonValue.size(); ++i)
					{
						TValue val;
						DeSerialize(i, val);
						vec.push_back(val);
					}
				}
			}; //class autoSerialize
        }
	}
}
}
#endif //UTIL_JSON_AUTO_H
