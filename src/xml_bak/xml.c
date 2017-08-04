/*xml.c*/

#include "xml.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * ���ܣ��½���һ���յ�xmldoc, ������doc, root ָ��, doc����Ҫ�ͷ�
 * ����1���µ�xml�ĸ����ָ��
 * ����2�����������
 * ����ֵ���ɹ����ط���docָ�룬ʧ�ܷ���NULL
 */
xmlDocPtr new_xmldoc(xmlNodePtr *root_node, const char *root_name) //  res_root_node 
{
    xmlDocPtr xml_doc = NULL;
    if(!root_name)
    {
        EPRINT("null pointer");
        return NULL;
    }

    //Ŀ��xmldoc
    xml_doc = xmlNewDoc(BAD_CAST"1.0");
    if(!xml_doc)
    {
        EPRINT("xmlNewDoc error!\n");
        return NULL;
    }
    //����root���
    xmlNodePtr root_node_tmp = xmlNewNode(NULL, BAD_CAST root_name); // root_name: "test"
    if(root_node_tmp == NULL)
    {
        EPRINT("xmlNewNode root error!\n");
        xmlFreeDoc(xml_doc);
        return NULL;
    }
    xmlDocSetRootElement(xml_doc, root_node_tmp);
    *root_node = root_node_tmp;

    return xml_doc; // respose_7u.xml
} 

/*
 * ���ܣ��½��ڵ�
 * ����1���½�������
 * ����2���½ڵ��ֵ,valueΪ��,�򴴽��ս��
 * ����ֵ���ɹ����ط��ؽڵ�ָ�룬ʧ�ܷ���NULL
 */
xmlNodePtr new_node(const char *key_name, const char *value)
{
    if(!key_name)
    {
        return NULL;
    }

    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST key_name);
    if(!node)
    {
        return NULL;
    }
    //value�������������ݽ��, Ϊ���򴴽��ս��
    if(value)
    {
        xmlNodePtr content = xmlNewText(BAD_CAST value);
        if(!content)
        {
            xmlFreeNode(node);
            return NULL;
        }

        if(NULL == xmlAddChild(node, content))
        {
            xmlFreeNode(node);
            xmlFreeNode(content);
            return NULL;
        }
    }
    return node;
}

/*
 * ���ܣ���father�ڵ��������һ�����,����������û��������
 * ����1�����ڵ�ָ��
 * ����2�����ӽ�������
 * ����3: ���ӽڵ��ֵ
 * ����ֵ���ɹ����ط��ؽڵ�ָ�룬ʧ�ܷ���NULL
 */
 xmlNodePtr add_child_new_node(xmlNodePtr father, const char *node_name)
 {	
 	//xmlNodePtr tmp_node = new_node(key, value);
 	xmlNodePtr tmp_node = xmlNewNode(NULL, BAD_CAST node_name);
    if(!tmp_node)
    {
        EPRINT("new node err, errno=%d", errno);
        return NULL;
    }

    if(NULL == xmlAddChild(father, tmp_node))
    {
        xmlFreeNodeList(tmp_node);
        EPRINT("xml add node err, errno=%d", errno);
        return NULL;
    }
    return tmp_node;
}


/*
 * ���ܣ��ڵ�ǰ���cur_node��,��Ԫ����Ϊelem_name���ӽ��ָ��
 * ����1��cur_node��ǰ���
 * ����2��elem_nameҪ�����ӽ������
 * ����ֵ���ɹ����ؽ��ָ�룬ʧ�ܷ���NULL
 */
xmlNodePtr get_child_node(xmlNodePtr cur_node, char *elem_name)
{
    if(!cur_node || !elem_name)
    {
        EPRINT("null pointer");
        return NULL;
    }
    //DPRINT("CUR NAME=%s", cur_node->name);

    xmlNodePtr tmp_node = cur_node->children;
    while(tmp_node != NULL)
    {
        //DPRINT("%s--%s", tmp_node->name, elem_name);
        if (tmp_node->name && 0 == xmlStrcmp(tmp_node->name, (const xmlChar *)elem_name))
        {
            //DPRINT("get it %s=%s", tmp_node->name, elem_name);
            return tmp_node;
        }
        tmp_node = tmp_node->next;
    }
    return NULL;
}

