/*
 * =====================================================================================
 *
 *       Filename:  test_xml.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/25/17 18:35:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "xml/wv_xml.h"

#include "log/log.h"

#include <stdio.h>
#include <stdlib.h>

void travesal_node(xmlNodePtr cur_node)
{
    if(cur_node == NULL)
    {
        return ;
    }

    cur_node = cur_node->xmlChildrenNode;
    while(cur_node)
    {
        printf("name: %s\n", cur_node->name);
        travesal_node(cur_node);
        cur_node = cur_node->next;
    }

}

void travesal_xml(const char *filename)
{
    xmlDocPtr xml_doc = XML_OpenXmlFile(filename, NULL);
    xmlNodePtr cur_node = NULL;
    
    if(!xml_doc)
    {
        printf("xml_doc = NULL\n");
        return ;
    }

    xmlChar *xpath=(xmlChar*)"//keyword";  
    xmlNodeSetPtr nodeset = NULL;  
    xmlXPathObjectPtr result = NULL;  
    int i;  
    xmlChar *keyword = NULL; 

    result = XML_GetNodeSet(xml_doc, xpath);  
    if(result){  
        nodeset = result->nodesetval;  
        for(i=0; i < nodeset->nodeNr; i++){
            keyword = xmlNodeListGetString(xml_doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);  
            printf("keyword: %s\n", keyword);  
            xmlFree(keyword);  
        }  
        xmlXPathFreeObject(result);  
    }  

    cur_node = xmlDocGetRootElement(xml_doc);

    travesal_node(cur_node);

    XML_FreeDoc(xml_doc);
}


void create_xml(const char *file_name, const char *root_name)
{
     xmlDocPtr xml_doc = NULL;
     xmlNodePtr root_node = NULL;
     xmlNodePtr node = NULL;

     xml_doc = XML_NewXmlDoc(&root_node, root_name);
     node = XML_NewNode("book", "C plus plus");
     {
        XML_AddChildCopyNode(root_node, node);
     }
     XML_AddNewChildNode(root_node, "book", "How to learn python");
     XML_WriteXmlFile(file_name, xml_doc);

}


int main(int argc, char *argv[])
{
    Log_Init("/home/mark/log.txt", 2000);    
    //travesal_xml("new_test.xml");
    create_xml("new_test.xml", "bookstore");
    return 0;
}


