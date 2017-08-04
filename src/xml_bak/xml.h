/*xml.h*/

#ifndef XML_H
#define XML_H

#include <stdio.h>
#include <errno.h>
#include <libxml/parser.h>

#define MAX_PATH_LEN  (256)

#define DPRINT(fmt, args...) \
{\
   printf("DEBUG file(%s)-line(%05d)-->fun(%s): "fmt, __FILE__, __LINE__, __FUNCTION__, ##args);\
}

#define EPRINT(fmt, args...) \
{\
    printf("ERROR file(%s)-line(%05d)-->fun(%s): "fmt, __FILE__, __LINE__, __FUNCTION__, ##args);\
}

xmlDocPtr new_xmldoc(xmlNodePtr *root_node, const char *root_name);

xmlNodePtr new_node(const char *key_name, const char *value);

//xmlNodePtr add_child_new_node(xmlNodePtr father, const char *key, const char *value);
xmlNodePtr add_child_new_node(xmlNodePtr father, const char *node_name);

xmlNodePtr get_child_node(xmlNodePtr cur_node, char *elem_name);

xmlChar* get_child_node_value(xmlNodePtr cur_node, char *elem_name);

int update_child(xmlNodePtr node_father, char *key_name, const char *value);

int del_child_node(xmlNodePtr father_node, char *child_name);

void print_xmlerr();

xmlDocPtr open_xmldoc_file(xmlNodePtr *root_node, const char *root_name, const char *file);

int rename_conf(const char *file_tmp, const char *file, const char *svr_id);

int write_xml_file(const char *path, xmlDocPtr xml_doc);


//解析模块
//生成模块
//编码转换模块

#endif
