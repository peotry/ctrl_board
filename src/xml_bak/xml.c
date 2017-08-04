/*xml.c*/

#include "xml.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * 功能：新建立一个空的xmldoc, 并返回doc, root 指针, doc用完要释放
 * 参数1：新的xml的根结点指针
 * 参数2：根结点名称
 * 返回值：成功返回返回doc指针，失败返回NULL
 */
xmlDocPtr new_xmldoc(xmlNodePtr *root_node, const char *root_name) //  res_root_node 
{
    xmlDocPtr xml_doc = NULL;
    if(!root_name)
    {
        EPRINT("null pointer");
        return NULL;
    }

    //目标xmldoc
    xml_doc = xmlNewDoc(BAD_CAST"1.0");
    if(!xml_doc)
    {
        EPRINT("xmlNewDoc error!\n");
        return NULL;
    }
    //创建root结点
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
 * 功能：新建节点
 * 参数1：新结点的名称
 * 参数2：新节点的值,value为空,则创建空结点
 * 返回值：成功返回返回节点指针，失败返回NULL
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
    //value不空则增加内容结点, 为空则创建空结点
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
 * 功能：向father节点下面添加一个结点,不管里面有没有重名的
 * 参数1：父节点指针
 * 参数2：孩子结点的名称
 * 参数3: 孩子节点的值
 * 返回值：成功返回返回节点指针，失败返回NULL
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
 * 功能：在当前结点cur_node下,找元素名为elem_name的子结点指针
 * 参数1：cur_node当前结点
 * 参数2：elem_name要找了子结点名称
 * 返回值：成功返回结点指针，失败返回NULL
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
 * 功能：获取cur_node下名称是elem_name的子结点的值
 * 参数1：cur_node当前结点
 * 参数2：elem_name要找了子结点名称
 * 返回值：成功返回结点值，失败返回NULL, 值使用完要主动释放
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
 * 功能：编辑节点,更新key_name结点,存在kye_name结点,则覆盖老的
 * 参数1：父节点指针
 * 参数2：孩子结点的名称
 * 参数3: 孩子节点的值
 * 返回值：成功返回返回0，失败返回-1
 */
//确保更新的元素不是数组，因为存在同名的则替换，不存则新增
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
        //存在则替换,防止有非法同名的
        xmlNodePtr new_xml_node = new_node(key_name, value);
        if(!new_xml_node)
        {
            return -1;
        }
        if(NULL == xmlReplaceNode(tmp_child_node, new_xml_node))
        {
            xmlFreeNodeList(new_xml_node);//替换失败，要释放新的
            return -1;
        }
        xmlFreeNodeList(tmp_child_node);//替换成功，要释放老的
        return 0;
    }
    else
    {
        //不存则加入到父结点下面
        if(NULL == xmlNewTextChild(node_father, NULL, (xmlChar*)key_name, (xmlChar*)value))
        {
            EPRINT("xmlNewTextChild error! %s=%s\n", key_name, value);
            return -1;
        }
    }
    return 0;
}

/*
 * 功能：删除节点,有多个同名的话也是只删除第一个
 * 参数1：父节点指针
 * 参数2：孩子结点的名称
 * 返回值：成功返回返回0
 */
int del_child_node(xmlNodePtr father_node, char *child_name)
{
    xmlNodePtr tmp_node = get_child_node(father_node, child_name);
    if(tmp_node)
    {
        //存在结点, 则把此结点删除
        xmlUnlinkNode(tmp_node);
        xmlFreeNodeList(tmp_node);
        tmp_node = NULL;
    }
    return 0;
}

/*
 * 功能：
 * 参数1：
 * 参数2：
 * 参数3：
 * 返回值：成功返回0，失败返回非0
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
 * 功能：以UTF8打开一个xml,并作xml格式检查
 * 参数1：打开后xml的根结点指针
 * 参数2：根结点名称，需要做校验
 * 参数3：xml文件路径
 * 返回值：成功返回xmldoc，失败返回NULL
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
    //root name为空,不检根名称
    if(root_name)
    {
        //确保根结点是root名字,可以一定程序判断合法性
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
 * 功能：把service的临时写好的配置文件rename成正式文件
 *       原子操作，防止断电配置损坏, 新增、修改都要这样做。
 * 参数1：临时文件file_tmp
 * 参数2：正式文件file
 * 参数3：svr_id号, 不空,则是填充前面两个参数是格式化%s
 * 返回值：成功返回0，失败返回非0
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
        //没有填充字符,则是直接把file_tmp rename 成 file
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
 * 功能：把一个xmlDoc原子写入文件
 * 参数1：目标文件路径
 * 参数2: xml文档指针
 * 返回值：成功返回0，失败返回-1
 */
int write_xml_file(const char *path, xmlDocPtr xml_doc)
{
    //构造路径
    char path_tmp[MAX_PATH_LEN] = {0};
    int cnt = snprintf(path_tmp, sizeof(path_tmp)-1, "%s.tmp", path);
    if(cnt < 1 || cnt > (sizeof(path_tmp)-1))
    {
        EPRINT("err get_netinput_info path too long (%s)", path);
        return -1;
    }

    //然后写临时文件
    if(1 > xmlSaveFormatFileEnc(path_tmp, xml_doc, (const char *) TXT_ENCODING, 1)) //返回写入的字节数
    {
        EPRINT("xmlSaveFormatFile err(%d), path=%s", errno, path_tmp);
        return -1;
    }

    //然后rename
    return rename_conf(path_tmp, path, NULL);
}



