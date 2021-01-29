/*******************************************************************************
 * Project:  neb
 * @file     CJsonObject.hpp
 * @brief    Json
 * @author   bwarliao
 * @date:    2014-7-16
 * @note
 * Modify history:
 ******************************************************************************/

#ifndef CJSONOBJECT_HPP_
#define CJSONOBJECT_HPP_

#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <map>
#include <list>
#ifdef __cplusplus
extern "C" {
#endif
#include "cJSON.h"
#ifdef __cplusplus
}
#endif
using std::map;
using std::list;
using std::string;
using std::pair;

namespace neb
{

class CJsonObject
{
public:     // method of ordinary json object or json array
    CJsonObject();
    CJsonObject(const string strJson);
    CJsonObject(const CJsonObject* pJsonObject);
    CJsonObject(const CJsonObject& oJsonObject);
    virtual ~CJsonObject();

    CJsonObject& operator=(const CJsonObject& oJsonObject);
    bool operator==(const CJsonObject& oJsonObject) const;
    bool Parse(const string& strJson);
    void Clear();
    bool IsEmpty() const;
    bool IsArray() const;
    string ToString() const;
    string ToFormattedString() const;
    const string& GetErrMsg() const
    {
        return(m_strErrMsg);
    }

public:     // method of ordinary json object
    bool AddEmptySubObject(const string strKey);
    bool AddEmptySubArray(const string strKey);
    bool GetKey(string& strKey);
    void ResetTraversing();
    CJsonObject& operator[](const string strKey);
    string operator()(const string strKey) const;
    bool Get(const string strKey, CJsonObject& oJsonObject) const;
    bool Get(const string strKey, string& strValue) const;
    bool Get(const string strKey, char& iValue) const;
    bool Get(const string strKey, unsigned char& uiValue) const;
    bool Get(const string strKey, short& iValue) const;
    bool Get(const string strKey, unsigned short& uiValue) const;
    bool Get(const string strKey, long& iValue) const;
    bool Get(const string strKey, unsigned long& uiValue) const;
    bool Get(const string strKey, int& iValue) const;
    bool Get(const string strKey, unsigned int& uiValue) const;
    bool Get(const string strKey, long long& llValue) const;
    bool Get(const string strKey, unsigned long long& ullValue) const;
    bool Get(const string strKey, bool& bValue) const;
    bool Get(const string strKey, float& fValue) const;
    bool Get(const string strKey, double& dValue) const;
    bool IsNull(const string strKey) const;
    bool Add(const string strKey, const CJsonObject& oJsonObject);
    bool Add(const string strKey, const char* pValue);
    bool Add(const string strKey, const string strValue);
    bool Add(const string strKey, char iValue);
    bool Add(const string strKey, unsigned char uiValue);
    bool Add(const string strKey, short iValue);
    bool Add(const string strKey, unsigned short uiValue);
    bool Add(const string strKey, long iValue);
    bool Add(const string strKey, unsigned long uiValue);
    bool Add(const string strKey, int32 iValue);
    bool Add(const string strKey, uint32 uiValue);
    bool Add(const string strKey, int64 llValue);
    bool Add(const string strKey, uint64 ullValue);
    bool Add(const string strKey, bool bValue);
    //bool Add(const string strKey, bool bValue, bool bValueAgain);
    bool Add(const string strKey, float fValue);
    bool Add(const string strKey, double dValue);
    bool AddNull(const string strKey);    // add null like this:   "key":null
    bool Delete(const string strKey);
    bool Replace(const string strKey, const CJsonObject& oJsonObject);
    bool Replace(const string strKey, const string& strValue);
    bool Replace(const string strKey, int32 iValue);
    bool Replace(const string strKey, uint32 uiValue);
    bool Replace(const string strKey, int64 llValue);
    bool Replace(const string strKey, uint64 ullValue);
    //bool Replace(const string strKey, bool bValue, bool bValueAgain);
    bool Replace(const string strKey, float fValue);
    bool Replace(const string strKey, double dValue);
    bool ReplaceWithNull(const string strKey);    // replace value with null

public:     // method of json array
    int GetArraySize();
    CJsonObject& operator[](unsigned int uiWhich);
    string operator()(unsigned int uiWhich) const;
    bool Get(int iWhich, char& iValue) const;
    bool Get(int iWhich, unsigned char& uiValue) const;
    bool Get(int iWhich, short& iValue) const;
    bool Get(int iWhich, unsigned short& uiValue) const;
    bool Get(int iWhich, long& iValue) const;
    bool Get(int iWhich, unsigned long& uiValue) const;
    bool Get(int iWhich, CJsonObject& oJsonObject) const;
    bool Get(int iWhich, string& strValue) const;
    bool Get(int iWhich, int32& iValue) const;
    bool Get(int iWhich, uint32& uiValue) const;
    bool Get(int iWhich, int64& llValue) const;
    bool Get(int iWhich, uint64& ullValue) const;
    bool Get(int iWhich, bool& bValue) const;
    bool Get(int iWhich, float& fValue) const;
    bool Get(int iWhich, double& dValue) const;
    bool IsNull(int iWhich) const;
    bool Add(const CJsonObject& oJsonObject);
    bool Add(const string strValue);
    bool Add(char iValue);
    bool Add(unsigned char uiValue);
    bool Add(short iValue);
    bool Add(unsigned short uiValue);
    bool Add(long iValue);
    bool Add( unsigned long uiValue);
    bool Add(int32 iValue);
    bool Add(uint32 uiValue);
    bool Add(int64 llValue);
    bool Add(uint64 ullValue);
    bool Add(bool bValue);
    bool Add(float fValue);
    bool Add(double dValue);
    bool AddNull();   // add a null value
    bool AddAsFirst(const CJsonObject& oJsonObject);
	bool AddAsFirst(const string strValue);
	bool AddAsFirst(char iValue);
	bool AddAsFirst(unsigned char uiValue);
	bool AddAsFirst(short iValue);
	bool AddAsFirst(unsigned short uiValue);
	bool AddAsFirst(long iValue);
	bool AddAsFirst(unsigned long uiValue);
    bool AddAsFirst(int32 iValue);
    bool AddAsFirst(uint32 uiValue);
    bool AddAsFirst(int64 llValue);
    bool AddAsFirst(uint64 ullValue);
    bool AddAsFirst(bool bValue);
    bool AddAsFirst(float fValue);
    bool AddAsFirst(double dValue);
    bool AddNullAsFirst();     // add a null value
    bool Delete(int iWhich);
    bool Replace(int iWhich, const CJsonObject& oJsonObject);
    bool Replace(int iWhich, const string& strValue);
    bool Replace(int iWhich, int32 iValue);
    bool Replace(int iWhich, uint32 uiValue);
    bool Replace(int iWhich, int64 llValue);
    bool Replace(int iWhich, uint64 ullValue);
    //bool Replace(int iWhich, bool bValue, bool bValueAgain);
    bool Replace(int iWhich, float fValue);
    bool Replace(int iWhich, double dValue);
    bool ReplaceWithNull(int iWhich);      // replace with a null value

private:
    CJsonObject(cJSON* pJsonData);

private:
    cJSON* m_pJsonData;
    cJSON* m_pExternJsonDataRef;
    string m_strErrMsg;
    map<unsigned int, CJsonObject*> m_mapJsonArrayRef;
    map<string, CJsonObject*> m_mapJsonObjectRef;
    list<string> m_listKeys;
    list<string>::const_iterator m_itKey;
};

}

#endif /* CJSONHELPER_HPP_ */
