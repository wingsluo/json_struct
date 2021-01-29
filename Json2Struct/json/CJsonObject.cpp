/*******************************************************************************
 * Project:  neb
 * @file     CJsonObject.cpp
 * @brief
 * @author   bwarliao
 * @date:    2014-7-16
 * @note
 * Modify history:
 ******************************************************************************/

#include "CJsonObject.hpp"
#include <string.h>


namespace neb
{

CJsonObject::CJsonObject()
    : m_pJsonData(nullptr), m_pExternJsonDataRef(nullptr)
{
    // m_pJsonData = cJSON_CreateObject();
}

CJsonObject::CJsonObject(const string strJson)
    : m_pJsonData(nullptr), m_pExternJsonDataRef(nullptr)
{
    Parse(strJson);
}

CJsonObject::CJsonObject(const CJsonObject* pJsonObject)
    : m_pJsonData(nullptr), m_pExternJsonDataRef(nullptr)
{
    if (pJsonObject)
    {
        Parse(pJsonObject->ToString());
    }
}

CJsonObject::CJsonObject(const CJsonObject& oJsonObject)
    : m_pJsonData(nullptr), m_pExternJsonDataRef(nullptr)
{
    Parse(oJsonObject.ToString());
}

CJsonObject::~CJsonObject()
{
    Clear();
}

CJsonObject& CJsonObject::operator=(const CJsonObject& oJsonObject)
{
    Parse(oJsonObject.ToString().c_str());
    return(*this);
}

bool CJsonObject::operator==(const CJsonObject& oJsonObject) const
{
    return(this->ToString() == oJsonObject.ToString());
}

bool CJsonObject::AddEmptySubObject(const string strKey)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateObject();
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("create sub empty object error!");
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::AddEmptySubArray(const string strKey)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateArray();
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("create sub empty array error!");
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::GetKey(string& strKey)
{
    if (IsArray())
    {
        return(false);
    }
    if (m_listKeys.size() == 0)
    {
        cJSON* pFocusData = nullptr;
        if (m_pJsonData != nullptr)
        {
            pFocusData = m_pJsonData;
        }
        else if (m_pExternJsonDataRef != nullptr)
        {
            pFocusData = m_pExternJsonDataRef;
        }
        else
        {
            return(false);
        }

        cJSON *c = pFocusData->child;
        while (c)
        {
            m_listKeys.push_back(c->string);
            c = c->next;
        }
        m_itKey = m_listKeys.begin();
    }

    if (m_itKey == m_listKeys.end())
    {
        strKey = "";
        m_itKey = m_listKeys.begin();
        return(false);
    }
    else
    {
        strKey = *m_itKey;
        ++m_itKey;
        return(true);
    }
}

void CJsonObject::ResetTraversing()
{
    m_itKey = m_listKeys.begin();
}

CJsonObject& CJsonObject::operator[](const string strKey)
{
    map<string, CJsonObject*>::iterator iter;
    iter = m_mapJsonObjectRef.find(strKey);
    if (iter == m_mapJsonObjectRef.end())
    {
        cJSON* pJsonStruct = nullptr;
        if (m_pJsonData != nullptr)
        {
            if (m_pJsonData->type == cJSON_Object)
            {
                pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
            }
        }
        else if (m_pExternJsonDataRef != nullptr)
        {
            if (m_pExternJsonDataRef->type == cJSON_Object)
            {
                pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
            }
        }
        if (pJsonStruct == nullptr)
        {
            CJsonObject* pJsonObject = new CJsonObject();
            m_mapJsonObjectRef.insert(pair<string, CJsonObject*>(strKey, pJsonObject));
            return(*pJsonObject);
        }
        else
        {
            CJsonObject* pJsonObject = new CJsonObject(pJsonStruct);
            m_mapJsonObjectRef.insert(pair<string, CJsonObject*>(strKey, pJsonObject));
            return(*pJsonObject);
        }
    }
    else
    {
        return(*(iter->second));
    }
}

CJsonObject& CJsonObject::operator[](unsigned int uiWhich)
{
    map<unsigned int, CJsonObject*>::iterator iter;
    iter = m_mapJsonArrayRef.find(uiWhich);
    if (iter == m_mapJsonArrayRef.end())
    {
        cJSON* pJsonStruct = nullptr;
        if (m_pJsonData != nullptr)
        {
            if (m_pJsonData->type == cJSON_Array)
            {
                pJsonStruct = cJSON_GetArrayItem(m_pJsonData, uiWhich);
            }
        }
        else if (m_pExternJsonDataRef != nullptr)
        {
            if (m_pExternJsonDataRef->type == cJSON_Array)
            {
                pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, uiWhich);
            }
        }
        if (pJsonStruct == nullptr)
        {
            CJsonObject* pJsonObject = new CJsonObject();
            m_mapJsonArrayRef.insert(pair<unsigned int, CJsonObject*>(uiWhich, pJsonObject));
            return(*pJsonObject);
        }
        else
        {
            CJsonObject* pJsonObject = new CJsonObject(pJsonStruct);
            m_mapJsonArrayRef.insert(pair<unsigned int, CJsonObject*>(uiWhich, pJsonObject));
            return(*pJsonObject);
        }
    }
    else
    {
        return(*(iter->second));
    }
}

