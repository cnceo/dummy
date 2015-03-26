#ifndef _prop_decl_h_
#define _prop_decl_h_

#include <string>
#include <vector>

typedef _Longlong int64_t;

using namespace std;

enum enum_raw_field_type
{
    fieldtype_none   = 0, 
    fieldtype_bool   = 1, // bool����
    fieldtype_string = 2, // �ַ���
    fieldtype_byte   = 3, // ���ֽ�����
    fieldtype_word   = 4, // ˫�ֽ�����
    fieldtype_dword  = 5, // ���ֽ�����
    fieldtype_qword  = 6, // ���ֽ�����
    fieldtype_float  = 7, // ������
    fieldtype_double = 8, // ˫���ȸ�����
    fieldtype_max,
};

// ��Ա����
struct field_t
{
    field_t()
        : fieldtype(fieldtype_none)
        , maxlen(0)
        , minval(0)
        , maxval(0)
        , is_unique(false)
    {
    }

    string& get_field_type();

    enum_raw_field_type fieldtype; // ��������
    string fieldname;         // �ֶ�����
    string comment;           // ע��
    int    maxlen;               // ��󳤶ȣ���������Ϊ�ַ���ʱ��Ч

    int64_t minval;    // ��Сֵ��������ַ�����Ϊ��С����
    int64_t maxval;    // ���ֵ��������ַ�����Ϊ��󳤶�
    bool    is_unique; // ÿ����¼�еı��ֶ�ֵ�Ƿ���붼��һ��
};

typedef std::vector<field_t> fieldvec_t;
typedef std::vector<int> keyvec_t;

// �������ݵ�����
struct propdecl_t
{
    propdecl_t()
        : mincnt(0)
        , maxcnt(0)
    {
    }

    string filename;   // ���ýṹ�����ļ�������
    string propname;   // ���ýṹ�������  

    fieldvec_t fields; // ����Ա
    keyvec_t keys;     // ���������б���ĳ���ֶ�����������������������뵽����б���

    size_t mincnt; // �����м�������
    size_t maxcnt; // ����м�������

    bool exist(const char* fieldname)
    {
        return (NULL != getfield(fieldname));
    }

    // �����ֶ�����ȡ�ֶ�
    field_t* getfield(const char* fieldname)
    {
        for(fieldvec_t::iterator itr = fields.begin(); itr != fields.end(); ++itr)
        {
            field_t &field = *itr;
            if(field.fieldname == fieldname)
            {
                return &field;
            }
        }

        return NULL;
    }

    int getfieldidx(const char* fieldname)
    {
        int ret = -1;

        int i = 0;
        for(fieldvec_t::iterator itr = fields.begin(); itr != fields.end(); ++itr, ++i)
        {
            field_t &field = *itr;
            if(field.fieldname == fieldname)
            {
                ret = i;
                break;
            }
        }

        return ret;
    }
};

typedef std::vector<propdecl_t> propdeclvec_t;
struct propunit_t
{
    propdecl_t* find_by_file_name(const char* filename)
    {
        if(NULL == filename)
        {
            return NULL;
        }

        for(propdeclvec_t::iterator itr = propdecls.begin(); itr != propdecls.end(); ++itr)
        {
            propdecl_t &propdecl = *itr;
            if(propdecl.filename == filename)
            {
                return &propdecl;
            }
        }

        return NULL;
    }

    propdecl_t* find_by_prop_name(const char* propname)
    {
        if(NULL == propname)
        {
            return NULL;
        }

        for(propdeclvec_t::iterator itr = propdecls.begin(); itr != propdecls.end(); ++itr)
        {
            propdecl_t &propdecl = *itr;
            if(propdecl.propname == propname)
            {
                return &propdecl;
            }
        }

        return NULL;
    }

    string filename;
    propdeclvec_t propdecls;
};

typedef std::vector<string> errvec_t;
namespace proputil
{
    string& raw_type_2_c_type(enum_raw_field_type);

    string& raw_type_2_xsd_type(enum_raw_field_type);

    field_t& get_key(propdecl_t &decl, int keyidx);

    bool parse(const string &xml, propunit_t&, errvec_t&);

    bool parse_decl(const string &file, const propunit_t &propunit, propdecl_t&, errvec_t&);
}

#endif // _prop_decl_h_