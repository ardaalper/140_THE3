#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "the3.h"
DomainFreqNode* createFreqNode(char* n,int f,DomainFreqNode* p, DomainFreqNode* ne);
DomainFreqList* createFreqList(DomainFreqNode* h, DomainFreqNode* t);
CacheNode* createCacheNode(CacheNode* p, CacheNode* ne, DomainFreqList* l, char* n, int s);
void insertNode(DomainFreqList* list, DomainFreqNode* newNode);
/*
    Create a cache by reading inputs from stdin
*/
Cache* createCache()
{
    char str[1000];
    const char s[2] = " ";
    char *token;
    
    int isize, imediacount;

    int i;
    int itemp1;
    char* stemp1;
    

    DomainFreqNode* newNode;

    
    DomainFreqList* templist;
    CacheNode* cachenodetemp;
    CacheNode* cachenodecurrent;
    Cache* bizimcache;
    
    bizimcache = (Cache*)malloc(sizeof(Cache));
    
    scanf("%d %d\n",&isize,&imediacount);

    bizimcache->head=NULL;
    bizimcache->tail=NULL;
    bizimcache->mediaCount=imediacount;
    bizimcache->currentSize=0;
    bizimcache->cacheLimit=isize;
    
    for(i=0;i<imediacount;i++)
    {
        fgets(str, 1000, stdin);
        token = strtok(str, s);
        stemp1 =token;
        
        token = strtok(NULL, s);
        itemp1 = atoi(token);

        templist=createFreqList(NULL, NULL);

        cachenodetemp=createCacheNode(NULL,NULL,templist,stemp1,itemp1);
        

        while(token!=NULL) 
        {   
            
            token = strtok(NULL, s);
            if (token==NULL)
                break;
            stemp1= token;
            token = strtok(NULL, s);
            itemp1= atoi(token);
            
                
            newNode=createFreqNode(stemp1,itemp1,NULL,NULL);

            insertNode(cachenodetemp->domainFreqList,newNode);

        }

            
            
        if (bizimcache->head==NULL)
        {
            bizimcache->head=cachenodetemp;
        }
        else
        {
            cachenodecurrent=bizimcache->head;
            while (cachenodecurrent->next!=NULL)
            {
                cachenodecurrent=cachenodecurrent->next;
            }
            cachenodecurrent->next=cachenodetemp;
            cachenodetemp->prev=cachenodecurrent;
            
        }
        bizimcache->tail=cachenodetemp;
        bizimcache->currentSize+=cachenodetemp->media.size;
    }

    return bizimcache;
}

/*
    Prints cache information and its contents
*/
void printCache(Cache* cache)
{   
    
    
    CacheNode* cachenodetemp;
    DomainFreqNode* dfntemp;
    
    if (cache->head==NULL)
    {
        printf("The Cache is Empty");
        return;
    }    
    printf("-------------- Cache Information --------------\n");
    printf("Cache Limit: %d KB\n",cache->cacheLimit);
    printf("Current Size: %d KB\n",cache->currentSize);
    printf("Media Count: %d\n",cache->mediaCount);
    printf("Cache Media Info:\n");
    
    cachenodetemp=cache->head;
    
    while (cachenodetemp!=NULL)
    {
        printf("    Name: %s    Size: %d KB\n",cachenodetemp->media.name,cachenodetemp->media.size);
        printf("    ------- Media Domain Frequencies -------\n");
        dfntemp=cachenodetemp->domainFreqList->head;
        while(dfntemp!=NULL)
        {
            printf("        Name: %s        Freq: %d\n",dfntemp->name,dfntemp->freq);
            dfntemp=dfntemp->next;
        }
        printf("    ----------------------------------------\n");
        cachenodetemp=cachenodetemp->next;
    }
    printf("----------------------------------------------\n");
}