string CJsonObject::operator()(const string strKey) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(string(""));
    }
    if (pJsonStruct->type == cJSON_String)
    {
        return(pJsonStruct->valuestring);
    }
    else if (pJsonStruct->type == cJSON_Int)
    {
        char szNumber[128] = {0};
        if (pJsonStruct->sign == -1)
        {
            if ((int64)pJsonStruct->valueint <= (int64)INT_MAX && (int64)pJsonStruct->valueint >= (int64)INT_MIN)
            {
                snprintf(szNumber, sizeof(szNumber), "%d", (int32)pJsonStruct->valueint);
            }
            else
            {
                snprintf(szNumber, sizeof(szNumber), "%lld", (int64)pJsonStruct->valueint);
            }
        }
        else
        {
            if (pJsonStruct->valueint <= (uint64)UINT_MAX)
            {
                snprintf(szNumber, sizeof(szNumber), "%u", (uint32)pJsonStruct->valueint);
            }
            else
            {
                snprintf(szNumber, sizeof(szNumber), "%llu", pJsonStruct->valueint);
            }
        }
        return(string(szNumber));
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        char szNumber[128] = {0};
        if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9)
        {
            snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
        }
        else
        {
            snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
        }
    }
    else if (pJsonStruct->type == cJSON_False)
    {
        return(string("false"));
    }
    else if (pJsonStruct->type == cJSON_True)
    {
        return(string("true"));
    }
    return(string(""));
}

string CJsonObject::operator()(unsigned int uiWhich) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, uiWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, uiWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(string(""));
    }
    if (pJsonStruct->type == cJSON_String)
    {
        return(pJsonStruct->valuestring);
    }
    else if (pJsonStruct->type == cJSON_Int)
    {
        char szNumber[128] = {0};
        if (pJsonStruct->sign == -1)
        {
            if ((int64)pJsonStruct->valueint <= (int64)INT_MAX && (int64)pJsonStruct->valueint >= (int64)INT_MIN)
            {
                snprintf(szNumber, sizeof(szNumber), "%d", (int32)pJsonStruct->valueint);
            }
            else
            {
                snprintf(szNumber, sizeof(szNumber), "%lld", (int64)pJsonStruct->valueint);
            }
        }
        else
        {
            if (pJsonStruct->valueint <= (uint64)UINT_MAX)
            {
                snprintf(szNumber, sizeof(szNumber), "%u", (uint32)pJsonStruct->valueint);
            }
            else
            {
                snprintf(szNumber, sizeof(szNumber), "%llu", pJsonStruct->valueint);
            }
        }
        return(string(szNumber));
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        char szNumber[128] = {0};
        if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9)
        {
            snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
        }
        else
        {
            snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
        }
    }
    else if (pJsonStruct->type == cJSON_False)
    {
        return(string("false"));
    }
    else if (pJsonStruct->type == cJSON_True)
    {
        return(string("true"));
    }
    return(string(""));
}

