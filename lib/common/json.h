#pragma once

#include "iostream"
#include "boost/optional.hpp"

#include <folly/json.h>

using namespace std;
using namespace folly;

namespace rtbxegen {
    class Serializable
    {
    public:
        Serializable(){}
        virtual ~Serializable(){}

        virtual folly::dynamic serialize() const = 0;
        virtual bool deserialize(const folly::dynamic& ) = 0;
    };

    namespace json {
        //T (std::string + arithmetic + Serializable)
        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value ||
                        std::is_base_of<rtbxegen::Serializable, T>::value>::type* = nullptr >
        bool deserialize(std::string fieldName, T& structField, const folly::dynamic& jsonObj) {
            if(auto* jsonField = jsonObj.get_ptr(fieldName)){
                if(std::is_same<std::string, T>::value) {
                    string tmp = jsonField->asString().toStdString();
                    structField = *(static_cast<T*>(static_cast<void*>(&tmp)));
                } else
                if(std::is_same<int, T>::value){
                    int tmp = jsonField->asInt();
                    structField = *(static_cast<T*>(static_cast<void*>(&tmp)));
                } else if(std::is_same<double, T>::value){
                    double tmp = jsonField->asDouble();
                    structField = *(static_cast<T*>(static_cast<void*>(&tmp)));
                } else if(std::is_same<bool, T>::value){
                    bool tmp = jsonField->asBool();
                    structField = *(static_cast<T*>(static_cast<void*>(&tmp)));
                }
                else if(std::is_base_of< Serializable, T >::value){
                    Serializable* tmp = static_cast<Serializable*>(static_cast<void*>(&structField));
                    return tmp->deserialize(*jsonField);
                }
                else {
                    //do nothing for now
                }
            }

            return true;
        };

        //vector<T>
        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value ||
                        std::is_base_of<rtbxegen::Serializable, T>::value>::type* = nullptr >
        bool deserialize(std::string fieldName, vector<T>& structField, const folly::dynamic& jsonObj) {
            structField.clear();

            if(auto* jsonField = jsonObj.get_ptr(fieldName)){
                if(jsonField->isArray()) {
                    for(auto& elem : *jsonField) {
                        //TODO: check to enable in the case T is derived class of Serializable instead of primitives value
                        //If possible; don't use push_back but reset vector to the one with specific size
                        if(std::is_same<int, T>::value) {
                            int tmp = elem.getInt();
                            structField.push_back(*(static_cast<T*>(static_cast<void*>(&tmp))));
                        } else if(std::is_same<std::string, T>::value){
                            std::string tmp = elem.getString().toStdString();
                            structField.push_back(*(static_cast<T*>(static_cast<void*>(&tmp))));
                        } else if(std::is_same<double, T>::value) {
                            double tmp = elem.getDouble();
                            structField.push_back(*(static_cast<T*>(static_cast<void*>(&tmp))));
                        } else if(std::is_same<bool, T>::value) {
                            bool tmp = elem.getBool();
                            structField.push_back(*(static_cast<T*>(static_cast<void*>(&tmp))));
                        } else if(std::is_base_of< Serializable, T >::value) {
                            T tmp;
                            static_cast<Serializable*>(static_cast<void*>(&tmp))->deserialize(elem);
                            structField.push_back(tmp);
                        } else return false;
                    }
                } else return false;
            }

            return true;
        };


        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr >
        bool deserialize(std::string fieldName, boost::optional<T>& structField, const folly::dynamic& jsonObj) {
            if(jsonObj.get_ptr(fieldName)) {
                if(!structField) structField = T();
                return deserialize(fieldName, structField.get(), jsonObj);
            }

            return true;
        };

        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr >
        bool deserialize(std::string fieldName, boost::optional<vector<T>>& structField, const folly::dynamic& jsonObj) {
            if(jsonObj.get_ptr(fieldName)) {
                if(!structField) structField = vector<T>();
                return deserialize(fieldName, structField.get(), jsonObj);
            }

            return true;
        };

        //SERIALIZE
        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value ||
                        std::is_base_of<rtbxegen::Serializable, T>::value>::type* = nullptr >
        void serialize(std::string fieldName, const T& structField, folly::dynamic& out) {
            bool assign = true;
            if(std::is_same<std::string, T>::value) {
                const std::string* ptr = static_cast<const std::string*>(static_cast<const void*>(&structField));
                if(ptr->empty()){
                    assign = false;
                }
            }

            //if T is derived class of Serializable
            if(std::is_base_of<rtbxegen::Serializable, T>::value) {
                const Serializable* tmp = static_cast<const Serializable*>(static_cast<const void*>(&structField));
                out[fieldName] = tmp->serialize();
            } else {
                if(assign) {
                    if(std::is_same<std::string, T>::value){
                        out[fieldName] = *(static_cast<const std::string*>(static_cast<const void*>(&structField)));
                    } else if(std::is_same<int, T>::value){
                        out[fieldName] = *(static_cast<const int*>(static_cast<const void*>(&structField)));
                    } else if(std::is_same<double, T>::value){
                        out[fieldName] = *(static_cast<const double*>(static_cast<const void*>(&structField)));
                    } else if(std::is_same<bool, T>::value){
                        out[fieldName] = *(static_cast<const bool*>(static_cast<const void*>(&structField)));
                    } else {
                        //do nothing for now
                        return;
                    }
                }
            }
        };

        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value ||
                        std::is_base_of<rtbxegen::Serializable, T>::value>::type* = nullptr >
        void serialize(std::string fieldName, const vector<T>& structField, folly::dynamic& out) {
            dynamic array = {};

            //if T is derived class of Serializable
            if(std::is_base_of<rtbxegen::Serializable, T>::value) {
                for(auto& elem : structField) {
                    const Serializable* tmp = static_cast<const Serializable*>(static_cast<const void*>(&elem));
                    array.push_back(tmp->serialize());
                }
            } else if(std::is_same<std::string, T>::value){
                for(auto& elem : structField) {
                    array.push_back(*(static_cast<const std::string*>(static_cast<const void*>(&elem))));
                }
            } else if(std::is_same<int, T>::value){
                for(auto& elem : structField) {
                    array.push_back(*(static_cast<const int*>(static_cast<const void*>(&elem))));
                }
            } else if(std::is_same<double, T>::value){
                for(auto& elem : structField) {
                    array.push_back(*(static_cast<const double*>(static_cast<const void*>(&elem))));
                }
            } else if(std::is_same<bool, T>::value){
                for(auto& elem : structField) {
                    array.push_back(*(static_cast<const bool*>(static_cast<const void*>(&elem))));
                }
            } else {
                //do nothing for now
                return;
            }

            out[fieldName] = array;
        };

        //boost::optional
        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr >
        void serialize(std::string fieldName, const boost::optional<T>& structField, folly::dynamic& out) {
            //only serialize when field is available
            if(structField){
                serialize(fieldName, structField.get(), out);
            }
        }

        template<typename T,
                typename std::enable_if<
                        std::is_arithmetic<T>::value || std::is_same<std::string, T>::value>::type* = nullptr >
        void serialize(std::string fieldName, const boost::optional<vector<T>>& structField, folly::dynamic& out) {
            if(structField) {
                serialize(fieldName, structField.get(), out);
            }
        };
    }
}