/*
    Adds media to the cache
*/
CacheNode* addMediaRequest(Cache* cache, Media media, char* domain)
{
    int flag1=0;
    int flag2=0;
    int k;
    CacheNode* tempnode;
    CacheNode* tempnode1;
    DomainFreqList* dflist;
    DomainFreqList* templist;
    DomainFreqNode* dfnode;
    DomainFreqNode* dfnode2;
    DomainFreqNode* dfnode3;
    
    
    tempnode=cache->head;
    while (tempnode != NULL)
    {
        if (strcmp(tempnode->media.name, media.name) == 0)
        {
            flag1=1;
            break;
        }
        else
            tempnode = tempnode->next;
    }
    
    
    if(flag1)/*media varsa*/
    {
        dflist=tempnode->domainFreqList;
        dfnode=dflist->head;
        
        while (dfnode != NULL)
        {
            if (strcmp(dfnode->name, domain) == 0)
            {
                flag2=1;
                break;
            }
            else
                dfnode = dfnode->next;
        }
        

        if (flag2)/*domain varsa*/
        {   
            k=(dfnode->freq)+1;
            dfnode2=createFreqNode(domain,k,NULL,NULL);
            /*head ve tail durumları*/
            if (dfnode->next==NULL)
            {
                dfnode->prev->next=NULL;
                dflist->tail=dfnode->prev;
                dfnode->prev=NULL;
            }
            else if (dfnode->prev==NULL)
            {
                dfnode->next->prev=NULL;
                dflist->head=dfnode->next;
                dfnode->next=NULL;
            }
            else
            {    
                dfnode->next->prev=dfnode->prev;
                dfnode->prev->next=dfnode->next;
                dfnode->next=NULL;
                dfnode->prev=NULL;
            }
            insertNode(dflist, dfnode2);
            
        }
        
        else/*domain yoksa*/
        {
            dfnode2=createFreqNode(domain,1,NULL,NULL);
            insertNode(dflist, dfnode2);
        }
        
        /*tüm işlemlerin sonunda cachenodeu head yap*/
        if (cache->head != tempnode)
        {
            if (tempnode->next==NULL)
            {
                cache->tail=tempnode->prev;
                cache->tail->next=NULL;
                tempnode->prev=NULL;
                tempnode->next=cache->head;
                cache->head->prev=tempnode;
                cache->head=tempnode;
            }
            else
            {
                tempnode->prev->next=tempnode->next;
                tempnode->next->prev=tempnode->prev;
                tempnode->next=cache->head;
                tempnode->prev=NULL;
                cache->head=tempnode;
                tempnode->next->prev=tempnode;
            }
        }
    }
    
    else/*media yoksa*/
    {
        templist=createFreqList(NULL, NULL);
        dfnode3=createFreqNode(domain,1,NULL, NULL);
        insertNode(templist,dfnode3);
        tempnode1=createCacheNode(NULL,NULL,templist,media.name,media.size);
        
        if (cache->currentSize+media.size > cache->cacheLimit)/*yeterli yer yoksa*/ 
        {
            while ((cache->currentSize + media.size) > cache->cacheLimit)/*yer ayıklama*/
            {
                cache->currentSize -= cache->tail->media.size;
                cache->mediaCount--;
                
                if (cache->head==cache->tail)
                {
                    cache->head=NULL;
                    cache->tail=NULL;
                }
                else
                {
                    cache->tail=cache->tail->prev;
                    cache->tail->next->prev=NULL;
                    cache->tail->next=NULL;
                }
            }
            if (cache->head==NULL)
            {
                cache->head=tempnode1;
                cache->tail=tempnode1;
                cache->currentSize += media.size;
                cache->mediaCount++;
            }
            else
            {
                tempnode1->next=cache->head;
                cache->head->prev=tempnode1;
                cache->head=tempnode1;
                
                cache->currentSize += media.size;
                cache->mediaCount++;
            }
        }
        else
        {
            tempnode1->next=cache->head;
            cache->head->prev=tempnode1;
            cache->head=tempnode1;
            
            cache->currentSize += media.size;
            cache->mediaCount++;
        }
    }
    
    if(flag1)
        return tempnode;
    else
        return NULL;
}


/*
    Finds the corresponding media's node in the cache
    Return NULL if it is not found
*/
CacheNode* findMedia(Cache* cache, char* name)
{

    int flag=0;
    CacheNode* node;
    
    node=cache->head;
    
    while (node != NULL)
    {
        if (strcmp((node->media).name, name) == 0)
        {
            flag=1;
            break;

        }
        else
        {
            node = node->next;
        }
    }
    
    if(flag)
        return node;
    else
        return NULL;
}

/*
    Finds a media from its name and deletes it's node from the cache
    Do not do anything if media is not found
*/
void deleteMedia(Cache* cache, char* name)
{
    int flag=0;
    CacheNode* node;
    
    node=cache->head;
    
    while (node != NULL)
    {
        if (strcmp((node->media).name, name) == 0)
        {
            flag=1;
            break;
        }
        else
            node = node->next;
    }
    if(0==flag)
        return;
        
    node->prev->next=node->next;
    node->next->prev=node->prev;
    
    (cache->mediaCount)--;
    cache->currentSize -= node->media.size;
}

DomainFreqNode* createFreqNode(char* n,int f,DomainFreqNode* p, DomainFreqNode* ne)
{   
    DomainFreqNode* newNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
    newNode->name = (char*)malloc(strlen(n) + 1);
    strcpy(newNode->name, n);
    newNode->freq = f;
    newNode->prev = p;
    newNode->next = ne;
    return newNode;
}

DomainFreqList* createFreqList(DomainFreqNode* h, DomainFreqNode* t)
{
    DomainFreqList* newList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
    newList->head=h;
    newList->tail=t;
    return newList;
}

CacheNode* createCacheNode(CacheNode* p, CacheNode* ne, DomainFreqList* l, char* n, int s)
{
    CacheNode* newCacheNode = (CacheNode*)malloc(sizeof(CacheNode));
    newCacheNode->domainFreqList=l;
    newCacheNode->prev=p;
    newCacheNode->next=ne;
    
    newCacheNode->media.size = s;
    newCacheNode->media.name = (char*)malloc(strlen(n) + 1);
    strcpy(newCacheNode->media.name, n);
    
    
    return newCacheNode;
}

void insertNode(DomainFreqList* list, DomainFreqNode* newNode) {
    DomainFreqNode* current;
    current = list->head;

    /* Empty list or newNode should be the new head*/ 
    if (current == NULL)
    {
        newNode->next = NULL;
        newNode->prev = NULL;
        list->head = newNode;
        list->tail = newNode;
        return;
    }
    
    if (newNode->freq > current->freq)
    {/*liste başı*/
        newNode->next = current;
        newNode->prev = NULL;
        
        current->prev = newNode;
        list->head = newNode;

        return;
    }
    
    if (strcmp(newNode->name, current->name) < 0 && newNode->freq == current->freq)
    {/*liste başı*/
        newNode->next = current;
        newNode->prev = NULL;
        
        current->prev = newNode;
        list->head = newNode;

        return;
    }
    /* Find the position to insert newNode*/ /*ERROR*/
    while(current->next != NULL && (newNode->freq < current->next->freq ))
    {
        current = current->next;

    }
    if (current->next != NULL && newNode->freq == current->next->freq )
        while (current->next != NULL && newNode->freq == current->next->freq)
        {   
            if (strcmp(newNode->name, current->next->name) > 0)
                current = current->next;
            else 
                break;
        }
    
    

    /* Insert newNode*/
    newNode->next = current->next;
    if (current->next != NULL) 
    {
        current->next->prev = newNode;
    } 
    else 
    {
        list->tail = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
}