/*
 * ���ܣ���ȡcur_node��������elem_name���ӽ���ֵ
 * ����1��cur_node��ǰ���
 * ����2��elem_nameҪ�����ӽ������
 * ����ֵ���ɹ����ؽ��ֵ��ʧ�ܷ���NULL, ֵʹ����Ҫ�����ͷ�
 */
xmlChar* get_child_node_value(xmlNodePtr cur_node, char *elem_name)
{
    if(!cur_node || !elem_name)
    {
        EPRINT("null pointer");
        return NULL;
    }

    xmlNodePtr child_node = get_child_node(cur_node, elem_name);
    if(!child_node || !child_node->children)
    {
        DPRINT("get_child_node do not have child or content,  key(%s)", elem_name);
        return NULL;
    }
    return xmlNodeGetContent(child_node->children);
}

/*
 * ���ܣ��༭�ڵ�,����key_name���,����kye_name���,�򸲸��ϵ�
 * ����1�����ڵ�ָ��
 * ����2�����ӽ�������
 * ����3: ���ӽڵ��ֵ
 * ����ֵ���ɹ����ط���0��ʧ�ܷ���-1
 */
//ȷ�����µ�Ԫ�ز������飬��Ϊ����ͬ�������滻������������
int update_child(xmlNodePtr node_father, char *key_name, const char *value)
{
    if(!node_father || !key_name)
    {
        EPRINT("empty pointer");
        return -1;
    }

    xmlNodePtr tmp_child_node = get_child_node(node_father, key_name);
    if(tmp_child_node)
    {
        //�������滻,��ֹ�зǷ�ͬ����
        xmlNodePtr new_xml_node = new_node(key_name, value);
        if(!new_xml_node)
        {
            return -1;
        }
        if(NULL == xmlReplaceNode(tmp_child_node, new_xml_node))
        {
            xmlFreeNodeList(new_xml_node);//�滻ʧ�ܣ�Ҫ�ͷ��µ�
            return -1;
        }
        xmlFreeNodeList(tmp_child_node);//�滻�ɹ���Ҫ�ͷ��ϵ�
        return 0;
    }
    else
    {
        //��������뵽���������
        if(NULL == xmlNewTextChild(node_father, NULL, (xmlChar*)key_name, (xmlChar*)value))
        {
            EPRINT("xmlNewTextChild error! %s=%s\n", key_name, value);
            return -1;
        }
    }
    return 0;
}

/*
 * ���ܣ�ɾ���ڵ�,�ж��ͬ���Ļ�Ҳ��ֻɾ����һ��
 * ����1�����ڵ�ָ��
 * ����2�����ӽ�������
 * ����ֵ���ɹ����ط���0
 */
int del_child_node(xmlNodePtr father_node, char *child_name)
{
    xmlNodePtr tmp_node = get_child_node(father_node, child_name);
    if(tmp_node)
    {
        //���ڽ��, ��Ѵ˽��ɾ��
        xmlUnlinkNode(tmp_node);
        xmlFreeNodeList(tmp_node);
        tmp_node = NULL;
    }
    return 0;
}

/*
 * ���ܣ�
 * ����1��
 * ����2��
 * ����3��
 * ����ֵ���ɹ�����0��ʧ�ܷ��ط�0
 */
void print_xmlerr()
{
    xmlErrorPtr perr = xmlGetLastError();
    if(!perr)
    {
        return;
    }
    EPRINT("xml err(%d): %s\n", perr->code, perr->message);
    //xmlFree(perr);
    return;
}

/*
 * ���ܣ���UTF8��һ��xml,����xml��ʽ���
 * ����1���򿪺�xml�ĸ����ָ��
 * ����2����������ƣ���Ҫ��У��
 * ����3��xml�ļ�·��
 * ����ֵ���ɹ�����xmldoc��ʧ�ܷ���NULL
 */