bool CJsonObject::Parse(const string& strJson)
{
    Clear();
    m_pJsonData = cJSON_Parse(strJson.c_str());
    if (m_pJsonData == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
    }
    return(true);
}

void CJsonObject::Clear()
{
    m_pExternJsonDataRef = nullptr;
    if (m_pJsonData != nullptr)
    {
        cJSON_Delete(m_pJsonData);
        m_pJsonData = nullptr;
    }
    for (map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.begin();
                    iter != m_mapJsonArrayRef.end(); ++iter)
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
    }
    m_mapJsonArrayRef.clear();
    for (map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.begin();
                    iter != m_mapJsonObjectRef.end(); ++iter)
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
    }
    m_mapJsonObjectRef.clear();
    m_listKeys.clear();
}

bool CJsonObject::IsEmpty() const
{
    if (m_pJsonData != nullptr)
    {
        return(false);
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::IsArray() const
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }

    if (pFocusData == nullptr)
    {
        return(false);
    }

    if (pFocusData->type == cJSON_Array)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

string CJsonObject::ToString() const
{
    char* pJsonString = nullptr;
    string strJsonData = "";
    if (m_pJsonData != nullptr)
    {
        pJsonString = cJSON_PrintUnformatted(m_pJsonData);
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pJsonString = cJSON_PrintUnformatted(m_pExternJsonDataRef);
    }
    if (pJsonString != nullptr)
    {
        strJsonData = pJsonString;
        free(pJsonString);
    }
    return(strJsonData);
}

string CJsonObject::ToFormattedString() const
{
    char* pJsonString = nullptr;
    string strJsonData = "";
    if (m_pJsonData != nullptr)
    {
        pJsonString = cJSON_Print(m_pJsonData);
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pJsonString = cJSON_Print(m_pExternJsonDataRef);
    }
    if (pJsonString != nullptr)
    {
        strJsonData = pJsonString;
        free(pJsonString);
    }
    return(strJsonData);
}


bool CJsonObject::Get(const string strKey, CJsonObject& oJsonObject) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    char* pJsonString = cJSON_Print(pJsonStruct);
    string strJsonData = pJsonString;
    free(pJsonString);
    if (oJsonObject.Parse(strJsonData))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

bool CJsonObject::Get(const string strKey, string& strValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type != cJSON_String)
    {
        return(false);
    }
    strValue = pJsonStruct->valuestring;
    return(true);
}


bool CJsonObject::Get(const string strKey, char& iValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        iValue = (char)ullValue;
        return true;
    }
    return false;
}
bool CJsonObject::Get(const string strKey, unsigned char& uiValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        uiValue = (unsigned char)ullValue;
        return true;
    }
    return false;
}


bool CJsonObject::Get(const string strKey, short& iValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        iValue = (short)ullValue;
        return true;
    }
    return false;
}
bool CJsonObject::Get(const string strKey, unsigned short& uiValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        uiValue = (unsigned short)ullValue;
        return true;
    }
    return false;
}
bool CJsonObject::Get(const string strKey, int& iValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        iValue = (int)ullValue;
        return true;
    }
    return false;
}

bool CJsonObject::Get(const string strKey, unsigned int& uiValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        uiValue = (unsigned int)ullValue;
        return true;
    }
    return false;
}


bool CJsonObject::Get(const string strKey, long& iValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        iValue = (long)ullValue;
        return true;
    }
    return false;
}
bool CJsonObject::Get(const string strKey, unsigned long& uiValue) const
{
    unsigned long long ullValue;
    if (Get(strKey, ullValue))
    {
        uiValue = (unsigned long)ullValue;
        return true;
    }
    return false;
}

