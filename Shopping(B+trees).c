#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>//1 5 6 8 9
#define M 3     //M represents at max how many children index nodes may have
#define C 3     //C represents at max homany children data nodes i.e. leaf nodes can have
/*
We have Implemented two types of trees :
1)B+ tree for products in walmart
2)Nested B+ tree i.e B+ inside a B+ tree for collection of bills
*/
unsigned int idgen=0,aislecnt=0,bid=0;  //It generates ids for items,aisle and bill respectively
struct date
{
    unsigned int dd,mm,yyyy;
};
/*This structures represents and item in a store*/
struct aisle_node
{
    unsigned int item_id,quantity,aisle_no,price,threshold;
    char*item_name;
    struct date u;
};
/*It is a node from bill for each item you bought*/
struct bill_node
{
    unsigned int item_id,quantity,price;
    char*item_name;
};
/*Collection of bill_nodes i.e. items that will act as doubly linked list has following structure*/
struct bill_data_node
{
    struct bill_node*arr[C+1];
    int size;
    struct bill_data_node*left;
    struct bill_data_node*right;
};
/*It is data node i.e. leaf node for  products in store*/
struct datanode{
    struct aisle_node*data[C+1];
    int size;
    struct datanode*left;
    struct datanode*right;
};
/*It is node that stores index value*/
/*Note that this node fulfills 6 roles :
    1)  As index value and data value nodes for products
    2)  As index value and data value nodes for each bill
    3)  As index value and data value nodes for collection of bills.
*/
struct node{
    unsigned int data[M];
    int size;
    int flag;
    unsigned int billid;
    union{
        struct node*next1[M+1];
        struct datanode*next2[M+1];
        struct bill_data_node*next3[M+1];
        struct ultimatenode*next4[M+1];
    }u;
};
/*This nodes is used as data values for collection  of bills tree*/
struct ultimatenode{
    struct node*arr[C+1];
    int size;
    struct ultimatenode*left;
    struct ultimatenode*right;
};
/*
1) Here Datamine refers to data regarding which else products are bought along with given product
2) Updatedatamine2 main work is to update datamine when compation takes place
*/
void updatedatamine2(int size1,unsigned int*datamine[size1],int num1,int num2)
{
    for(int i=0;i<size1;i++)
    {
        datamine[i][num1]=datamine[i][num2];
        if(num1!=num2)
        {
            datamine[i][num2]=0;
        }
    }
    for(int i=0;i<size1;i++)
    {
        datamine[num1][i]=datamine[num2][i];
        if(num1!=num2)
        {
            datamine[num2][i]=0;
        }
    }
}
/*
Along period of time there may be case where ssome aisle is deleted and array has became sparse,
in such case we compact the array once when array hits its maxsize
*/
int CompactTree(struct node*root,int*num,int aisle,int size,unsigned int*datamine[size])
{
    int retval=0;
    if(root)
    {
        if(root->flag==0)
        {
            CompactTree(root->u.next1[0],num,aisle,size,datamine);
            for(int i=1;i<=root->size;i++)
            {
                root->data[i-1]=CompactTree(root->u.next1[i],num,aisle,size,datamine);
            }
        }
        else
        {
            for(int i=0;i<=root->size;i++)
            {
                struct datanode*dptr=root->u.next2[i];
                for(int j=0;j<dptr->size;j++)
                {
                    updatedatamine2(size,datamine,dptr->data[j]->item_id,*num);
                    dptr->data[j]->item_id=*num;
                    dptr->data[j]->aisle_no=aisle;
                    ++(*num);
                }
                if(i!=0)
                {
                    root->data[i-1]=dptr->data[0]->item_id;
                }
            }
        }
        retval=root->data[0];
    }
    return retval;
}
void compaction(struct node**arr,unsigned int size,unsigned int size1,unsigned int*datamine[size1])
{
    unsigned int j=0;
    int cnt=0;
    for(unsigned int i=0;i<size;i++)
    {
        if(arr[i])
        {
            arr[j]=arr[i];
            if(i!=j)arr[i]=NULL;
            CompactTree(arr[j],&cnt,j,size1,datamine);
            j++;
        }
    }
    aislecnt=j;
    idgen=cnt;
}
struct node*createnode()
{
    struct node*retval=(struct node*)malloc(sizeof(struct node));
    retval->size=0;
    retval->flag=0;
    for(int i=0;i<M;i++)retval->u.next1[i]=NULL;
 //   for(int i=0;i<M-1;i++)retval->data[i]=0;
    return retval;
}
struct aisle_node*createAislenode(unsigned int quantity,unsigned int aisle_no,unsigned int price,char*name,int threshold,int d,int m,int y)
{
    struct aisle_node*n=(struct aisle_node*)malloc(sizeof(struct aisle_node));
    n->item_id=idgen++;
    n->quantity=quantity;
    n->aisle_no=aisle_no;
    n->price=price;
    n->item_name=(char*)malloc((strlen(name)+1)*sizeof(char));
    strcpy(n->item_name,name);
    n->threshold=threshold;
    n->u.dd=d;
    n->u.mm=m;
    n->u.yyyy=y;
    printf("ID-no assigned, Remember it for future purpose -> ID-no : %u\n",n->item_id);
    return n;
}
struct aisle_node*search(struct node*root,int item_id)
{
    struct aisle_node*retval=NULL;
    if(root)
    {
        int i=0;
        while(i<root->size&&root->data[i]<=item_id)i++;
        if(root->flag==0)
        {
            retval=search(root->u.next1[i],item_id);
        }
        else
        {
            int j=0;
            while(j<root->u.next2[i]->size&&root->u.next2[i]->data[j]->item_id<item_id)j++;
            if(j<root->u.next2[i]->size&&root->u.next2[i]->data[j]->item_id==item_id)
            {
                retval=root->u.next2[i]->data[j];
            }
        }
    }
    return retval;
}
struct node*insert(struct node*root,struct aisle_node*key,struct bill_node*key1,struct node*key2,struct node*nptr,int index,int f)
{
    struct node*retval=NULL;
    if(root)
    {
        int i=0;
        if(f==0)while(i<root->size&&root->data[i]<=key->item_id)i++;
        else if(f==1) while(i<root->size&&root->data[i]<=key1->item_id)i++;
        else while(i<root->size&&root->data[i]<=key2->billid)i++;
        if(root->flag==0)
        {
            retval=insert(root->u.next1[i],key,key1,key2,root,i,f);
            if(retval)
            {
                /*i=0;
                while(i<root->size&&retval->data[0]>root->data[i])i++;*/
                for(int j=root->size;j>i;j--)
                {
                    root->data[j]=root->data[j-1];
                }
                for(int j=root->size+1;j>i+1;j--)
                {
                    root->u.next1[j]=root->u.next1[j-1];
                }
                root->data[i]=retval->data[0];
                ++root->size;
                root->u.next1[i+1]=retval->u.next1[1];
                free(retval);
                retval=NULL;
                if(root->size>M-1)
                {
                    int mid=root->size/2;
                    struct node*n1=createnode();
                    n1->data[0]=root->data[mid];
                    n1->size=1;
                    struct node*n2=createnode();
                    for(int k=mid+1;k<root->size;k++)
                    {
                        n2->data[k-mid-1]=root->data[k];
                        n2->u.next1[k-mid-1]=root->u.next1[k];
                        root->u.next1[k]=NULL;
                        if(k==root->size-1)
                        {
                            n2->u.next1[root->size-mid-1]=root->u.next1[root->size];
                            root->u.next1[root->size]=NULL;
                        }
                    }
                    n2->size=root->size-1-mid;
                    n1->u.next1[0]=root;
                    n1->u.next1[1]=n2;
                    root->size=mid;
                    retval=n1;
               //     return retval;
               
                }
            }
        }
        else
        {
         //       printf("aa");
            if(root->flag==1)
            {
                int j=0;

                while(j<root->u.next2[i]->size&&root->u.next2[i]->data[j]->item_id<key->item_id)j++;
        //        printf("%d\n",j);
                for(int k=root->u.next2[i]->size;k>j;k--)
                {
                    root->u.next2[i]->data[k]=root->u.next2[i]->data[k-1];
                }
         //       printf("\n%d",root->u.next2[i]->size);
                root->u.next2[i]->data[j]=key;
                ++root->u.next2[i]->size;
                if(root->u.next2[i]->size>C)
                {
                   // printf("%d",key);
                    struct datanode*nptr=(struct datanode*)malloc(sizeof(struct datanode));
                    for(int j=0;j<=C;j++)
                    {
                        nptr->data[i]=NULL;
                    }
                  //  printf("%d\n",key);
                    for(int k=root->u.next2[i]->size/2;k<root->u.next2[i]->size;k++)
                    {
                    //    printf("%d ",root->u.next2[i]->size);
                        nptr->data[k-root->u.next2[i]->size/2]=root->u.next2[i]->data[k];
                    }

                    nptr->size=root->u.next2[i]->size-root->u.next2[i]->size/2;
                    root->u.next2[i]->size/=2;
                    nptr->right=root->u.next2[i]->right;
                    nptr->left=root->u.next2[i];
                    root->u.next2[i]->right=nptr;

                    if(nptr->right)nptr->right->left=nptr;

                    for(int k=root->size+1;k>i+1;k--)
                    {
                        root->u.next2[k]=root->u.next2[k-1];
                    }
                    root->u.next2[i+1]=nptr;
                    for(int k=root->size;k>i;k--)
                    {
                        root->data[k]=root->data[k-1];
                    }                
                    root->data[i]=root->u.next2[i+1]->data[0]->item_id;
                    ++root->size;
                    if(root->size>=M)
                    {
                        int mid=root->size/2;
                      //  printf("\n%d",root->u.next2[3]->data[0]);
                        struct node*n1=createnode();
                        n1->data[0]=root->data[mid];
                        n1->size=1;
                        struct node*n2=createnode();
                        n2->flag=1;
                        for(int k=mid+1;k<root->size;k++)
                        {
                            n2->data[k-mid-1]=root->data[k];
                            n2->u.next2[k-mid-1]=root->u.next2[k];
                            root->u.next2[k]=NULL;
                            if(k==root->size-1)
                            {
                                n2->u.next2[root->size-mid-1]=root->u.next2[root->size];
                                root->u.next2[root->size]=NULL;
                            }
                        }
                        n2->size=root->size-1-mid;
                        n1->u.next1[0]=root;
                        n1->u.next1[1]=n2;
                        root->size=mid;
                        retval=n1;
                    }
                 //   printf("a");
                }
            }
            else if(root->flag==2)
            {
                int j=0;
                while(j<root->u.next3[i]->size&&root->u.next3[i]->arr[j]->item_id<key1->item_id)j++;
        //        printf("%d\n",j);
            //    printf("a");
                for(int k=root->u.next3[i]->size;k>j;k--)
                {
                    root->u.next3[i]->arr[k]=root->u.next3[i]->arr[k-1];
                }
         //       printf("\n%d",root->u.next2[i]->size);
                root->u.next3[i]->arr[j]=key1;
                ++root->u.next3[i]->size;
                if(root->u.next3[i]->size>C)
                {
                   // printf("%d",key);
                    struct bill_data_node*nptr=(struct bill_data_node*)malloc(sizeof(struct bill_data_node));
                    for(int j=0;j<=C;j++)
                    {
                        nptr->arr[i]=NULL;
                    }
                  //  printf("%d\n",key);
                    for(int k=root->u.next3[i]->size/2;k<root->u.next3[i]->size;k++)
                    {
                    //    printf("%d ",root->u.next2[i]->size);
                        nptr->arr[k-root->u.next3[i]->size/2]=root->u.next3[i]->arr[k];
                    }

                    nptr->size=root->u.next3[i]->size-root->u.next3[i]->size/2;
                    root->u.next3[i]->size/=2;
                    nptr->right=root->u.next3[i]->right;
                    nptr->left=root->u.next3[i];
                    root->u.next3[i]->right=nptr;

                    if(nptr->right)nptr->right->left=nptr;

                    for(int k=root->size+1;k>i+1;k--)
                    {
                        root->u.next3[k]=root->u.next3[k-1];
                    }
                    root->u.next3[i+1]=nptr;
                    for(int k=root->size;k>i;k--)
                    {
                        root->data[k]=root->data[k-1];
                    }                
                    root->data[i]=root->u.next3[i+1]->arr[0]->item_id;
                    ++root->size;
                    if(root->size>=M)
                    {
                        int mid=root->size/2;
                      //  printf("\n%d",root->u.next2[3]->data[0]);
                        struct node*n1=createnode();
                        n1->billid=bid;
                        n1->data[0]=root->data[mid];
                        n1->size=1;
                        struct node*n2=createnode();
                        n2->billid=bid;
                        n2->flag=2;
                        for(int k=mid+1;k<root->size;k++)
                        {
                            n2->data[k-mid-1]=root->data[k];
                            n2->u.next3[k-mid-1]=root->u.next3[k];
                            root->u.next3[k]=NULL;
                            if(k==root->size-1)
                            {
                                n2->u.next3[root->size-mid-1]=root->u.next3[root->size];
                                root->u.next3[root->size]=NULL;
                            }
                        }
                        n2->size=root->size-1-mid;
                        n1->u.next1[0]=root;
                        n1->u.next1[1]=n2;
                        root->size=mid;
                        retval=n1;
                    }
                 //   printf("a");
                }    
            }
            else if(root->flag==3)
            {
                int j=0;
                while(j<root->u.next4[i]->size&&root->u.next4[i]->arr[j]->billid<key2->billid)j++;
        //        printf("%d\n",j);
            //    printf("a");
                for(int k=root->u.next4[i]->size;k>j;k--)
                {
                    root->u.next4[i]->arr[k]=root->u.next4[i]->arr[k-1];
                }
         //       printf("\n%d",root->u.next2[i]->size);
                root->u.next4[i]->arr[j]=key2;
                ++root->u.next4[i]->size;
                if(root->u.next4[i]->size>C)
                {
                   // printf("%d",key);
                    struct ultimatenode*nptr=(struct ultimatenode*)malloc(sizeof(struct ultimatenode));
                    for(int j=0;j<=C;j++)
                    {
                        nptr->arr[i]=NULL;
                    }
                  //  printf("%d\n",key);
                    for(int k=root->u.next4[i]->size/2;k<root->u.next4[i]->size;k++)
                    {
                    //    printf("%d ",root->u.next2[i]->size);
                        nptr->arr[k-root->u.next4[i]->size/2]=root->u.next4[i]->arr[k];
                    }

                    nptr->size=root->u.next4[i]->size-root->u.next4[i]->size/2;
                    root->u.next4[i]->size/=2;
                    nptr->right=root->u.next4[i]->right;
                    nptr->left=root->u.next4[i];
                    root->u.next4[i]->right=nptr;

                    if(nptr->right)nptr->right->left=nptr;

                    for(int k=root->size+1;k>i+1;k--)
                    {
                        root->u.next4[k]=root->u.next4[k-1];
                    }
                    root->u.next4[i+1]=nptr;
                    for(int k=root->size;k>i;k--)
                    {
                        root->data[k]=root->data[k-1];
                    }                
                    root->data[i]=root->u.next4[i+1]->arr[0]->billid;
                    ++root->size;
                    if(root->size>=M)
                    {
                        int mid=root->size/2;
                      //  printf("\n%d",root->u.next2[3]->data[0]);
                        struct node*n1=createnode();
                        n1->data[0]=root->data[mid];
                        n1->size=1;
                        struct node*n2=createnode();
                        n2->flag=3;
                        for(int k=mid+1;k<root->size;k++)
                        {
                            n2->data[k-mid-1]=root->data[k];
                            n2->u.next4[k-mid-1]=root->u.next4[k];
                            root->u.next4[k]=NULL;
                            if(k==root->size-1)
                            {
                                n2->u.next4[root->size-mid-1]=root->u.next4[root->size];
                                root->u.next4[root->size]=NULL;
                            }
                        }
                        n2->size=root->size-1-mid;
                        n1->u.next1[0]=root;
                        n1->u.next1[1]=n2;
                        root->size=mid;
                        retval=n1;
                    }
                }
            }
        }
    }
    else
    {
        retval=root=createnode();
        root->size=1;
        if(f==0)
        {
            root->flag=1;
            root->data[0]=key->item_id;
            root->u.next2[0]=(struct datanode*)malloc(sizeof(struct datanode));
            root->u.next2[1]=(struct datanode*)malloc(sizeof(struct datanode));
            for(int j=0;j<=C;j++)
            {
                root->u.next2[0]->data[j]=root->u.next2[1]->data[j]=NULL;
            }
            root->u.next2[1]->data[0]=key;
            root->u.next2[1]->size=1;
            root->u.next2[0]->size=0;
            root->u.next2[0]->left=NULL;
            root->u.next2[0]->right=root->u.next2[1];
            root->u.next2[1]->left=root->u.next2[0];
            root->u.next2[1]->right=NULL;
        }
        else if(f==1)
        {
            root->flag=2;
            root->billid=bid;
            root->data[0]=key1->item_id;
            root->u.next3[0]=(struct bill_data_node*)malloc(sizeof(struct bill_data_node));
            root->u.next3[1]=(struct bill_data_node*)malloc(sizeof(struct bill_data_node));
            for(int j=0;j<=C;j++)
            {
                root->u.next3[0]->arr[j]=root->u.next3[1]->arr[j]=NULL;
            }
            root->u.next3[1]->arr[0]=key1;
            root->u.next3[1]->size=1;
            root->u.next3[0]->size=0;
            root->u.next3[0]->left=NULL;
            root->u.next3[0]->right=root->u.next3[1];
            root->u.next3[1]->left=root->u.next3[0];
            root->u.next3[1]->right=NULL;
        }
        else
        {
            root->flag=3;
            root->data[0]=key2->billid;
            root->u.next4[0]=(struct ultimatenode*)malloc(sizeof(struct ultimatenode));
            root->u.next4[1]=(struct ultimatenode*)malloc(sizeof(struct ultimatenode));
            for(int j=0;j<=C;j++)
            {
                root->u.next4[0]->arr[j]=root->u.next4[1]->arr[j]=NULL;
            }
            root->u.next4[1]->arr[0]=key2;
            root->u.next4[1]->size=1;
            root->u.next4[0]->size=0;
            root->u.next4[0]->left=NULL;
            root->u.next4[0]->right=root->u.next4[1];
            root->u.next4[1]->left=root->u.next4[0];
            root->u.next4[1]->right=NULL;
        }
    }
    return retval;
}
struct node*delete(struct node*root,int key,struct node*nptr,int index)
{
    struct node*retval=NULL;
    if(root)
    {
        int i=0;
        while(i<root->size&&root->data[i]<=key)i++;
        if(root->flag==0)
        {
            retval=delete(root->u.next1[i],key,root,i);
            if(nptr)
            {
             //   printf("\n\n%d\n\n",root->data[0]);
                if(root->size<M/2)//check for condition when neighbour have enough elements after deletion
                {
                //
                    if(index-1>=0&&(nptr->u.next1[index-1]->size-1)>=M/2)
                    {
                        for(int k=root->size;k>=1;k--)
                        {
                            root->data[k]=root->data[k-1];
                        }
                        for(int k=root->size+1;k>=1;k--)
                        {
                            root->u.next1[k]=root->u.next1[k-1];
                        }
                        root->u.next1[0]=nptr->u.next1[index-1]->u.next1[nptr->u.next1[index-1]->size];
                        nptr->u.next1[index-1]->u.next1[nptr->u.next1[index-1]->size]=NULL;
                        root->data[0]=nptr->data[index-1];
                        ++root->size;
                        nptr->data[index-1]=nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size-1];
                        --nptr->u.next1[index-1]->size;
                    }
                    else if((index+1<M)&&nptr->u.next1[index+1]&&(nptr->u.next1[index+1]->size-1>=M/2))
                    {
                        root->data[root->size]=nptr->data[index];
                        ++root->size;
                        root->u.next1[root->size]=nptr->u.next1[index+1]->u.next1[0];
                        nptr->data[index]=nptr->u.next1[index+1]->data[0];
                        for(int j=0;j<nptr->u.next1[index+1]->size-1;j++)
                        {
                            nptr->u.next1[index+1]->data[j]=nptr->u.next1[index+1]->data[j+1];
                        }
                        for(int j=0;j<nptr->u.next1[index+1]->size;j++)
                        {
                            nptr->u.next1[index+1]->u.next1[j]=nptr->u.next1[index+1]->u.next1[j+1];
                        }
                        --nptr->u.next1[index+1]->size;
                    }
                    else if(index-1>=0)
                    {
                    
                        nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size]=nptr->data[index-1];
                        ++nptr->u.next1[index-1]->size;
                        for(int j=0;j<root->size;j++)
                        {
                            nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size+j]=root->data[j];
                        }
                        for(int j=0;j<=root->size;j++)
                        {
                            nptr->u.next1[index-1]->u.next1[nptr->u.next1[index-1]->size+j]=root->u.next1[j];
                        }
                        nptr->u.next1[index-1]->size+=root->size;
                        for(int j=index;j<nptr->size;j++)
                        {
                            nptr->u.next1[j]=nptr->u.next1[j+1];
                        }
                        for(int j=index-1;j<nptr->size-1;j++)
                        {
                            nptr->data[j]=nptr->data[j+1];
                        }
                        --nptr->size;
                        free(root);
                        if(nptr->size==0)retval=nptr->u.next1[index-1];
                    }
                    else
                    {
                      //  printf("\n\n%d %d\n\n",root->data[0],root->size);
                        root->data[root->size]=nptr->data[index];
                        ++root->size;
                        for(int j=0;j<nptr->u.next1[index+1]->size;j++)
                        {
                            root->data[root->size+j]=nptr->u.next1[index+1]->data[j];
                        }
                        for(int j=0;j<=nptr->u.next1[index+1]->size;j++)
                        {
                            root->u.next1[root->size+j]=nptr->u.next1[index+1]->u.next1[j];
                        }
                        root->size+=nptr->u.next1[index+1]->size;
                        free(nptr->u.next1[index+1]);
                        for(int j=index+1;j<nptr->size;j++)
                        {
                            nptr->u.next1[j]=nptr->u.next1[j+1];
                        }
                        for(int j=index;j<nptr->size-1;j++)
                        {
                            nptr->data[j]=nptr->data[j+1];
                        }
                        --nptr->size;
                        if(nptr->size==0)retval=root;
                    }
                }
            }
        }
        else
        {
          //  printf("l");
            int j=0;
            while(j<root->u.next2[i]->size&&root->u.next2[i]->data[j]->item_id<key)j++;
            if(root->u.next2[i]->data[j]->item_id==key)
            {
                for(int k=j;k<root->u.next2[i]->size-1;k++)
                {
                    root->u.next2[i]->data[k]=root->u.next2[i]->data[k+1];
                }
                --root->u.next2[i]->size;
                if(root->u.next2[i]->size<(C+1)/2)
                {
                    if(i-1>=0&&(root->u.next2[i-1]->size-1>=(C+1)/2))
                    {
                        for(int k=root->u.next2[i]->size;k>=1;k--)
                        {
                            root->u.next2[i]->data[k]=root->u.next2[i]->data[k-1];
                        }
                        root->u.next2[i]->data[0]=root->u.next2[i-1]->data[root->u.next2[i-1]->size-1];
                        root->data[i-1]=root->u.next2[i]->data[0]->item_id;
                        ++root->u.next2[i]->size;
                        --root->u.next2[i-1]->size;
                    }
                    else if(i+1<=root->size&&(root->u.next2[i+1]->size-1>=(C+1)/2))
                    {
                        root->u.next2[i]->data[root->u.next2[i]->size]=root->u.next2[i+1]->data[0];
                        for(int k=0;k<root->u.next2[i+1]->size-1;k++)
                        {
                            root->u.next2[i+1]->data[k]=root->u.next2[i+1]->data[k+1];
                        }
                        root->data[i]=root->u.next2[i+1]->data[0]->item_id;
                        ++root->u.next2[i]->size;
                        --root->u.next2[i+1]->size;
                    }
                    else
                    {
                        if(i-1>=0)
                        {
                            for(int k=0;k<root->u.next2[i]->size;k++)
                            {
                                root->u.next2[i-1]->data[root->u.next2[i-1]->size+k]=root->u.next2[i]->data[k];
                            }
                            root->u.next2[i-1]->size+=root->u.next2[i]->size;
                            root->u.next2[i-1]->right=root->u.next2[i]->right;
                            if(root->u.next2[i-1]->right)root->u.next2[i-1]->right->left=root->u.next2[i-1];
                            for(int k=i-1;k<root->size-1;k++)
                            {
                                root->data[k]=root->data[k+1];
                            }
                            for(int k=i;k<root->size;k++)
                            {
                                root->u.next2[k]=root->u.next2[k+1];
                            }
                            --root->size;
                            
                            if(root->size<M/2)
                            {
                                if(index-1>=0&&(nptr->u.next1[index-1]->size-1)>=M/2)
                                {
                                    for(int k=root->size;k>=1;k--)
                                    {
                                        root->data[k]=root->data[k-1];
                                    }
                                    for(int k=root->size+1;k>=1;k--)
                                    {
                                        root->u.next2[k]=root->u.next2[k-1];
                                    }
                                    root->u.next2[0]=nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size];
                                    nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size]=NULL;
                                    root->data[0]=nptr->data[index-1];
                                    ++root->size;
                                    nptr->data[index-1]=nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size-1];
                                    --nptr->u.next1[index-1]->size;
                                }
                                else if((index+1<M)&&nptr->u.next1[index+1]&&(nptr->u.next1[index+1]->size-1>=M/2))
                                {
                                   /* root->data[root->size]=nptr->data[index];
                                    ++root->size;
                                    nptr->data[index]=nptr->u.next1[index+1]->data[0];
                                    for(int j=0;j<nptr->u.next1[index+1]->size-1;j++)
                                    {
                                        nptr->u.next1[index+1]->data[j]=nptr->u.next1[index+1]->data[j+1];
                                    }
                                    --nptr->u.next1[index+1]->size;*/
                                    root->data[root->size]=nptr->data[index];
                                    ++root->size;
                                    root->u.next2[root->size]=nptr->u.next1[index+1]->u.next2[0];
                                    nptr->data[index]=nptr->u.next1[index+1]->data[0];
                                    for(int j=0;j<nptr->u.next1[index+1]->size-1;j++)
                                    {
                                        nptr->u.next1[index+1]->data[j]=nptr->u.next1[index+1]->data[j+1];
                                    }
                                    for(int j=0;j<nptr->u.next1[index+1]->size;j++)
                                    {
                                        nptr->u.next1[index+1]->u.next2[j]=nptr->u.next1[index+1]->u.next2[j+1];
                                    }
                                    --nptr->u.next1[index+1]->size;
                                }
                                else
                                {
                                    if(index-1>=0)
                                    {
                                     //   root->data[i]=nptr->data[index-1];
                                        nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size]=nptr->data[index-1];
                                        ++nptr->u.next1[index-1]->size;
                                        for(int j=0;j<root->size;j++)
                                        {
                                            nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size+j]=root->data[j];
                                        }
                                        for(int j=0;j<=root->size;j++)
                                        {
                                            nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size+j]=root->u.next2[j];
                                        }
                                  //      printf("%d",nptr->u.next1[index-1]->size);
                                        nptr->u.next1[index-1]->size+=root->size;
                                        for(int j=index;j<nptr->size;j++)
                                        {
                                            nptr->u.next1[j]=nptr->u.next1[j+1];
                                        }
                                        for(int j=index-1;j<nptr->size-1;j++)
                                        {
                                            nptr->data[j]=nptr->data[j+1];
                                        }
                                        --nptr->size;
                                        free(root);
                                        if(nptr->size==0)retval=nptr->u.next1[index-1];
                                    }
                                    else
                                    {

                                    //    printf("\n\n%d\n\n",nptr->data[index]);
                                        root->data[root->size]=nptr->data[index];
                                        ++root->size;
                                        for(int j=0;j<nptr->u.next1[index+1]->size;j++)
                                        {
                                            root->data[root->size+j]=nptr->u.next1[index+1]->data[j];
                                        }
                                        for(int j=0;j<=nptr->u.next1[index+1]->size;j++)
                                        {
                                            root->u.next2[root->size+j]=nptr->u.next1[index+1]->u.next2[j];
                                        }
                                        root->size+=nptr->u.next1[index+1]->size;
                                        free(nptr->u.next1[index+1]);
                                        for(int j=index+1;j<nptr->size;j++)
                                        {
                                            nptr->u.next1[j]=nptr->u.next1[j+1];
                                        }
                                        for(int j=index;j<nptr->size-1;j++)
                                        {
                                            nptr->data[j]=nptr->data[j+1];
                                        }
                                        --nptr->size;
                                        if(nptr->size==0)retval=root;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int k=0;k<root->u.next2[i+1]->size;k++)
                            {
                                root->u.next2[i]->data[root->u.next2[i]->size+k]=root->u.next2[i+1]->data[k];
                            }
                            root->u.next2[i]->right=root->u.next2[i+1]->right;
                            root->u.next2[i]->size+=root->u.next2[i+1]->size;
                            if(root->u.next2[i]->right)root->u.next2[i]->right->left=root->u.next2[i];
                            for(int k=i;k<root->size-1;k++)
                            {
                                root->data[k]=root->data[k+1];
                            }
                            for(int k=i+1;k<root->size;k++)
                            {
                                root->u.next2[k]=root->u.next2[k+1];
                            }
                            --root->size;
                            if(root->size<M/2)
                            {
                                if(index-1>=0&&(nptr->u.next1[index-1]->size-1)>=M/2)
                                {
                                    for(int k=root->size;k>=1;k--)
                                    {
                                        root->data[k]=root->data[k-1];
                                    }
                                    for(int k=root->size+1;k>=1;k--)
                                    {
                                        root->u.next2[k]=root->u.next2[k-1];
                                    }
                                    root->u.next2[0]=nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size];
                                    nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size]=NULL;
                                    root->data[0]=nptr->data[index-1];
                                    ++root->size;
                                    nptr->data[index-1]=nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size-1];
                                    --nptr->u.next1[index-1]->size;
                                }
                                else if((index+1<M)&&nptr->u.next1[index+1]&&(nptr->u.next1[index+1]->size-1>=M/2))
                                {
                                    root->data[root->size]=nptr->data[index];
                                    ++root->size;
                                    nptr->data[index]=nptr->u.next1[index+1]->data[0];
                                    for(int j=0;j<nptr->u.next1[index+1]->size-1;j++)
                                    {
                                        nptr->u.next1[index+1]->data[j]=nptr->u.next1[index+1]->data[j+1];
                                    }
                                    --nptr->u.next1[index+1]->size;
                                }
                                else
                                {
                                    if(index-1>=0)
                                    {
                                     // root->data[i]=nptr->data[index-1];
                                        nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size]=nptr->data[index-1];
                                        ++nptr->u.next1[index-1]->size;
                                        for(int j=0;j<root->size;j++)
                                        {
                                            nptr->u.next1[index-1]->data[nptr->u.next1[index-1]->size+j]=root->data[j];
                                        }
                                        for(int j=0;j<=root->size;j++)
                                        {
                                            nptr->u.next1[index-1]->u.next2[nptr->u.next1[index-1]->size+j]=root->u.next2[j];
                                        }
                                        nptr->u.next1[index-1]->size+=root->size;
                                        for(int j=index;j<nptr->size;j++)
                                        {
                                            nptr->u.next1[j]=nptr->u.next1[j+1];
                                        }
                                        for(int j=index-1;j<nptr->size-1;j++)
                                        {
                                            nptr->data[j]=nptr->data[j+1];
                                        }
                                        --nptr->size;
                                        free(root);
                                        if(nptr->size==0)retval=nptr->u.next1[index-1];
                                    }
                                    else
                                    {

                                    //    printf("\n\n%d\n\n",nptr->data[index]);
                                        root->data[root->size]=nptr->data[index];
                                        ++root->size;
                                        for(int j=0;j<nptr->u.next1[index+1]->size;j++)
                                        {
                                            root->data[root->size+j]=nptr->u.next1[index+1]->data[j];
                                        }
                                        for(int j=0;j<=nptr->u.next1[index+1]->size;j++)
                                        {
                                            root->u.next2[root->size+j]=nptr->u.next1[index+1]->u.next2[j];
                                        }
                                        root->size+=nptr->u.next1[index+1]->size;
                                        free(nptr->u.next1[index+1]);
                                        for(int j=index+1;j<nptr->size;j++)
                                        {
                                            nptr->u.next1[j]=nptr->u.next1[j+1];
                                        }
                                        for(int j=index;j<nptr->size-1;j++)
                                        {
                                            nptr->data[j]=nptr->data[j+1];
                                        }
                                        --nptr->size;
                                        if(nptr->size==0)retval=root;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retval;
}
void AddItem(struct node**arr,FILE*fp,int flag)
{
    unsigned int quantity,aisle_no,price,threshold,d,m,y;
    char item_name[100];
    printf("Enter Name of Item to be added : ");
    if(flag==0)
    {
        fscanf(fp,"%s",item_name);
    }
    else scanf("%s",item_name);
    printf("Enter Quantity of Item : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&quantity);
    }
    else scanf("%u",&quantity);
    printf("Enter Aisle_no : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&aisle_no);
    }
    else scanf("%u",&aisle_no);
    printf("Enter Price : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&price);
    }
    else scanf("%u",&price);
    printf("Enter Threshold : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&threshold);
    }
    else
    {
        scanf("%u",&threshold);
    }
    printf("Enter Expiry date :\nDD : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&d);
    }
    else
    {
        scanf("%u",&d);
    }
    printf("mm : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&m);
    }
    else scanf("%u",&m);
    printf("yyyy : ");
    if(flag==0)
    {
        fscanf(fp,"%u",&y);
    }
    else scanf("%u",&y);
    struct aisle_node*n=createAislenode(quantity,aisle_no,price,item_name,threshold,d,m,y);
    struct node*n1=insert(arr[aisle_no],n,NULL,NULL,NULL,-1,0);
    if(n1)arr[aisle_no]=n1;
}
void updateItem(struct node**arr)
{
    int itno;
    printf("Enter Item_id to be updated : ");
    scanf("%d",&itno);
    struct aisle_node*nptr=NULL;
    for(int i=0;i<aislecnt&&nptr==NULL;i++)
    {
        nptr=search(arr[i],itno);
    }
    if(nptr)
    {
        printf("Enter Quantity of Item : ");
        scanf("%u",&nptr->quantity);
        printf("Enter Price : ");
        scanf("%u",&nptr->price);
        printf("Enter Threshold : ");
        scanf("%u",&nptr->threshold);
        printf("Enter Expiry date :\nDD : ");
        scanf("%u",&nptr->u.dd);
        printf("mm : ");
        scanf("%u",&nptr->u.mm);
        printf("yyyy : ");
        scanf("%u",&nptr->u.yyyy);
        printf("Item updated successfully\n");
    }
    else 
        printf("Itemid not found\n");
}
void DeleteItem(struct node**arr,unsigned int itemid)
{
    int flag=0;
    for(int i=0;i<aislecnt&&flag==0;i++)
    {
        if(arr[i])
        {
            unsigned int lar,small;
            struct node*nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[nptr->size];
            }
            lar=nptr->u.next2[nptr->size]->data[nptr->u.next2[nptr->size]->size-1]->item_id;
            nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[0];
            }
            if(nptr->u.next2[0]->size!=0)
            {
                small=nptr->u.next2[0]->data[0]->item_id;
            }
            else
            {
                small=nptr->u.next2[1]->data[0]->item_id;
            }
        //    printf("%d %d\n",small,lar);
            if(itemid>=small&&itemid<=lar)
            {
               // printf("aa");
                struct node*n1=delete(arr[i],itemid,NULL,-1);
                if(n1)arr[i]=n1;
                flag=1;
            }
        }
    }
}
void addupdateAisle(FILE*fp,struct node**arr,unsigned int n,char*aislename[n],int flag)
{
    bool ans=0;
    printf("Enter 0 to update name of Aisle or 1 to add new aisle : ");
    if(flag!=0)
    {
        scanf("%d",&ans);
    }
    else fscanf(fp,"%d",&ans);
    if(ans==0)
    {
        unsigned int asid;
        printf("Enter id of Aisle to be renamed : ");
        scanf("%u",asid);
        if(asid<aislecnt)
        {
            printf("Enter new name :");
            scanf("%s",aislename[asid]);
            printf("Aisle renamed successfully\n");
        }
        else 
            printf("Invalid Aisle-id");
    }
    else
    {
        ++aislecnt;
        printf("Enter name of Aisle : ");
        aislename[aislecnt-1]=(char*)malloc(100*sizeof(char));
        if(flag!=0)
        {
            scanf("%s",aislename[aislecnt-1]);
        }
        else fscanf(fp,"%s",aislename[aislecnt-1]);
        printf("Aisle_id : %u\nAisle Name :%s successfully added\n",aislecnt-1,aislename[aislecnt-1]);
    }
}
void deleteroot(struct node*root,int flag)
{
    if(root)
    {
        
        if(root->flag==0)
        {
            for(int i=0;i<root->size;i++)
            {
                
                deleteroot(root->u.next1[i],flag);
                free(root->u.next1[i]);
                root->u.next1[i]=NULL;
            }
        }
        else
        {
            if(flag==0)
            {
                for(int i=0;i<root->size;i++)
                {
                  //  printf("t");
                    struct datanode*nptr=root->u.next2[i];
                    for(int j=0;j<nptr->size;j++)
                    {
                        free(nptr->data[j]->item_name);
                        free(nptr->data[j]);
                    }
                    free(root->u.next2[i]);
                    root->u.next2[i]=NULL;
                }
            }
            else if(flag==1)
            {
                for(int i=0;i<root->size;i++)
                {
                    struct bill_data_node*nptr=root->u.next3[i];
                    for(int j=0;j<nptr->size;j++)
                    {
                        free(nptr->arr[j]->item_name);
                        free(nptr->arr[i]);
                    }
                    free(root->u.next3[i]);
                    root->u.next3[i]=NULL;
                }
            }
            else
            {
                for(int i=0;i<root->size;i++)
                {
                    struct ultimatenode*nptr=root->u.next4[i];
                    for(int j=0;j<nptr->size;j++)
                    {
                        deleteroot(nptr->arr[j],1);
                        nptr->arr[j]=NULL;
                    }
                    free(root->u.next4[i]);
                    root->u.next4[i]=NULL;
                }
            }
        }
        free(root);
    }
}
void deleteAisle(struct node**arr,int Aisleid,unsigned int n,char*aislename[n])
{
    if(Aisleid<aislecnt)
    {
        struct node*nptr=arr[Aisleid];
        if(nptr)
        {
            deleteroot(arr[Aisleid],0);
            arr[Aisleid]=NULL;
        }
    }
    else 
        printf("Aisle-id is beyond limit\n");
}
bool isavailableitem(struct node**arr)
{
    bool retval=1,flag=0;
    unsigned int id,quantity;
    struct date d;
    printf("Enter ID : ");
    scanf("%u",&id);
    printf("Enter quantity : ");
    scanf("%u",&quantity);
    printf("Enter current date :\nDD : ");
    scanf("%u",&d.dd);
    printf("mm : ");
    scanf("%u",&d.mm);
    printf("yyyy : ");
    scanf("%u",&d.yyyy);
    for(int i=0;i<aislecnt&&retval;i++)
    {
        if(arr[i])
        {
            unsigned int lar,small;
            struct node*nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[nptr->size];
            }
            lar=nptr->u.next2[nptr->size]->data[nptr->u.next2[nptr->size]->size-1]->item_id;
            nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[0];
            }
            if(nptr->u.next2[0]->size)
            {
                small=nptr->u.next2[0]->data[0]->item_id;
            }
            else
            {
                small=nptr->u.next2[1]->data[0]->item_id;
            }
            if(id>=small&&id<=lar)
            {
                flag=1;
                struct aisle_node*n1=search(arr[i],id);
                if(n1)
                {
                    if(n1->quantity>=quantity)
                    {
                        if(d.yyyy>n1->u.yyyy||(d.yyyy==n1->u.yyyy&&d.mm>n1->u.mm)||(d.yyyy==n1->u.yyyy&&d.mm==n1->u.mm&&d.dd>n1->u.dd))
                        {
                            retval=0;
                        }
                    }
                    else retval=0;
                }
            }   
        }
    }
    if(flag==0)
    {
        retval=0;
    }
    return retval;
}
void beyondthreshold(struct node**arr)
{
    for(int i=0;i<aislecnt;i++)
    {
        if(arr[i])
        {
            struct node*nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[0];
            }
            struct datanode*nptr1=nptr->u.next2[0];
            while(nptr1)
            {
                for(int j=0;j<nptr1->size;j++)
                {
                    if(nptr1->data[j]->quantity<nptr1->data[j]->threshold)
                    {
                        printf("Item id : %u\nItem name : %s\nQuantity : %u\nThreshold : %u\n\n",nptr1->data[j]->item_id,nptr1->data[j]->item_name,nptr1->data[j]->quantity,nptr1->data[j]->threshold);
                    }
                }
                nptr1=nptr1->right;
            }
        }
    }
}
void readytoeat(struct node**arr1,unsigned int size)
{
    int arr[size],n,flag=0,i;
    struct date d;
    printf("Enter aisle number for search (which consist of ready-to-eat items): (enter -1 to stop input)\n");
    for(i=0;flag==0;i++)
    {
        int temp;
        scanf("%d",&temp);
        if(temp==-1)
        {
            flag=1;
            i--;
        }
        else arr[i]=temp;
    }
    printf("Enter current date :\nDD : ");
    scanf("%u",&d.dd);
    printf("mm : ");
    scanf("%u",&d.mm);
    printf("yyyy : ");
    scanf("%u",&d.yyyy);
    for(int j=0;j<i;j++)
    {
        struct node*nptr=arr1[arr[j]];
        printf("Aisle no :%d\n",arr[j]);
        while(nptr->flag==0)
        {
            nptr=nptr->u.next1[0];
        }
        struct datanode*nptr1=nptr->u.next2[0];
        while(nptr1)
        {
            for(int j=0;j<nptr1->size;j++)
            {
                if(nptr1->data[j]->u.yyyy==d.yyyy)
                {
                    if(nptr1->data[j]->u.mm==d.mm)
                    {
                        if(nptr1->data[j]->u.dd-d.dd<=7&&nptr1->data[j]->u.dd>=d.dd)
                        {
                            printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,nptr1->data[j]->u.dd-d.dd);
                        }
                    }
                    else if(nptr1->data[j]->u.mm==d.mm+1)
                    {
                        if(d.mm==1||d.mm==3||d.mm==5||d.mm==7||d.mm==8||d.mm==10||d.mm==12)
                        {
                            if(31-d.dd+nptr1->data[j]->u.dd<=7)
                            {
                                printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,31-d.dd+nptr1->data[j]->u.dd);
                            }
                        }
                        else if(d.mm!=2)
                        {
                            if(30-d.dd+nptr1->data[j]->u.dd<=7)
                            {
                                printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,30-d.dd+nptr1->data[j]->u.dd);
                            }
                        }
                        else
                        {
                            if((d.yyyy%100!=0&&d.yyyy%4==0)||(d.yyyy%100==0&&d.yyyy%400==0))
                            {
                                if(29-d.dd+nptr1->data[j]->u.dd<=7)
                                {
                                    printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,29-d.dd+nptr1->data[j]->u.dd);
                                }
                            }
                            else
                            {
                                if(28-d.dd+nptr1->data[j]->u.dd<=7)
                                {
                                    printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,28-d.dd+nptr1->data[j]->u.dd);
                                }
                            }
                        }
                    }
                }
                else if(nptr1->data[j]->u.yyyy=d.yyyy+1)
                {
                    if(nptr1->data[j]->u.mm==1&&d.mm==12)
                    {
                        if(31-d.dd+nptr1->data[j]->u.dd<=7)
                        {
                            printf("ID no.%u :getting expired in %u days\n",nptr1->data[j]->item_id,31-d.dd+nptr1->data[j]->u.dd);
                        }
                    }
                }
            }
            nptr1=nptr1->right;
        }
    }
}
void dairyexpiry(struct node**arr1)
{
    struct date d;
    printf("Enter current date :\nDD : ");
    scanf("%u",&d.dd);
    printf("mm : ");
    scanf("%u",&d.mm);
    printf("yyyy : ");
    scanf("%u",&d.yyyy);
    struct node*nptr=arr1[0];
    while(nptr->flag==0)
    {
        nptr=nptr->u.next1[0];
    }
    struct datanode*nptr1=nptr->u.next2[0];
    while(nptr1)
    {
        for(int i=0;i<nptr1->size;i++)
        {
            if(nptr1->data[i]->u.yyyy==d.yyyy)
            {
                if(nptr1->data[i]->u.mm==d.mm)
                {
                    if(nptr1->data[i]->u.dd-d.dd<=1&&nptr1->data[i]->u.dd>=d.dd)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,nptr1->data[i]->u.dd-d.dd);
                    }
                }
                else if(d.mm==1||d.mm==3||d.mm==5||d.mm==7||d.mm==8||d.mm==10||d.mm==12)
                {
                    if(d.dd==31&&nptr1->data[i]->u.dd==1)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,1);
                    }
                }
                else if(d.mm!=2)
                {
                    if(d.dd==30&&nptr1->data[i]->u.dd==1)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,1);
                    }
                }
                else
                {
                    if((d.yyyy%100!=0&&d.yyyy%4==0)||(d.yyyy%100==0&&d.yyyy%400==0))
                    {
                        if(d.dd==29&&nptr1->data[i]->u.dd==1)
                        {
                            printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,1);
                        }
                    }
                    else
                    {
                        if(d.dd=28&&nptr1->data[i]->u.dd==1)
                        {
                            printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,1);
                        }
                    }
                }
            }
            else if(nptr1->data[i]->u.yyyy==d.yyyy+1)
            {
                if(nptr1->data[i]->u.mm==1&&d.mm==12)
                {
                    if(nptr1->data[i]->u.dd==1&&d.dd==31)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr1->data[i]->item_id,1);
                    }
                }
            }
        }
        nptr1=nptr1->right;
    }
}
struct aisle_node**merge(struct datanode*n1,struct datanode*n2,int*size)
{
    struct datanode*nptr=n1;
    int cnt=0,size1=0,size2=0;
    while(nptr)
    {
        for(int i=0;i<nptr->size;i++)
        {
            ++cnt;
        }
        nptr=nptr->right;
    }
    size1=cnt;
    cnt=0;
    nptr=n2;
    while(nptr)
    {
        for(int i=0;i<nptr->size;i++)
        {
            ++cnt;
        }
        nptr=nptr->right;
    }
    size2=cnt;
    struct aisle_node**arr=(struct aisle_node**)malloc((size1+size2)*sizeof(struct aisle_node*));
    struct aisle_node**arr1=(struct aisle_node**)malloc(size1*sizeof(struct aisle_node*));
    struct aisle_node**arr2=(struct aisle_node**)malloc(size2*sizeof(struct aisle_node*));
    nptr=n1;
    cnt=0;
    while(nptr)
    {
        for(int i=0;i<nptr->size;i++)
        {
            arr1[cnt]=nptr->data[i];
            ++cnt;
        }
        nptr=nptr->right;
    }
    nptr=n2;
    cnt=0;
    while(nptr)
    {
        for(int i=0;i<nptr->size;i++)
        {
            arr2[cnt]=nptr->data[i];
            ++cnt;
        }
        nptr=nptr->right;
    }
    int i1=0,i2=0;
    while(i1<size1&&i2<size2)
    {
        if(arr1[i1]->item_id<arr2[i2]->item_id)
        {
            arr[i1+i2]=arr1[i1];
            i1++;
        }
        else
        {
            arr[i1+i2]=arr2[i2];
            i2++;
        }
    }
    while(i1<size1)
    {
        arr[i1+i2]=arr1[i1];
        i1++;
    }
    while(i2<size2)
    {
        arr[i1+i2]=arr2[i2];
        i2++;
    }
    *size=size1+size2;
    return arr;
}
struct node*construct(int size,struct aisle_node*arr[size],struct node*arr1[size])
{
    struct node**retval=NULL,*retval1=NULL;
    if(arr)
    {
        int temp=(C+1)/2;
        int num1=size/temp,num2=0;
        num2=size%temp+temp;
        struct datanode**arr2=(struct datanode**)malloc(num1*sizeof(struct datanode*));
        int cnt=0;
        for(int i=0;i<(num1-1)*temp;i++)
        {
            struct datanode*dptr=(struct datanode*)malloc(sizeof(struct datanode));
            int j=0;
            while(j<temp)
            {
                dptr->data[j]=arr[j+i];
                j++;
            }
            dptr->size=temp;
            arr2[cnt]=dptr;
            if(cnt>=1)
            {
                dptr->left=arr2[cnt-1];
                arr2[cnt-1]->right=dptr;
            }
            else
            {
                dptr->left=NULL;
            }
            ++cnt;
            dptr->right=NULL;
            i+=j-1;
          //  printf("%d",cnt);
        }
        struct datanode*dptr=(struct datanode*)malloc(sizeof(struct datanode));
        int j=0,i=temp*(num1-1);
        while(j<num2)
        {
            dptr->data[j]=arr[i+j];
            j++;
        }
        dptr->size=num2;
        if(cnt>=1)
        {
            dptr->left=arr2[cnt-1];
            arr2[cnt-1]->right=dptr;
        }
        else
        {
            dptr->left=NULL;
        }
        dptr->right=NULL;
        arr2[cnt]=dptr;
        ++cnt;
        i+=num2;
        int temp2=(M+1)/2;
        int num3=num1/temp2,num4=0;
        num4=temp2+num1%temp2;
        retval=(struct node**)malloc(num4*sizeof(struct node*));
        int count=0;
        for(int i=0;i<num3-1;i++)
        {
            struct node*val=(struct node*)malloc(sizeof(struct node));
            j=temp2*i;
            int cnt1=0;
            while(cnt1<temp2)
            {
                val->u.next2[cnt1]=arr2[j+cnt1];
                if(cnt1!=0)
                {
                    val->data[cnt1-1]=arr2[j+cnt1]->data[0]->item_id;
                }
                ++cnt1;
            }
            val->size=temp2-1;
            val->flag=1;
            retval[count]=val;
            ++count;
        }
        struct node*val=(struct node*)malloc(sizeof(struct node));
        val->size=num4-1;
        val->flag=1;
        i=(num3-1)*temp2,j=0;
    //    printf("a");
    //    printf("%d",i);
        while(j<num4)
        {
            val->u.next2[j]=arr2[i+j];
         //   printf("%d\n",arr2[i+j]->data[0]->item_id);
            if(j!=0)
            {
                val->data[j-1]=arr2[j+i]->data[0]->item_id;
            }
            j++;
        }
      //  printf("a");
        retval[count]=val;
        ++count;
      // printf("a");
        if(count>1)
        {
         //           printf("a");
      //   printf("%d",count);
            retval1=construct(count,NULL,retval);
        }
        else
        {
            retval1=retval[0];
        }
        free(arr);
        free(arr2);
    }
    else
    {
     //   printf("%d",arr1[1]->data[1]);
        int temp1=(M+1)/2;
        int num1=size/temp1;
        int num2=temp1+size%temp1;
        retval=(struct node**)malloc(num1*sizeof(struct node*));
        int count=0;
        for(int i=0;i<num1-1;i++)
        {
            struct node*val=(struct node*)malloc(sizeof(struct node));
            val->size=temp1-1;
            val->flag=0;
            int j=temp1*i;
            int cnt1=0;
            while(cnt1<temp1)
            {
                val->u.next1[cnt1]=arr1[j+cnt1];
                if(cnt1!=0)
                {
                    val->data[cnt1-1]=arr1[j+cnt1]->data[0];
                }
                ++cnt1;
            }
            retval[count]=val;
            ++count;
        }
        struct node*val=(struct node*)malloc(sizeof(struct node));
        val->size=num2-1;
        val->flag=0;
        int i=(num1-1)*temp1,j=0;
        while(j<num2)
        {
            val->u.next1[j]=arr1[j+i];
            if(j!=0)
            {
                struct node*t=arr1[i+j];
                while(t->flag==0)
                {
                    t=t->u.next1[0];
                }
                val->data[j-1]=t->u.next2[0]->data[0]->item_id;
            }
            j++;
        }
        retval[count]=val;
        count++;
        if(count>1)
        {
            retval1=construct(count,NULL,retval);
        }
        else retval1=retval[0];
        free(arr1);
    }
    return retval1;
}
void mergeaisle(struct node**arr1,unsigned int size,char*aislename[size])
{
    unsigned int n1,n2;
    printf("Enter two aisle no. to be merged : ");
    scanf("%u %u",&n1,&n2);
    
    if(n1<aislecnt&&n2<aislecnt&&n1!=n2)
    {
        if(arr1[n1]&&arr1[n2])
        {
            struct node*nptr1=arr1[n1],*nptr2=arr1[n2];
            while(nptr1->flag==0)
            {
                nptr1=nptr1->u.next1[0];
            }
            while(nptr2->flag==0)
            {
                nptr2=nptr2->u.next1[0];
            }
            int size1=0;
            struct aisle_node**arr=merge(nptr1->u.next2[0],nptr2->u.next2[0],&size1);
          /*  for(int i=0;i<size;i++)
            {
                printf("%d\n",arr[i]->item_id);
            }*/
            struct node*retval=construct(size1,arr,NULL);
            if(n1<n2)
            {
                arr1[n2]=NULL;
                arr1[n1]=retval;
                if(aislename[n2])
                {
                    free(aislename[n2]);
                    aislename[n2]=NULL;
                }
                printf("Aisle no.%d and Aisle no.%d merged successfully in aisle no.%d\n",n1,n2,n1);
                printf("Enter the updated name of merged aisle : ");
                scanf("%s",aislename[n1]);
            }
            else
            {
                arr1[n1]=NULL;
                arr1[n2]=retval;
                if(aislename[n1])
                {
                    free(aislename[n1]);
                    aislename[n1]=NULL;
                }
                printf("Aisle no.%d and Aisle no.%d merged successfully in aisle no.%d\n",n2,n1,n2);
                printf("Enter the updated name of merged aisle : ");
                scanf("%s",aislename[n2]);
            }
            struct node*ptr=retval;
            while(ptr->flag==0)
            {
                ptr=ptr->u.next1[0];
            }
            struct datanode*ptr2=ptr->u.next2[0];
            int low=(n1<n2)?n1:n2;
            while(ptr2)
            {
                for(int i=0;i<ptr2->size;i++)
                {
                    ptr2->data[i]->aisle_no=low;
                }
                ptr2=ptr2->right;
            }
        }
    }
}
void updatedatamine(int size,unsigned int*arr[size],struct node*bill)
{
    struct node*nptr=bill;
    while(nptr&&nptr->flag!=2)
    {
        nptr=nptr->u.next1[0];
    }
    struct bill_data_node*nptr1=nptr->u.next3[0],*prev=NULL;
    while(nptr1)
    {
        for(int i=0;i<nptr1->size;i++)
        {
            struct bill_data_node*nptr2=nptr1;
            while(nptr2)
            {
                for(int j=(nptr1==nptr2)?i:0;j<nptr2->size;j++)
                {
                //    printf("%d %d\n",nptr1->arr[i]->item_id,nptr2->arr[j]->item_id);
                    ++arr[nptr1->arr[i]->item_id][nptr2->arr[j]->item_id];
                    if(nptr1->arr[i]->item_id!=nptr2->arr[j]->item_id)
                    {
                        ++arr[nptr2->arr[j]->item_id][nptr1->arr[i]->item_id];
                    }
                }
                nptr2=nptr2->right;
            }
        }
        nptr1=nptr1->right;
    }
    //continue
}
void associateinfo(unsigned int n,unsigned int*datamine[n])
{
    unsigned int itemid;
    printf("Enter ID : ");
    scanf("%u",&itemid);
    if(itemid<idgen)
    {
        for(unsigned int i=0;i<n;i++)
        {
            if(datamine[itemid][i]>0)
            {
                printf("Item-id no.%u : %u times\n",i,datamine[itemid][i]);
            }
        }
    }
    else printf("Invalid ID");
}
void RangeSearch(struct node**arr)
{
    int low,high,aisleno;;
    printf("Enter aisle number : ");
    scanf("%d",&aisleno);
    if(aisleno<aislecnt&&arr[aisleno])
    {
        printf("Enter lower limit of range search : ");
        scanf("%d",&low);
        printf("Enter upper limit of range search : ");
        scanf("%d",&high);
        if(low<=high)
        {
            struct node*nptr=arr[aisleno];
            while(nptr->flag==0)
            {
                int i=0;
                while(nptr&&nptr->data[i]<=low)
                {
                    i++;
                }
                nptr=nptr->u.next1[i];
            }
            int i=0;
            while(nptr&&nptr->data[i]<=low)
            {
                i++;
            }
            struct datanode*dptr=nptr->u.next2[i];
            int j=0;
            while(dptr&&j<dptr->size&&dptr->data[j]->item_id<low)
            {
                j++;
            }
            int flag=0;
            while(flag==0)
            {
                if(dptr==NULL)flag=1;
                while(flag==0&&j<dptr->size)
                {
                    if(dptr->data[j]->item_id<=high)
                    {
                        printf("Aisle no : %u\n",dptr->data[j]->aisle_no);
                        printf("Item id : %u\n",dptr->data[j]->item_id);
                        printf("Item Name : %s\n",dptr->data[j]->item_name);
                        printf("Price : %u\n",dptr->data[j]->price);
                        printf("Quantity : %u\n\n",dptr->data[j]->quantity);
                    }
                    else flag=1;
                    j++;
                }
                if(dptr)
                {
                    dptr=dptr->right;
                }
                j=0;
            }
        }
    }
}
int totalprice(struct node*root)
{
    int retval=0;
    struct node*nptr=root;
    while(nptr&&nptr->flag!=2)
    {
        nptr=nptr->u.next1[0];
    }
    struct bill_data_node*bptr=nptr->u.next3[0];
    while(bptr)
    {
        for(int i=0;i<bptr->size;i++)
        {
            retval+=bptr->arr[i]->price*bptr->arr[i]->quantity;
        }
        bptr=bptr->right;
    }
    return retval;
}
void displayItems(struct node**arr)
{
    for(int i=0;i<aislecnt;i++)
    {
        if(arr[i])
        {
            printf("Aisle no.%d : \n",i);
            struct node*nptr=arr[i];
            while(nptr->flag==0)
            {
                nptr=nptr->u.next1[0];
            }
            struct datanode*dptr=nptr->u.next2[0];
            while(dptr)
            {
                for(int j=0;j<dptr->size;j++)
                {
                    printf("Item-id : %u\nItem-name : %s\nQuantity : %u\n\n",dptr->data[j]->item_id,dptr->data[j]->item_name,dptr->data[j]->quantity);
                }
                dptr=dptr->right;
            }
        }
    }
}
struct node*BuyandMakeBill(struct node**arr,struct node*arr1,unsigned int n,unsigned int*datamine[n])
{
    struct node*bill=NULL;
    int size=0;
    char ch='1';
    while(ch=='1')
    {
        fflush(stdin);
        printf("Press 1 to buy from store or 0 to exit the system : ");
        scanf("%c",&ch);
        if(ch=='1')
        {
            ++size;
            printf("Following are list of items in store as per item-id :\n");
            displayItems(arr);
            printf("Enter Item-id to be purchased : ");
            unsigned int itid,qty;
            scanf("%u",&itid);
            printf("Enter quantity of item : ");
            scanf("%u",&qty);
            int flag=0;
            for(int i=0;i<aislecnt&&flag==0;i++)
            {
                if(arr[i])
                {
                    struct aisle_node*nptr=search(arr[i],itid);//look for seach in given range of min and max by going extreme left and extreme right
                    if(nptr)
                    {
                        flag=1;
                        if(nptr->quantity<qty)
                        {
                            printf("Insufficient stock\n");
                        }
                        else
                        {
                            nptr->quantity-=qty;
                            struct bill_node*temp=(struct bill_node*)malloc(sizeof(struct bill_node));
                            temp->item_id=itid;
                            temp->quantity=qty;
                            temp->item_name=nptr->item_name;
                            temp->price=nptr->price;
                            struct node*te=insert(bill,NULL,temp,NULL,NULL,-1,1);
                            if(te)bill=te;
                            printf("Item successfully purchased\n");
                        }
                    }
                }
            }
            if(flag==0)
            {
                printf("Entered Item-id not found\n");
            }
        }
    }
    updatedatamine(n,datamine,bill);
    printf("Total Amount : %u\n",totalprice(bill));
    struct node*n1=insert(arr1,NULL,NULL,bill,NULL,-1,2);
    ++bid;
    if(n1)arr1=n1;
    return arr1;
}
struct node**initializeaisle(unsigned int size)
{
    struct node**retval=(struct node**)malloc(size*sizeof(struct node*));
    for(int i=0;i<size;i++)
    {
        retval[i]=NULL;
    }
    return retval;
}
unsigned int**initializemine(unsigned int n)
{
    unsigned int**datamine=(unsigned int**)malloc(n*sizeof(int*));
    for(unsigned int i=0;i<n;i++)
    {
        datamine[i]=(unsigned int*)malloc(n*sizeof(int*));
    }
    for(unsigned int i=0;i<n;i++)
    {
        for(unsigned int j=0;j<n;j++)
        {
            datamine[i][j]=0;
        }
    }
    return datamine;
}
char**initializeaislename(unsigned int size)
{
    char**aislename=(char**)malloc(size*sizeof(char*));
    return aislename;
}
void deallocmemory(unsigned int size,struct node*arr[size],unsigned int size1,unsigned int*datamine[size1],char*aislename[size],struct node*bptr)
{
    for(unsigned int i=0;i<size;i++)
    {
      //  printf("a");
        if(arr[i])
        {
         //   printf("a");
            deleteroot(arr[i],0);
            arr[i]=NULL;
        }
        if(aislename[i])
        {
            free(aislename[i]);
            aislename[i]=NULL;
        }
    }
    for(int i=0;i<size1;i++)
    {
        if(datamine[i])
        {
            free(datamine[i]);
            datamine[i]=NULL;
        }
    }
    if(datamine)
    {
        free(datamine);
        datamine=NULL;
    }
    if(arr)
    {
        free(arr);
        arr=NULL;
    }
    if(aislename)
    {
        free(aislename);
        aislename=NULL;
    }
    if(bptr)deleteroot(bptr,2);
    bptr=NULL;
}
void main()
{
    struct node**arr=NULL;
    char**aislename=NULL;
    unsigned int**datamine=NULL;
    int c=0,flag=0;
    unsigned int size,size1;
    bool ans;
    FILE*fp=fopen("input1.txt","r");
    printf("Enter maximum number of aisles that can fit in Walmart : ");
    scanf("%u",&size);
    printf("Enter the maximum number of items that can fit in Walmart : ");
    scanf("%u",&size1);
    struct node*bptr=NULL;
    arr=initializeaisle(size);
    datamine=initializemine(size1);
    aislename=initializeaislename(size);
 //   ++aislecnt;
    while(flag<=1)
    {
        if(flag==1)
        {
            for(int i=0;i<aislecnt;i++)
            {
             //   printf("%d",aislecnt);
                struct node*temp=arr[i];
                while(temp->flag==0)
                {
                    temp=temp->u.next1[0];
                }
                struct aisle_node**arrt=NULL;
                int s=0;
                arrt=merge(temp->u.next2[1],NULL,&s);
                arr[i]=construct(s,arrt,NULL);
              //  printf("%d",arr[i]->data[0]);
               // if(arr[i])printf("a");
            }
        }
        do
        {
            if(flag>0)
            {
                if(aislecnt>=size)
                {
                    compaction(arr,size,size1,datamine);
                }
            }
            printf("Enter 1 to add item in store\n");
            printf("Enter 2 to update an item in store\n");
            printf("Enter 3 to delete an item from store\n");
            printf("Enter 4 to add an aisle to store\n");
            printf("Enter 5 to delete an aisle from store\n");
            printf("Enter 6 to Check availability of particular item for particular quantity and within the expiry date\n");
            printf("Enter 7 to list all the items whose quantity is less than their threshold quantity\n");
            printf("Enter 8 to list certain ready-to-eat item if its expiry is within one week\n");
            printf("Enter 9 to list those dairy products that are likely to expire within one day\n");
            printf("Enter 10 to merge two aisles in a single aisle\n");
            printf("Enter 11 to to list all other items that are bought with given item-id\n");
            printf("Enter 12 to proceed with buying items\n");
            printf("Enter 13 to do range search\n");
            printf("Enter 14 to do manual compaction\n");
            printf("Enter 15 to display items\n");
            printf("Enter 0 to exit\n");
            if(flag==0)
            {
                fscanf(fp,"%d",&c);
            }
            else scanf("%d",&c);
            switch (c)
            {
                case 1:
                    fflush(stdin);
                    AddItem(arr,fp,flag);
                    break;
                case 2:
                    updateItem(arr);
                    break;
                case 3:
                    printf("Enter Item-id to be deleted : ");
                    unsigned int num;
                    scanf("%u",&num);
                    DeleteItem(arr,num);
                    break;
                case 4:
                    addupdateAisle(fp,arr,size,aislename,flag);
                    break;
                case 5:
                    printf("Enter Aisle-id to be deleted : ");
                    scanf("%d",&num);
                    deleteAisle(arr,num,size,aislename);
                    break;
                case 6:
                    ans=isavailableitem(arr);
                    if(ans)
                        printf("Requested Item is available\n");
                    else
                        printf("Requested Item is not available\n");
                    break;
                case 7:
                    beyondthreshold(arr);
                    break;
                case 8:
                    readytoeat(arr,size);
                    break;
                case 9:
                    dairyexpiry(arr);
                    break;
                case 10:
                    mergeaisle(arr,size,aislename);
                    fflush(stdin);
                    break;
                case 11:
                    associateinfo(size1,datamine);
                    break;
                case 12:
                    bptr=BuyandMakeBill(arr,bptr,size1,datamine);
                    break; 
                case 13:
                    RangeSearch(arr);
                    break;
                case 14:
                    compaction(arr,size,size1,datamine);
                    break;
                case 15:
                    displayItems(arr);
            }
           /* if(bptr)
            {
                deleteroot(bptr,1);
                bptr=NULL;
            }*/
        }while(c);
        ++flag;
    }
 //   printf("%d %d %d %d %d",arr[0]->size,arr[1]->size,arr[2]->size,arr[3]->size,arr[4]->size);
 //   printf("main");//arr[0]->u.next1[1]->u.next1[1]->u.next1[2]->u.next2[2]->data[1]->item_id
 //   printf("%d",arr[1]->data[1]);
  //  printf("%d",arr[1]->u.next1[0]->u.next2[0]->data[0]->item_id);
 //   printf("%u",bptr->u.next4[1]->arr[0]->u.next1[0]->u.next1[0]->u.next1[0]->u.next3[1]->arr[0]->item_id);
    deallocmemory(aislecnt,arr,idgen,datamine,aislename,bptr);
    fclose(fp);
}
/*  struct node*root[5]={NULL};
    for(int i=0;i<7;i++)
    {
        AddItem(root,NULL,1);
    }
    for(int i=0;i<8;i++)
    {
        AddItem(root,NULL,1);
    }
    for(int i=0;i<8;i++)
    {
        AddItem(root,NULL,1);
    }
    for(int i=0;i<6;i++)
    {
        AddItem(root,NULL,1);
    }
    for(int i=0;i<7;i++)
    {
        AddItem(root,NULL,1);
    }
    int size=0;
    struct aisle_node**mer=merge(root[0]->u.next1[0]->u.next2[1],root[1]->u.next1[0]->u.next2[1],&size);
    struct node*n=construct(size,-1,mer,NULL,NULL);
    printf("%u\n",n->u.next1[2]->u.next2[2]->data[2]->item_id);
    struct datanode*dptr=n->u.next1[0]->u.next2[0];
    while(dptr)
    {
        for(int i=0;i<dptr->size;i++)
        {
            printf("%d ",dptr->data[i]->item_id);
        }
        printf("\n");
        dptr=dptr->right;
    }*/
 //   struct node*n=construct(size,mer,NULL);
 //   printf("%d",n->u.next1[0]->data[0]);