xmlDocPtr open_xmldoc_file(xmlNodePtr *root_node, const char *root_name, const char *file)
{
    xmlDocPtr xml_doc = NULL;
    if(!root_node || !file)
    {
        EPRINT("null pointer\n");
        return NULL;
    }

    xml_doc = xmlReadFile(file, "UTF-8", XML_PARSE_NOBLANKS); // UTF-8
    if(!xml_doc)
    {
        EPRINT("xml read file error!\n");
        print_xmlerr();
        return NULL;
    }
    xmlNodePtr cur_node_tmp = xmlDocGetRootElement(xml_doc);
    if(cur_node_tmp == NULL)
    {
        EPRINT("xmlDoc GetRoot Element error!\n");
        xmlFreeDoc(xml_doc);
        return NULL;
    }
    //root nameΪ��,���������
    if(root_name)
    {
        //ȷ���������root����,����һ�������жϺϷ���
        if(0 != xmlStrcmp(cur_node_tmp->name, (xmlChar*)root_name))
        {
            EPRINT("xml root Element name error! it must be '%s'\n", root_name);
            xmlFreeDoc(xml_doc);
            return NULL;
        }
    }

    *root_node = cur_node_tmp;
    return xml_doc;
}

/*
 * ���ܣ���service����ʱд�õ������ļ�rename����ʽ�ļ�
 *       ԭ�Ӳ�������ֹ�ϵ�������, �������޸Ķ�Ҫ��������
 * ����1����ʱ�ļ�file_tmp
 * ����2����ʽ�ļ�file
 * ����3��svr_id��, ����,�������ǰ�����������Ǹ�ʽ��%s
 * ����ֵ���ɹ�����0��ʧ�ܷ��ط�0
 */
int rename_conf(const char *file_tmp, const char *file, const char *svr_id)
{
    if(!file_tmp || !file)
    {
        EPRINT("empty pointer");
        return -1;
    }

    if(!svr_id)
    {
        //û������ַ�,����ֱ�Ӱ�file_tmp rename �� file
        return rename(file_tmp, file);
    }

    char path[MAX_PATH_LEN] = {0};
    char path_tmp[MAX_PATH_LEN] = {0};
    int cnt = snprintf(path, sizeof(path)-1, file, svr_id);
    if(cnt < 1 || cnt > (sizeof(path)-1))
    {
        EPRINT("err service conf path too long (%s)", path);
        return -1;
    }
    cnt = snprintf(path_tmp, sizeof(path_tmp)-1, file_tmp, svr_id);
    if(cnt < 1 || cnt > (sizeof(path_tmp)-1))
    {
        EPRINT("err service conf path too long (%s)", path_tmp);
        return -1;
    }
    //rename conf
    return rename(path_tmp, path);
}

#define TXT_ENCODING      "GB2312"
/*
 * ���ܣ���һ��xmlDocԭ��д���ļ�
 * ����1��Ŀ���ļ�·��
 * ����2: xml�ĵ�ָ��
 * ����ֵ���ɹ�����0��ʧ�ܷ���-1
 */
int write_xml_file(const char *path, xmlDocPtr xml_doc)
{
    //����·��
    char path_tmp[MAX_PATH_LEN] = {0};
    int cnt = snprintf(path_tmp, sizeof(path_tmp)-1, "%s.tmp", path);
    if(cnt < 1 || cnt > (sizeof(path_tmp)-1))
    {
        EPRINT("err get_netinput_info path too long (%s)", path);
        return -1;
    }

    //Ȼ��д��ʱ�ļ�
    if(1 > xmlSaveFormatFileEnc(path_tmp, xml_doc, (const char *) TXT_ENCODING, 1)) //����д����ֽ���
    {
        EPRINT("xmlSaveFormatFile err(%d), path=%s", errno, path_tmp);
        return -1;
    }

    //Ȼ��rename
    return rename_conf(path_tmp, path, NULL);
}