bool CJsonObject::Get(const string strKey, long long& llValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        llValue = (int64)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        llValue = (int64)(pJsonStruct->valuedouble);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_String)
    {
        //for test
#if defined(WIN32) || defined(_WIN64)
        llValue = _atoi64(pJsonStruct->valuestring);
#else
        llValue = strtol(pJsonStruct->valuestring, nullptr, 10);
#endif
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(const string strKey, unsigned long long& ullValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        ullValue = (uint64)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        ullValue = (uint64)(pJsonStruct->valuedouble);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_String)
    {
        //for test
#if defined(WIN32) || defined(_WIN64)
        ullValue = _atoi64(pJsonStruct->valuestring);
#else
        ullValue = strtol(pJsonStruct->valuestring, nullptr, 10);
#endif
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(const string strKey, bool& bValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_True || pJsonStruct->type == cJSON_False || pJsonStruct->type == cJSON_Int)
    {
        bValue = pJsonStruct->valueint != 0 ;
        return true;
    }
    else if (pJsonStruct->type == cJSON_String)
    {
        if (!strcmp(pJsonStruct->valuestring, "true") || !strcmp(pJsonStruct->valuestring, "TRUE"))
        {
            bValue = true;
            return true;
        }
        else if (!strcmp(pJsonStruct->valuestring, "true") || !strcmp(pJsonStruct->valuestring, "TRUE"))
        {
            bValue = false;
            return true;
        }
    }
    return(false);
}

bool CJsonObject::Get(const string strKey, float& fValue) const
{
    double dDouble;
    if (Get(strKey, dDouble))
    {
        fValue = (float)dDouble;
        return true;
    }
    return false;
}

bool CJsonObject::Get(const string strKey, double& dValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Double)
    {
        dValue = pJsonStruct->valuedouble;
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Int)
    {
        dValue = (double)(pJsonStruct->valueint);
        return(true);
    }
    return(false);
}

bool CJsonObject::IsNull(const string strKey) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Object)
        {
            pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type != cJSON_NULL)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Add(const string strKey, const CJsonObject& oJsonObject)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, const char* pValue)
{
    if (!pValue) return false;
    string strValue = pValue;
    return Add(strKey, strValue);
}

bool CJsonObject::Add(const string strKey, const string strValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateString(strValue.c_str());
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, int32 iValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)iValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, uint32 uiValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)uiValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, char iValue)
{
    int64 llValue = (int64)iValue;
    return Add(strKey, llValue);
}
bool CJsonObject::Add(const string strKey, unsigned char uiValue)
{
    uint64 ullValue = (uint64)uiValue;
    return Add(strKey, ullValue);
}
bool CJsonObject::Add(const string strKey, short iValue)
{
    int64 llValue = (int64)iValue;
    return Add(strKey, llValue);
}
bool CJsonObject::Add(const string strKey, unsigned short uiValue)
{
    uint64 ullValue = (uint64)uiValue;
    return Add(strKey, ullValue);
}
bool CJsonObject::Add(const string strKey, long iValue)
{
    int64 llValue = (int64)iValue;
    return Add(strKey, llValue);
}
bool CJsonObject::Add(const string strKey, unsigned long uiValue)
{
    uint64 ullValue = (uint64)uiValue;
    return Add(strKey, ullValue);
}
bool CJsonObject::Add(const string strKey, int64 llValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)llValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, uint64 ullValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt(ullValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}
bool CJsonObject::Add(const string strKey, bool bValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

//bool CJsonObject::Add(const string strKey, bool bValue, bool bValueAgain)
//{
//    cJSON* pFocusData = nullptr;
//    if (m_pJsonData != nullptr)
//    {
//        pFocusData = m_pJsonData;
//    }
//    else if (m_pExternJsonDataRef != nullptr)
//    {
//        pFocusData = m_pExternJsonDataRef;
//    }
//    else
//    {
//        m_pJsonData = cJSON_CreateObject();
//        pFocusData = m_pJsonData;
//    }

//    if (pFocusData == nullptr)
//    {
//        m_strErrMsg = "json data is null!";
//        return(false);
//    }
//    if (pFocusData->type != cJSON_Object)
//    {
//        m_strErrMsg = "not a json object! json array?";
//        return(false);
//    }
//    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
//    if (pJsonStruct == nullptr)
//    {
//        return(false);
//    }
//    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
//    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
//    {
//        return(false);
//    }
//    m_listKeys.clear();
//    return(true);
//}

bool CJsonObject::Add(const string strKey, float fValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)fValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Add(const string strKey, double dValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)dValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::AddNull(const string strKey)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateNull();
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    cJSON_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Delete(const string strKey)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON_DeleteItemFromObject(pFocusData, strKey.c_str());
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    m_listKeys.clear();
    return(true);
}