/*
struct aisle_node**arr=NULL;
    char**aislename=NULL;
    unsigned int**datamine=NULL;
    int c=0,flag=0;
    unsigned int size,size1;
    bool ans;
    FILE*fp=fopen("input1.txt","r");
    printf("Enter maximum number of aisles that can fit in Walmart : ");
    scanf("%u",&size);
    printf("Enter the maximum number of items that can fit in Walmart : ");
    scanf("%u",&size1);
    struct bill_node*bptr=NULL;
    arr=initializeaisle(size);
    datamine=initializemine(size1);
    aislename=initializeaislename(size);
    while(flag<=1)
    {
        do
        {
            if(flag>0)
            {
                if(aislecnt>=size)
                {
                    compaction(arr,size,size1,datamine);
                }
            }
            printf("Enter 1 to add item in store\n");
            printf("Enter 2 to update an item in store\n");
            printf("Enter 3 to delete an item from store\n");
            printf("Enter 4 to add an aisle to store\n");
            printf("Enter 5 to delete an aisle from store\n");
            printf("Enter 6 to Check availability of particular item for particular quantity and within the expiry date\n");
            printf("Enter 7 to list all the items whose quantity is less than their threshold quantity\n");
            printf("Enter 8 to list certain ready-to-eat item if its expiry is within one week\n");
            printf("Enter 9 to list those dairy products that are likely to expire within one day\n");
            printf("Enter 10 to merge two aisles in a single aisle\n");
            printf("Enter 11 to to list all other items that are bought with given item-id\n");
            printf("Enter 12 to proceed with buying items\n");
            printf("Enter 13 to do manual compaction\n");
            printf("Enter 0 to exit\n");
            if(flag==0)
            {
                fscanf(fp,"%d",&c);
            }
            else scanf("%d",&c);
            switch (c)
            {
                case 1:
                    fflush(stdin);
                    AddItem(arr,fp,flag);
                    break;
                case 2:
                    updateItem(arr);
                    break;
                case 3:
                    printf("Enter Item-id to be deleted : ");
                    int num;
                    scanf("%d",&num);
                    DeleteItem(arr,num);
                    break;
                case 4:
                    addupdateAisle(fp,arr,size,aislename,flag);
                    break;
                case 5:
                    printf("Enter Aisle-id to be deleted : ");
                    scanf("%d",&num);
                    deleteAisle(arr,num,size,aislename);
                    break;
                case 6:
                    ans=isavailableitem(arr);
                    if(ans)
                        printf("Requested Item is available\n");
                    else
                        printf("Requested Item is not available\n");
                    break;
                case 7:
                    beyondthreshold(arr);
                    break;
                case 8:
                    readytoeat(arr,size);
                    break;
                case 9:
                    dairyexpiry(arr);
                    break;
                case 10:
                    mergeaisle(arr,size,aislename);
                    fflush(stdin);
                    break;
                case 11:
                    associateinfo(size,datamine);
                    break;
                case 12:
                    bptr=BuyandMakeBill(arr,size,datamine);
                    break;    
                case 13:
                    compaction(arr,size,size1,datamine);
            }
            if(bptr)
            {
                while(bptr)
                {
                    struct bill_node*nptr=bptr->next;
                    free(bptr);
                    bptr=nptr;
                }
            }
        }while(c);
        ++flag;
    }
    fclose(fp);
*/
/*
Butter
50
0
43
5
1
04
2024
Yogurt
50
0
30
5
20
03
2024
Ice_Cream
50
0
30
5
25
03
2024
Ghee
30
0
60
5
01
04
2024
Curd
30
0
10
5
15
03
2024
Wheat
25
1
45
10
20
10
2024
rice
25
1
80
10
03
03
2025
corn
30
1
35
5
20
03
2024
oats
35
1
150
10
03
12
2024
Barley
25
1
20
5
10
12
2024
*/
/*
Butter
50
0
43
5
1
04
2024
Yogurt
50
0
30
5
20
03
2024
Ice_Cream
50
0
30
5
25
03
2024
Ghee
30
0
60
5
01
04
2024
Curd
30
0
10
5
15
03
2024
whey_protein_powder
20
0
160
5
05
02
2025
Milk_powder
30
0
50
5
20
04
2024
Wheat
25
1
45
10
20
10
2024
rice
25
1
80
10
03
03
2025
corn
30
1
35
5
20
03
2024
oats
35
1
150
10
03
12
2024
Barley
25
1
20
5
10
12
2024
Navy_beans
25
1
75
5
05
01
2025
Rajma
30
1
150
10
10
12
2024
soybeans
40
1
55
10
20
03
2025
Bathroom_Mug
30
2
85
5
00
00
0000
Bathroom_Bucket
25
2
120
5
00
00
0000
Soap
50
2
40
10
10
12
2024
Shampoo_Bottle_350ml
25
2
450
5
10
01
2025
Detergent_1kg
30
2
75
5
24
10
2024
wiper
25
2
220
5
00
00
0000
Mop
25
2
300
5
00
00
0000
Detergent_Bar_250g
40
2
100
10
05
02
2025
Salad
20
3
54
5
18
03
2024
chocolate_cupcake
35
3
110
5
18
03
2024
cheese_cake
30
3
70
5
18
03
2024
Sandwich
30
3
40
10
18
03
2024
Burger
30
3
70
10
15
03
2024
Pizza
30
3
100
10
18
03
2024
Potato_1kg
100
4
10
10
13
04
2024
Tomato_1kg
50
4
30
10
20
03
2024
Brinjal_1kg
40
4
40
5
20
03
2024
Lady_Finger_1kg
50
4
60
10
20
03
2024
Carrot_1kg
40
4
60
5
23
03
2024
Radish_1kg
40
4
75
5
23
03
2024
Bottole_Gourd_1kg
40
4
30
5
20
03
2024
*/