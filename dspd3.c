#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>//1 5 6 8 9
unsigned int idgen=0,aislecnt=0;
struct date
{
    unsigned int dd,mm,yyyy;
};
struct aisle_node
{
    unsigned int item_id,quantity,aisle_no,price,threshold;
    char*item_name;
    struct date u;
    struct aisle_node*next;
};
struct bill_node
{
    unsigned int item_id,quantity,price;
    char*item_name;
    struct bill_node*next;
};
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
void compaction(struct aisle_node**arr,unsigned int size,int size1,unsigned int*datamine[size1])
{
    unsigned int j=0,cnt=0;
    for(unsigned int i=0;i<size;i++)
    {
        if(arr[i])
        {
            arr[j]=arr[i];
            struct aisle_node*nptr=arr[j]->next;
            do
            {
                nptr->aisle_no=j;
                updatedatamine2(size1,datamine,cnt,nptr->item_id);
                nptr->item_id=cnt;
                nptr=nptr->next;
                ++cnt;
            }while(nptr!=arr[j]->next);
            if(j!=i)
            {
                arr[i]=NULL;
            }
            j++;
        }
    }
    idgen=cnt;
    aislecnt=j;
}//1 0 0 0 1 0 1
struct aisle_node*createAislenode(unsigned int quantity,unsigned int aisle_no,unsigned int price,char*name,int threshold,int d,int m,int y)
{
    struct aisle_node*n=(struct aisle_node*)malloc(sizeof(struct aisle_node));
    n->item_id=idgen++;
    n->quantity=quantity;
    n->aisle_no=aisle_no;
    n->price=price;
    n->item_name=(char*)malloc((strlen(name)+1)*sizeof(char));
    strcpy(n->item_name,name);
    n->next=NULL;
    n->threshold=threshold;
    n->u.dd=d;
    n->u.mm=m;
    n->u.yyyy=y;
    printf("ID-no assigned, Remember it for future purpose -> ID-no : %u\n",n->item_id);
    return n;
}
void AddItem(struct aisle_node**arr,FILE*fp,int flag)
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
    if(arr[aisle_no])
    {
        n->next=arr[aisle_no]->next;
        arr[aisle_no]->next=n;
        arr[aisle_no]=n;
    }
    else
    {
        arr[aisle_no]=n;
        n->next=n;
    }
}
void updateItem(struct aisle_node**arr)
{
    int itno,flag=0;
    printf("Enter Item_id to be updated : ");
    scanf("%d",&itno);
    struct aisle_node*nptr;
    for(int i=0;i<aislecnt&&flag==0;i++)
    {
        if(arr[i])
        {
            if(arr[i]->item_id>=itno&&arr[i]->next->item_id<=itno)
            {
                nptr=arr[i];
                do
                {
                    if(nptr->item_id==itno)
                    {
                        flag=1;
                    }
                    else 
                        nptr=nptr->next;
                }while(nptr!=arr[i]&&flag==0);
            }
        }
    }
    if(flag==1)
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
void DeleteItem(struct aisle_node**arr,int itemid)
{
    int flag=0;
    for(int i=0;i<aislecnt&&flag==0;i++)
    {
        if(arr[i])
        {
            if(arr[i]->item_id>=itemid&&arr[i]->next->item_id<=itemid)
            {
                struct aisle_node*nptr=arr[i];
                if(nptr->item_id==itemid)
                {
                    if(nptr->next!=nptr)
                    {
                        while(nptr->next!=arr[i])
                        {
                            nptr=nptr->next;
                        }
                        nptr->next=arr[i]->next;
                        free(arr[i]);
                        arr[i]=nptr;
                        flag=1;
                    }
                    else
                    {
                        free(arr[i]);
                        arr[i]=NULL;
                        flag=1;
                    }
                }
                else
                {
                    while(nptr->next!=arr[i]&&nptr->next->item_id!=itemid)
                    {
                        nptr=nptr->next;
                    }
                    if(nptr->next->item_id==itemid)
                    {
                        struct aisle_node*tem=nptr->next;
                        nptr->next=tem->next;
                        free(tem);
                        flag=1;
                    }
                }
            }
        }
    }
}
void addupdateAisle(FILE*fp,struct aisle_node**arr,unsigned int n,char*aislename[n],int flag)
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
void deleteAisle(struct aisle_node**arr,int Aisleid,unsigned int n,char*aislename[n])
{
    if(Aisleid<aislecnt)
    {
        struct aisle_node*nptr=arr[Aisleid];
        if(nptr)
        {
            while(nptr->next!=nptr)
            {
                struct aisle_node*temp=nptr->next;
                nptr->next=temp->next;
                free(temp);
            }
            free(nptr);
            arr[Aisleid]=NULL;
            free(aislename[Aisleid]);
        }
    }
    else 
        printf("Aisle-id is beyond limit\n");
}
bool isavailableitem(struct aisle_node**arr)
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
            if(arr[i]->item_id>=id&&arr[i]->next->item_id<=id)
            {
                flag=1;
                int flag1=0;
                struct aisle_node*n=arr[i];
                do
                {
                    if(n->item_id==id)flag1=1;
                    else n=n->next;
                }while(n!=arr[i]&&flag1==0);
                if(flag1==1)
                {
                    if(quantity<=n->quantity)
                    {
                        if(d.yyyy>n->u.yyyy||(d.yyyy==n->u.yyyy&&d.mm>n->u.mm)||(d.yyyy==n->u.yyyy&&d.mm==n->u.mm&&d.dd>n->u.dd))
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
void beyondthreshold(struct aisle_node**arr)
{
    for(int i=0;i<aislecnt;i++)
    {
        if(arr[i])
        {
            struct aisle_node*n=arr[i];
            do
            {
                if(n->quantity<n->threshold)
                {
                    printf("Item id : %u\nQuantity : %u\nThreshold : %u\n\n",n->item_id,n->quantity,n->threshold);
                }
                n=n->next;
            }while(n!=arr[i]);
        }
    }
}
void readytoeat(struct aisle_node**arr1,unsigned int size)
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
        struct aisle_node*nptr=arr1[arr[j]];
        printf("Aisle no :%d\n",arr[j]);
        do
        {
            if(nptr->u.yyyy==d.yyyy)
            {
                if(nptr->u.mm==d.mm)
                {
                    if(nptr->u.dd-d.dd<=7&&nptr->u.dd>=d.dd)
                    {
                        printf("ID no.%u :getting expired in %u days\n",nptr->item_id,nptr->u.dd-d.dd);
                    }
                }
                else if(nptr->u.mm==d.mm+1)
                {
                    if(d.mm==1||d.mm==3||d.mm==5||d.mm==7||d.mm==8||d.mm==10||d.mm==12)
                    {
                        if(31-d.dd+nptr->u.dd<=7)
                        {
                            printf("ID no.%u :getting expired in %u days\n",nptr->item_id,31-d.dd+nptr->u.dd);
                        }
                    }
                    else if(d.mm!=2)
                    {
                        if(30-d.dd+nptr->u.dd<=7)
                        {
                            printf("ID no.%u :getting expired in %u days\n",nptr->item_id,30-d.dd+nptr->u.dd);
                        }
                    }
                    else
                    {
                        if((d.yyyy%100!=0&&d.yyyy%4==0)||(d.yyyy%100==0&&d.yyyy%400==0))
                        {
                            if(29-d.dd+nptr->u.dd<=7)
                            {
                                printf("ID no.%u :getting expired in %u days\n",nptr->item_id,29-d.dd+nptr->u.dd);
                            }
                        }
                        else
                        {
                            if(28-d.dd+nptr->u.dd<=7)
                            {
                                printf("ID no.%u :getting expired in %u days\n",nptr->item_id,28-d.dd+nptr->u.dd);
                            }
                        }
                    }
                }
            }
            else if(nptr->u.yyyy=d.yyyy+1)
            {
                if(nptr->u.mm==1&&d.mm==12)
                {
                    if(31-d.dd+nptr->u.dd<=7)
                    {
                        printf("ID no.%u :getting expired in %u days\n",nptr->item_id,31-d.dd+nptr->u.dd);
                    }
                }
            }
            nptr=nptr->next;
        }while(nptr!=arr1[arr[j]]);
    }
}
void dairyexpiry(struct aisle_node**arr1)
{
    struct date d;
    printf("Enter current date :\nDD : ");
    scanf("%u",&d.dd);
    printf("mm : ");
    scanf("%u",&d.mm);
    printf("yyyy : ");
    scanf("%u",&d.yyyy);
    struct aisle_node*nptr=arr1[0];
    do
    {
        if(nptr->u.yyyy==d.yyyy)
        {
            if(nptr->u.mm==d.mm)
            {
                if(nptr->u.dd-d.dd<=1&&nptr->u.dd>=d.dd)
                {
                    printf("ID no.%u :getting expired in %u day\n",nptr->item_id,nptr->u.dd-d.dd);
                }
            }
            else if(d.mm==1||d.mm==3||d.mm==5||d.mm==7||d.mm==8||d.mm==10||d.mm==12)
            {
                if(d.dd==31&&nptr->u.dd==1)
                {
                    printf("ID no.%u :getting expired in %u day\n",nptr->item_id,1);
                }
            }
            else if(d.mm!=2)
            {
                if(d.dd==30&&nptr->u.dd==1)
                {
                    printf("ID no.%u :getting expired in %u day\n",nptr->item_id,1);
                }
            }
            else
            {
                if((d.yyyy%100!=0&&d.yyyy%4==0)||(d.yyyy%100==0&&d.yyyy%400==0))
                {
                    if(d.dd==29&&nptr->u.dd==1)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr->item_id,1);
                    }
                }
                else
                {
                    if(d.dd=28&&nptr->u.dd==1)
                    {
                        printf("ID no.%u :getting expired in %u day\n",nptr->item_id,1);
                    }
                }
            }
        }
        else if(nptr->u.yyyy==d.yyyy+1)
        {
            if(nptr->u.mm==1&&d.mm==12)
            {
                if(nptr->u.dd==1&&d.dd==31)
                {
                    printf("ID no.%u :getting expired in %u day\n",nptr->item_id,1);
                }
            }
        }
        nptr=nptr->next;
    }while(nptr!=arr1[0]);
}
void mergeaisle(struct aisle_node**arr1,unsigned int n,char*aislename[n])
{
    int num1,num2;
    printf("Enter two Aisle numbers to be merged : ");
    scanf("%d%d",&num1,&num2);
    if(num1<aislecnt&&num2<aislecnt)
    {    
        struct aisle_node*head=NULL,*tail=head,*nptr1=arr1[num1]->next,*nptr2=arr1[num2]->next;
        arr1[num1]->next=NULL;
        arr1[num2]->next=NULL;
        while(nptr1&&nptr2)
        {
            if(nptr1->item_id<nptr2->item_id)
            {
                if(head)
                {
                    tail->next=nptr1;
                    tail=nptr1;
                    nptr1=nptr1->next;
                    tail->next=NULL;
                }
                else
                {
                    head=tail=nptr1;
                    nptr1=nptr1->next;
                    tail->next=NULL;
                }
            }
            else
            {
                if(head)
                {
                    tail->next=nptr2;
                    tail=nptr2;
                    nptr2=nptr2->next;
                    tail->next=NULL;
                }
                else
                {
                    head=tail=nptr2;
                    nptr2=nptr2->next;
                    tail->next=NULL;
                }
            }
        }
        if(nptr1)
        {
            if(head)
            {
                tail->next=nptr1;
                while(nptr1->next)
                {
                   nptr1=nptr1->next;
                }
                tail=nptr1;
            }
            else
            {
                head=nptr1;
                while(nptr1->next)
                {
                    nptr1=nptr1->next;
                }
                tail=nptr1;
            }
        }
        else if(nptr2)
        {
            if(head)
            {
                tail->next=nptr2;
                while(nptr2->next)
                {
                    nptr2=nptr2->next;
                }
                tail=nptr2;
            }
            else 
            {
                head=nptr2;
                while(nptr2->next)
                {
                    nptr2=nptr2->next;
                }
                tail=nptr2;
            }
        }
        tail->next=head;
        if(num1<num2)
        {
            arr1[num1]=tail;
            arr1[num2]=NULL;
            free(aislename[num2]);
            aislename[num2]=NULL;
            printf("Aisle no.%d and Aisle no.%d merged successfully in aisle no.%d\n",num1,num2,num1);
            printf("Enter the updated name of merged aisle : ");
            scanf("%s",aislename[num1]);
            struct aisle_node*nptr=tail;
            do
            {
                nptr->aisle_no=num1;
                nptr=nptr->next;
            }while(nptr!=tail);
        }
        else
        {
            arr1[num2]=tail;
            arr1[num2]=NULL;
            free(aislename[num1]);
            aislename[num1]=NULL;
            printf("Aisle no.%d and Aisle no.%d merged successfully in aisle no.%d",num2,num1,num2);
            printf("Enter the updated name of merged aisle : ");
            scanf("%s",aislename[num2]);
            struct aisle_node*nptr=tail;
            do
            {
                nptr->aisle_no=num2;
                nptr=nptr->next;
            }while(nptr!=tail);
        }
    }
    else printf("Enter aisle-id is beyond range of allocated ids for aisle\n");
}
void updatedatamine(struct bill_node*bptr,unsigned int n,unsigned int*datamine[n])
{
    struct bill_node*nptr1=bptr;
    struct bill_node*nptr2;
    while(nptr1)
    {
        nptr2=nptr1;
        while(nptr2)
        {
            ++datamine[nptr1->item_id][nptr2->item_id];
            if(nptr1!=nptr2)++datamine[nptr2->item_id][nptr1->item_id];
            nptr2=nptr2->next;
        }
        nptr1=nptr1->next;
    }
}
void associateinfo(unsigned int n,unsigned int*datamine[n])
{
    unsigned int itemid;
    printf("Enter ID : ");
    scanf("%u",&itemid);
    if(itemid<idgen)
    {
        for(unsigned int i=0;i<idgen;i++)
        {
            if(datamine[itemid][i]>0)
            {
                printf("Item-id no.%u : %u times\n",i,datamine[itemid][i]);
            }
        }
    }
    else printf("Invalid ID");
}
unsigned int total_price(struct bill_node*bptr)
{
    struct bill_node*nptr=bptr;
    unsigned int total=0;
    while(nptr)
    {
        total+=nptr->price*nptr->quantity;
        nptr=nptr->next;
    }
    return total;
}  
struct bill_node*BuyandMakeBill(struct aisle_node**arr,unsigned int n,unsigned int*datamine[n])
{
    struct bill_node*bill=NULL;
    char ch='1';
    while(ch=='1')
    {
        fflush(stdin);
        printf("Press 1 to buy from store or 0 to exit the system : ");
        scanf("%c",&ch);
        if(ch=='1')
        {
            printf("Following are list of items in store as per item-id :\n");
            for(int i=0;i<aislecnt;i++)
            {
                if(arr[i])
                {
                    printf("Aisle no.%d : \n",i);
                    struct aisle_node*nptr=arr[i]->next;
                    do
                    {
                        printf("Item-id : %u\nItem-name : %s\nQuantity : %u\n\n",nptr->item_id,nptr->item_name,nptr->quantity);
                        nptr=nptr->next;
                    }while(nptr!=arr[i]->next);
                }
            }
            printf("Enter Item-id to be purchased : ");
            unsigned int itid,qty;
            scanf("%u",&itid);
            printf("Enter quantity of item : ");
            scanf("%u",&qty);
            int flag=0;
            for(int i=0;i<aislecnt&&flag==0;i++)
            {
                if(arr[i]&&arr[i]->item_id>=itid&&arr[i]->next->item_id<=itid)
                {
                    struct aisle_node*nptr=arr[i];
                    do
                    {
                        if(nptr->item_id==itid)
                        {
                            flag=1;
                        }
                        else nptr=nptr->next;
                    }while(nptr!=arr[i]&&flag==0);
                    if(flag==1)
                    {
                        if(nptr->quantity<qty)
                        {
                            printf("Insufficient stock\n");
                        }
                        else
                        {
                            struct bill_node*temp=(struct bill_node*)malloc(sizeof(struct bill_node));
                            temp->item_id=itid;
                            temp->quantity=qty;
                            temp->item_name=nptr->item_name;
                            temp->price=nptr->price;
                            temp->next=bill;
                            bill=temp;
                            nptr->quantity-=qty;
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
    printf("Total Amount : %u\n",total_price(bill));
    updatedatamine(bill,n,datamine);
    return bill;
}
struct aisle_node**initializeaisle(unsigned int size)
{
    struct aisle_node**retval=(struct aisle_node**)malloc(size*sizeof(struct aisle_node*));
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
void deallocmemory(unsigned int size,struct aisle_node*arr[size],unsigned int size1,unsigned int*datamine[size1],char*aislename[size])
{
    if(arr)
    {
        for(unsigned int i=0;i<size;i++)
        {
            struct aisle_node*nptr=arr[i];
            if(nptr)
            {
                while(nptr->next!=nptr)
                {
                    struct aisle_node*temp=nptr->next;
                    nptr->next=temp->next;
                    free(temp);
                }
                free(nptr);
                arr[i]=nptr=NULL;
            }
            if(aislename[i])
            {
                free(aislename[i]);
            }
        }
        free(arr);
        free(aislename);
        for(unsigned int i=0;i<size1;i++)
        {
            free(datamine[i]);
            datamine[i]=NULL;
        }
        free(datamine);
    }
}
void main()
{
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
    deallocmemory(size,arr,size1,datamine,aislename);
}   
/*
per 100g
1
Butter
50
0
43
5
20
03
2024
1
Yogurt
50
0
30
5
20
03
2024
1
Ice_Cream
50
0
30
5
25
03
2024
1
Ghee
30
0
60
5
01
04
2024
1
Curd
30
0
10
5
15
03
2024
1
whey_protein_powder
20
0
160
5
05
02
2025
1
Milk_powder
30
0
50
5
20
04
2024
1
Wheat
25
1
45
10
20
10
2024
1
rice
25
1
80
10
03
03
2025
1
corn
30
1
35
5
20
03
2024
1
oats
35
1
150
10
03
12
2024
1
Barley
25
1
20
5
10
12
2024
1
Navy_beans
25
1
75
5
05
01
2025
1
Rajma
30
1
150
10
10
12
2024
1
soybeans
40
1
55
10
20
03
2025
0

wheat
rice
corn
oats
barley
rye
baked beans (navy beans)
kidney beans
mung beans
soybeans
chickpeas
lentils
split peas.

*/
/*
1-31
2-28/29


*/