bool CJsonObject::Replace(const string strKey, const CJsonObject& oJsonObject)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, const string& strValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateString(strValue.c_str());
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, int32 iValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)iValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, uint32 uiValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)uiValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, int64 llValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)llValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, uint64 ullValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)ullValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

//bool CJsonObject::Replace(const string strKey, bool bValue, bool bValueAgain)
//{
//    cJSON* pFocusData = nullptr;
//    if (m_pJsonData == nullptr)
//    {
//        pFocusData = m_pExternJsonDataRef;
//    }
//    else
//    {
//        pFocusData = m_pJsonData;
//    }
//    if (pFocusData == nullptr)
//    {
//        m_strErrMsg = "json data is null!";
//        return(false);
//    }
//    if (pFocusData->type != cJSON_Object)
//    {
//        m_strErrMsg = "not a json object! json array?";
//        return(false);
//    }
//    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
//    if (pJsonStruct == nullptr)
//    {
//        return(false);
//    }
//    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
//    if (iter != m_mapJsonObjectRef.end())
//    {
//        if (iter->second != nullptr)
//        {
//            delete (iter->second);
//            iter->second = nullptr;
//        }
//        m_mapJsonObjectRef.erase(iter);
//    }
//    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
//    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
//    {
//        return(false);
//    }
//    return(true);
//}

bool CJsonObject::Replace(const string strKey, float fValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)fValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(const string strKey, double dValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)dValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::ReplaceWithNull(const string strKey)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Object)
    {
        m_strErrMsg = "not a json object! json array?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateNull();
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonObjectRef.erase(iter);
    }
    cJSON_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (cJSON_GetObjectItem(pFocusData, strKey.c_str()) == nullptr)
    {
        return(false);
    }
    return(true);
}

int CJsonObject::GetArraySize()
{
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            return(cJSON_GetArraySize(m_pJsonData));
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            return(cJSON_GetArraySize(m_pExternJsonDataRef));
        }
    }
    return(0);
}

bool CJsonObject::Get(int iWhich, char& iValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        iValue = (char)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        iValue = (char)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, unsigned char& uiValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        uiValue = (unsigned char)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        uiValue = (unsigned char)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, short& iValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        iValue = (char)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        iValue = (char)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, unsigned short& uiValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        uiValue = (unsigned short)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        uiValue = (unsigned short)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, long& iValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        iValue = (long)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        iValue = (long)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, unsigned long& uiValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if (m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        uiValue = (unsigned long)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        uiValue = (unsigned long)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}
bool CJsonObject::Get(int iWhich, CJsonObject& oJsonObject) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    char* pJsonString = cJSON_Print(pJsonStruct);
    string strJsonData = pJsonString;
    free(pJsonString);
    if (oJsonObject.Parse(strJsonData))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

bool CJsonObject::Get(int iWhich, string& strValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type != cJSON_String)
    {
        return(false);
    }
    strValue = pJsonStruct->valuestring;
    return(true);
}

bool CJsonObject::Get(int iWhich, int32& iValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        iValue = (int32)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        iValue = (int32)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(int iWhich, uint32& uiValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        uiValue = (uint32)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        uiValue = (uint32)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(int iWhich, int64& llValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        llValue = (int64)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        llValue = (int64)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(int iWhich, uint64& ullValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Int)
    {
        ullValue = (uint64)(pJsonStruct->valueint);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Double)
    {
        ullValue = (uint64)(pJsonStruct->valuedouble);
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(int iWhich, bool& bValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type > cJSON_True)
    {
        return(false);
    }
    bValue = pJsonStruct->type == 0 ? false : true;
    return(true);
}

bool CJsonObject::Get(int iWhich, float& fValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Double)
    {
        fValue = (float)(pJsonStruct->valuedouble);
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Int)
    {
        fValue = (float)(pJsonStruct->valueint);
        return(true);
    }
    return(false);
}

bool CJsonObject::Get(int iWhich, double& dValue) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type == cJSON_Double)
    {
        dValue = pJsonStruct->valuedouble;
        return(true);
    }
    else if (pJsonStruct->type == cJSON_Int)
    {
        dValue = (double)(pJsonStruct->valueint);
        return(true);
    }
    return(false);
}

bool CJsonObject::IsNull(int iWhich) const
{
    cJSON* pJsonStruct = nullptr;
    if (m_pJsonData != nullptr)
    {
        if (m_pJsonData->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
        }
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        if(m_pExternJsonDataRef->type == cJSON_Array)
        {
            pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    if (pJsonStruct->type != cJSON_NULL)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Add(const CJsonObject& oJsonObject)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
	}
	//todo
	cJSON_AddItemToArray(pFocusData, pJsonStruct);
	//cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
	//int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
	//cJSON_AddItemToArray(pFocusData, pJsonStruct);
	//int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
	//if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
	//{
	//	return(false);
	//}
    //unsigned int uiLastIndex = (unsigned int)cJSON_GetArraySize(pFocusData) - 1;
    //for (map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.begin();
    //                iter != m_mapJsonArrayRef.end(); )
    //{
    //    if (iter->first >= uiLastIndex)
    //    {
    //        if (iter->second != nullptr)
    //        {
    //            delete (iter->second);
    //            iter->second = nullptr;
    //        }
    //        m_mapJsonArrayRef.erase(iter++);
    //    }
    //    else
    //    {
    //        iter++;
    //    }
    //}
    return(true);
}

bool CJsonObject::Add(const string strValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateString(strValue.c_str());
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}
bool CJsonObject::Add(char iValue)
{
    int64 llValue = (int64)iValue;
    return Add(llValue);
}
bool CJsonObject::Add(unsigned char uiValue)
{
    uint64 ullValue = (int64)uiValue;
    return Add(ullValue);
}
bool CJsonObject::Add(short iValue)
{
    int64 llValue = (int64)iValue;
    return Add(llValue);
}
bool CJsonObject::Add(unsigned short uiValue)
{
    uint64 ullValue = (int64)uiValue;
    return Add(ullValue);
}
bool CJsonObject::Add(long iValue)
{
    int64 llValue = (int64)iValue;
    return Add(llValue);
}
bool CJsonObject::Add(unsigned long uiValue)
{
    uint64 ullValue = (int64)uiValue;
    return Add(ullValue);
}

bool CJsonObject::Add(int32 iValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)iValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(uint32 uiValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)uiValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(int64 llValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)llValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(uint64 ullValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)ullValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(bool bValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(float fValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)fValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Add(double dValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)dValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddNull()
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateNull();
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArray(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(const CJsonObject& oJsonObject)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    //for (map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.begin();
    //                iter != m_mapJsonArrayRef.end(); )
    //{
    //    if (iter->second != nullptr)
    //    {
    //        delete (iter->second);
    //        iter->second = nullptr;
    //    }
    //    m_mapJsonArrayRef.erase(iter++);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(const string strValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateString(strValue.c_str());
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(char iValue)
{
    int64 llValue = (int64)iValue;
    return AddAsFirst(llValue);
}
bool CJsonObject::AddAsFirst(unsigned char uiValue)
{
	int64 ullValue = (int64)uiValue;
	return AddAsFirst(ullValue);
}
bool CJsonObject::AddAsFirst(short iValue)
{
	int64 llValue = (int64)iValue;
	return AddAsFirst(llValue);
}
bool CJsonObject::AddAsFirst(unsigned short uiValue)
{
	int64 ullValue = (int64)uiValue;
	return AddAsFirst(ullValue);
}
bool CJsonObject::AddAsFirst(long iValue)
{
	int64 llValue = (int64)iValue;
	return AddAsFirst(llValue);
}
bool CJsonObject::AddAsFirst(unsigned long uiValue)
{
	int64 ullValue = (int64)uiValue;
	return AddAsFirst(ullValue);
}

bool CJsonObject::AddAsFirst(int32 iValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)iValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(uint32 uiValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)uiValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(int64 llValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)llValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(uint64 ullValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)ullValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(bool bValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(float fValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)fValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddAsFirst(double dValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)dValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::AddNullAsFirst()
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData != nullptr)
    {
        pFocusData = m_pJsonData;
    }
    else if (m_pExternJsonDataRef != nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        m_pJsonData = cJSON_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateNull();
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    //int iArraySizeBeforeAdd = cJSON_GetArraySize(pFocusData);
    cJSON_AddItemToArrayHead(pFocusData, pJsonStruct);
    //int iArraySizeAfterAdd = cJSON_GetArraySize(pFocusData);
    //if (iArraySizeAfterAdd == iArraySizeBeforeAdd)
    //{
    //    return(false);
    //}
    return(true);
}

bool CJsonObject::Delete(int iWhich)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON_DeleteItemFromArray(pFocusData, iWhich);
    for (map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.begin();
                    iter != m_mapJsonArrayRef.end(); )
    {
        if (iter->first >= (unsigned int)iWhich)
        {
            if (iter->second != nullptr)
            {
                delete (iter->second);
                iter->second = nullptr;
            }
            m_mapJsonArrayRef.erase(iter++);
        }
        else
        {
            iter++;
        }
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, const CJsonObject& oJsonObject)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == nullptr)
    {
        m_strErrMsg = string("prase json string error at ") + cJSON_GetErrorPtr();
        return(false);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, const string& strValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateString(strValue.c_str());
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, int32 iValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)iValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, uint32 uiValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)uiValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, int64 llValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)((uint64)llValue), -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, uint64 ullValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateInt((uint64)ullValue, 1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

//bool CJsonObject::Replace(int iWhich, bool bValue, bool bValueAgain)
//{
//    cJSON* pFocusData = nullptr;
//    if (m_pJsonData == nullptr)
//    {
//        pFocusData = m_pExternJsonDataRef;
//    }
//    else
//    {
//        pFocusData = m_pJsonData;
//    }
//    if (pFocusData == nullptr)
//    {
//        m_strErrMsg = "json data is null!";
//        return(false);
//    }
//    if (pFocusData->type != cJSON_Array)
//    {
//        m_strErrMsg = "not a json array! json object?";
//        return(false);
//    }
//    cJSON* pJsonStruct = cJSON_CreateBool(bValue);
//    if (pJsonStruct == nullptr)
//    {
//        return(false);
//    }
//    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
//    if (iter != m_mapJsonArrayRef.end())
//    {
//        if (iter->second != nullptr)
//        {
//            delete (iter->second);
//            iter->second = nullptr;
//        }
//        m_mapJsonArrayRef.erase(iter);
//    }
//    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
//    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
//    {
//        return(false);
//    }
//    return(true);
//}

bool CJsonObject::Replace(int iWhich, float fValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)fValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::Replace(int iWhich, double dValue)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateDouble((double)dValue, -1);
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

bool CJsonObject::ReplaceWithNull(int iWhich)
{
    cJSON* pFocusData = nullptr;
    if (m_pJsonData == nullptr)
    {
        pFocusData = m_pExternJsonDataRef;
    }
    else
    {
        pFocusData = m_pJsonData;
    }
    if (pFocusData == nullptr)
    {
        m_strErrMsg = "json data is null!";
        return(false);
    }
    if (pFocusData->type != cJSON_Array)
    {
        m_strErrMsg = "not a json array! json object?";
        return(false);
    }
    cJSON* pJsonStruct = cJSON_CreateNull();
    if (pJsonStruct == nullptr)
    {
        return(false);
    }
    map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end())
    {
        if (iter->second != nullptr)
        {
            delete (iter->second);
            iter->second = nullptr;
        }
        m_mapJsonArrayRef.erase(iter);
    }
    cJSON_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (cJSON_GetArrayItem(pFocusData, iWhich) == nullptr)
    {
        return(false);
    }
    return(true);
}

CJsonObject::CJsonObject(cJSON* pJsonData)
    : m_pJsonData(nullptr), m_pExternJsonDataRef(pJsonData)
{
}

}


