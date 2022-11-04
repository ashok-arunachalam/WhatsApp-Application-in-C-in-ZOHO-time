#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmpareString(char *, char *);
List *insertMes(List *, void *); // Declearing Functions Globally
int chk_mes(char *, char *);

HashSet *hs;
HashGrp *hg;
char USER[50];
char res_user[50]; // Declearing Some Global Variables

int grpChat();
void perChat();
void log_out();

char mes[100];


typedef struct List
{

    char u_name[50];
    char message[500]; // LinkedList to Store the mess for your Friend.
    char time[30];
    struct List *next;

} List;

typedef struct HashMap
{

    char u_name[50];
    int block; // Map to store your each friend's data
    struct List *value;

} HashMap;

typedef struct Node
{
    char username[50];
    HashMap *data; // List to store each admin user's data
    struct Node *next;

} Node;

typedef struct HashSet
{

    struct Node *key; // HashSet to the All the user

} HashSet;
typedef struct Grp_mes
{
    char mes[100];
    char username[100]; // List to store the groupmessage
    char time[30];
    struct Grp_mes *next;

} Grp_mes;

typedef struct UsersList
{
    char user_List[50];
    struct UsersList *next; // List to store the group members

} UsersList;

typedef struct HashMapGrp
{
    char admin[50];
    struct UsersList *user_list; // Map to store the the mes and grpmembers with key value of Admin
    struct Grp_mes *mes;

} HashMapGrp;

typedef struct NodeGrp
{
    char grpName[50];
    struct HashMapGrp *hashmapGrp; // List to store the all the data for group
    struct NodeGrp *next;

} NodeGrp;

typedef struct HashGrp
{
    struct NodeGrp *key;

} HashGrp;



HashSet *newmalloc(int n) // Function to allocate memory for the HashSet holding the 1-n mes data
{

    HashSet *ob = (HashSet *)malloc(n * sizeof(HashSet)); // ob->reference for hashset
    for (int i = 0; i < n; i++)
    {
        ob[i].key = NULL;
    }

    hs = ob;
}

int hashValue(char *c) // returning the hash value for the given string
{

    int n = 0, i = 0;
    while (c[i] != '\0')
    {
        n = n + c[i];
        i++;
    }
    return n % 10; // 10->becoz size of the hashset
}

HashMap *mallocMap(HashMap *hm) // Function to allocating memory to HashMap
{
    hm = (HashMap *)malloc(10 * sizeof(HashMap));
    for (int i = 0; i < 10; i++)
    {
        hm[i].block = 0;
        hm[i].value = NULL;
    }
    return hm;
}

char *encode(char *c) // Function to encode the message
{
    int i = 0;
    while (c[i] != '\0')
    {
        c[i] = c[i] - 3;
        i++;
    }
    return c;
}
char *decode(char *c) // Function to decode the message
{
    int i = 0;
    while (c[i] != '\0')
    {
        c[i] = c[i] + 3;
        i++;
    }
    return c;
}

Node *insertList(Node *head, char *c) // inserting users to the hashset
{

    Node *nxt = head; // Node refers to the list that holds the username and usernames's HashMap
    if (head == NULL)
    {
        head = (Node *)malloc(sizeof(Node));
        head->data = mallocMap(head->data); // data is the map for the username
        strcpy(head->username, c);
        head->next = NULL;
        return head;
    }
    else
    {
        Node *tmp = head;
        while (tmp != NULL)
        {
            if (cmpareString(tmp->username, c)) // tmp is the temporary reference for Node
            {
                printf("User Already Available\n\n");
                return head;
            }
            tmp = tmp->next;
        }

        Node *tem = (Node *)malloc(sizeof(Node)); // tep is the another temporary reference for Node
        tem->data = mallocMap(tem->data);
        strcpy(tem->username, c);
        tem->next = NULL;

        while (nxt->next != NULL)
        {
            nxt = nxt->next;
        }
        nxt->next = tem;
        nxt = nxt->next;
        nxt->next = NULL;
    }

    return head;
}

void add(char *c) // c contains the username
{

    hs[hashValue(c)].key = insertList(hs[hashValue(c)].key, c); // function to add the user into the hashset  --->hs refers to the global variable
}

HashMap *ret_map(Node *tem, char *name) // function to return the map for the given node
{
    Node *pre, *t = tem;
    while (t != NULL)
    {
        if (cmpareString(t->username, name))
        {
            return t->data; // pre and t are the another reference for the node
        }
        pre = t;
        t = t->next;
    }
    return pre->data;
}

HashMap *insert_mes_list(HashMap *hm, void *ptr, char *name) // function to store the message for the into the map
{

    for (int i = 0; i < 10; i++) // Running only 10 times becoz of the size 10 init to the map
    {

        if (cmpareString(hm[i].u_name, name) && hm[i].block)
        {
            printf("Sorry Account Blocked by You:(\n\n");
            return hm;
        }
        else if (hm[i].value == NULL && (!cmpareString(name, hm[i].u_name))) // this condition to check whether the node is null or not
        {

            strcpy(hm[i].u_name, name);
            hm[i].value = insertMes(hm[i].value, ptr);
            return hm;
        }
        else if (cmpareString(name, hm[i].u_name)) // if not null this conditions will run
        {
            hm[i].value = insertMes(hm[i].value, ptr);
            return hm;
        }
    }
}

int blkChk(HashMap *hm, char *name) // this function to check whether  the user is bblocked or not
{

    for (int i = 0; i < 10; i++)
    {
        if (cmpareString(hm[i].u_name, name) && hm[i].block)
            return 0;
    }
    return 1;
}

Node *insertMap(HashMap *hm, char *user, Node *head)
{ // Insert the list to the map for the corresponding user
    Node *tem = head;
    while (tem != NULL)
    {
        if (cmpareString(tem->username, user))
        { // comparing to insert correct user's map's value
            tem->data = hm;
            return head;
        }
        tem = tem->next;
    }
    return head;
}

void sendMes(void *ptr) // contains void pointer which refers the List's data contains the sender time and message
{

    Node *tem = hs[hashValue(USER)].key;   // tem is the temporary reference for the struct node
    HashMap *hm_user = ret_map(tem, USER); // hm_user is the temporary reference for the strut hashmap for the sender
    Node *res_tem = hs[hashValue(res_user)].key;
    HashMap *hm_receiver = ret_map(res_tem, res_user); // hm_receiver is the temporary reference for the strut hashmap for the receiver
    if (blkChk(hm_receiver, USER) && blkChk(hm_user, res_user))
    {
        hm_user = insert_mes_list(hm_user, ptr, res_user); // Function to check wheather the user is block or not

        tem = insertMap(hm_user, USER, tem); // if not message wil be inserted
        hs[hashValue(USER)].key = tem;

        hm_receiver = insert_mes_list(hm_receiver, ptr, USER);
        res_tem = insertMap(hm_receiver, res_user, res_tem);
        ;

        hs[hashValue(res_user)].key = res_tem;
    }
    else
    {
        printf("Account Blocked:(\n\n");
    }
}

List *insertMes(List *head, void *ptr) // inserting message to list
{

    List *nxt = head; // ptr is the void variable which contains the mess info
    if (head == NULL)
    {
        head = (List *)ptr;
        head->next = NULL;
        return head;
    }
    else
    {

        List *tem = (List *)ptr; // logic for storing the data into the list
        tem->next = NULL;

        while (nxt->next != NULL)
        {
            nxt = nxt->next;
        }
        nxt->next = tem;
        nxt = nxt->next;
        nxt->next = NULL;
        return head;
    }
}

int cmpareString(char *str1, char *str2)
{

    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
            return 0;
        i++; // Comparing the two string
    }
    if (str1[i] == '\0' && str2[i] != '\0')
        return 0;
    if (str1[i] != '\0' && str2[i] == '\0')
        return 0;
    return 1;
}

void displayMes() // function to display the two conversation
{
    Node *obj = hs[hashValue(USER)].key; // ob is storing the list data for the user
    HashMap *ohm;

    while (obj != NULL)
    {

        if (obj->data != NULL && cmpareString(obj->username, USER)) // here we are returning the exact hashmap for the user
        {
            ohm = obj->data;
            break;
        }
        obj = obj->next;
    }
    List *li;
    int flag = 0;
    for (int i = 0; i < 10; i++)
    {
        if (cmpareString(ohm[i].u_name, res_user) && ohm[i].value != NULL) // return the list, that list holds the previous inserted message
        {
            flag = 1;
            li = ohm[i].value; // if contains flag value changed to 1
            break;
        }
    }
    printf("Receiver Info:---> %s\n", res_user);
    if (flag == 0)
    {
        printf("No Message yet\n");
        return;
    }
    while (li != NULL && li->message != NULL && li->u_name != NULL) // loop to display all the message from the list
    {
        char tem[100];
        strcpy(tem, li->message); // li is the reference of the list
        strcpy(tem, decode(tem));
        if (cmpareString(li->u_name, res_user) && (!cmpareString(tem, "def")))
        {
            printf("\tSend by---> %s\n", li->u_name);
            printf("\tMessage---> %s\n", tem);
            printf("\tTime   ---> %s\n", li->time);
        }
        else if (cmpareString(li->u_name, USER))
        {
            printf("\t\t\t\tSend by---> %s\n", li->u_name); // printing fot the conversation view
            printf("\t\t\t\tMessage---> %s\n", tem);
            printf("\t\t\t\tTime   ---> %s\n", li->time);
        }
        li = li->next;
    }
    printf("------------------------------------------------------\n\n");
    return;
}

int containsUser(char *c) // function to check the user present or not
{

    Node *tmp = hs[hashValue(c)].key;
    while (tmp != NULL)
    {
        if (cmpareString(tmp->username, c))
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

void displayAll() // function to display the table view for the message
{
    char test[100];
    for (int i = 0; i < 10; i++)
    {
        if (hs[i].key != NULL)
        {
            Node *tp = hs[i].key;
            printf("Showing the Data for --------->%s\n\n\n", hs[i].key->username);
            for (int j = 0; j < 10; j++)
            {
                if (hs[i].key->data[j].value != NULL)
                {
                    printf("Data for the friend-------->%s\n\n\n", hs[i].key->data[j].u_name);
                    List *tmp = hs[i].key->data[j].value;
                    while (tmp != NULL)
                    {
                        strcpy(test, tmp->message);
                        printf("Name-->%s\n", tmp->u_name);
                        printf("Mess-->%s\n", decode(test));
                        tmp = tmp->next;
                    }
                }
            }
        }
    }
}
void displayTable() // function to show the available users
{
    printf("Friends For You\n");
    for (int i = 0; i < 10; i++)
    {
        Node *tmp = hs[i].key;
        while (tmp != NULL)
        {
            if ((!cmpareString(tmp->username, USER)))
            {
                printf("%s \n", tmp->username); // display all the user except the sender info
            }
            tmp = tmp->next;
        }
    }
    printf("\n\n");
}

void searchMes(char *mes) // function to search the message
{

    Node *tem = hs[hashValue(USER)].key;

    HashMap *thm;
    while (tem != NULL)
    {
        if (cmpareString(tem->username, USER))
        {
            thm = tem->data;
            break;
        }
        tem = tem->next;
    }
    List *val;
    for (int i = 0; i < 10; i++)
    {
        if (cmpareString(thm[i].u_name, res_user))
        {
            val = thm->value;
            break;
        }
    }
    printf("Some Similar Messages:-\n");
    while (val != NULL)
    {
        char decMes[100];
        strcpy(decMes, val->message);
        strcpy(decMes, decode(decMes));
        if (chk_mes(decMes, mes)) // here chk_mes is to return wheather the message matches or not
        {
            printf("%s\n", decMes);
        }
        val = val->next;
    }
    printf("\n");
}

int chk_mes(char *u_mes, char *f_mes)
{
    int c = 0, i = 0, j = 0;
    while (u_mes[i] != '\0' && f_mes[j] != '\0')
    {
        if (u_mes[i] == f_mes[j]) // the function return 1 if two string characters matches more then 3 in a sequential order
        {
            j++;
            c++;
        }
        i++;
        if (c >= 3)
            return 1;
    }
    return 0;
}

void del_mes_users(char *mes, Node *tem, char *user, char *receiver) // function to delete message in the list
{

    HashMap *thm;
    while (tem != NULL)
    {
        if (cmpareString(tem->username, user))
        {
            thm = tem->data; // return the exact map
            break;
        }
        tem = tem->next;
    }
    char decodemes[100];
    List *val;
    int j = 0;
    for (int i = 0; i < 10; i++)
    {
        if (cmpareString(thm[i].u_name, receiver))
        {
            val = thm[i].value; // condiion to give the message list
            j = i;
            break;
        }
    }
    if (thm[j].value == NULL)
    {
        printf("NO Message Available\n\n");
        return;
    }
    strcpy(decodemes, thm[j].value->message); // decoding here message becoz the linked list contains the encoded message
    if (cmpareString(decode(decodemes), mes))
    {
        val = thm[j].value;
        val = val->next;
        thm[j].value = val;
        return;
    }
    else
    {
        List *pre = val, *tem = val;
        while (tem != NULL)
        {
            strcpy(decodemes, tem->message);
            if (cmpareString(decode(decodemes), mes)) // checking wheather the message matches or not
            {
                pre->next = tem->next;
                tem = NULL;
                free(tem);
                thm[j].value = val;
                // printf("Message Deleted SuccessFully:)\n\n");
                return;
            }
            pre = tem;
            tem = tem->next;
        }
    }

    printf("No Exact Matchs Found:(\n\n");
}

void del_mes(char *mes)
{
    Node *tem = hs[hashValue(USER)].key;
    del_mes_users(mes, tem, USER, res_user);
    tem = hs[hashValue(res_user)].key; // calling function two different time because we want to delete message in both the user and receiver's data
    del_mes_users(mes, tem, res_user, USER);
}

void block(char *user, char *res, int n) // this function turn the valut to 1or 0 in for the block status of the user
{
    Node *tem = hs[hashValue(user)].key;
    HashMap *thm;

    while (tem != NULL)
    {
        if (cmpareString(tem->username, user))
        {
            thm = tem->data;
            break;
        }
        tem = tem->next;
    }
    int j = 0;
    for (int i = 0; i < 10; i++)
    {
        if (cmpareString(thm[i].u_name, res))
        {
            thm[i].block = n;
            j = i;
            tem->data = thm;
            hs[hashValue(user)].key = tem;
            break;
        }
    }
    if (thm[j].block != n)
    {
        List *ob = (List *)malloc(sizeof(List));
        void *ptr;
        strcpy(ob->message, "abc");
        strcpy(ob->time, "");
        strcpy(ob->u_name, USER);
        ptr = ob;
        sendMes(ptr);
        del_mes("def");

        while (tem != NULL)
        {
            if (cmpareString(tem->username, user))
            {
                thm = tem->data;
                break;
            }
            tem = tem->next;
        }
        int j = 0;
        for (int i = 0; i < 10; i++)
        {
            if (cmpareString(thm[i].u_name, res))
            {
                thm[i].block = n;
                j = i;
                break;
            }
        }
    }
}

void block_user()
{
    block(USER, res_user, 1); // function calls with 1 to block the requested user
    printf("Account Blocked:)--->\n\n");
}
void unblock_user()
{
    block(USER, res_user, 0);
    printf("Account UnBlocked:)\n\n"); // function calls with 0 to unblock the requested user
}

HashGrp *mallocHashGrp(int n)
{
    HashGrp *ob = (HashGrp *)malloc(n * sizeof(HashGrp));
    for (int i = 0; i < n; i++)
    {
        ob[i].key = NULL;
    }
    hg = ob; // initiating the memory for the map for the group
    return ob;
}

int createGrp(char *grp_name) // function to create the new group
{
    NodeGrp *obj = hg[hashValue(grp_name)].key;
    if (obj == NULL)
    {

        NodeGrp *head = (NodeGrp *)malloc(sizeof(NodeGrp));
        strcpy(head->grpName, grp_name);
        head->hashmapGrp = (HashMapGrp *)malloc(sizeof(HashMapGrp));
        strcpy(head->hashmapGrp->admin, USER);
        head->hashmapGrp->user_list = NULL;
        head->hashmapGrp->mes = NULL;
        head->next = NULL;
        obj = head;
        hg[hashValue(grp_name)].key = obj;
    }
    else
    {
        NodeGrp *tmp = obj;

        NodeGrp *head = (NodeGrp *)malloc(sizeof(NodeGrp));
        strcpy(head->grpName, grp_name);
        head->hashmapGrp = (HashMapGrp *)malloc(sizeof(HashMapGrp));
        strcpy(head->hashmapGrp->admin, USER);
        head->hashmapGrp->user_list = NULL;
        head->hashmapGrp->mes = NULL;
        head->next = NULL;

        if (cmpareString(tmp->grpName, grp_name))
        {
            printf("Group name Already Available\n\n");
            free(head);
            return 0;
        }

        while (tmp->next != NULL)
        {
            if (cmpareString(tmp->grpName, grp_name))
            {
                printf("Group name Already Available\n\n");
                free(head);
                return 0;
            }
            tmp = tmp->next;
        }

        tmp->next = head;
        tmp = tmp->next;
        tmp->next = NULL;
        hg[hashValue(grp_name)].key = obj;
        printf("Group Created\n");
        return 1;
    }
}

Grp_mes *getmes(char *grpName) // this function return the list corresponding to the given groupname
{
    NodeGrp *tng = hg[hashValue(grpName)].key;
    HashMapGrp *thm;
    Grp_mes *tgm, *mes;

    while (tng != NULL)
    {
        if (cmpareString(tng->grpName, grpName))
        {
            thm = tng->hashmapGrp;
            break;
        }
        tng = tng->next;
    }
    tgm = thm->mes;
    return tgm;
}

void adduser(char *grpname, char *user) // functiom to add the user to the corresponding group
{
    NodeGrp *tng = hg[hashValue(grpname)].key;
    HashMapGrp *hsm;
    if (tng == NULL)
    {
        printf("No--> Such Group Available\n\n");
        return;
    }
    int flag = 0;
    while (tng != NULL)
    {
        if (cmpareString(tng->grpName, grpname))
        {
            flag = 1;
            hsm = tng->hashmapGrp;
            break;
        }
        tng = tng->next;
    }
    if (flag == 0)
    {
        printf("No Such Group Available\n\n");
        return;
    }
    flag = 0;

    Node *tmp = hs[hashValue(user)].key;
    if (tmp == NULL)
    {
        printf("NO--> Available users\n\n");
        return;
    }

    while (tmp != NULL)
    {
        if (cmpareString(tmp->username, user))
        {
            flag = 1;
            break;
        }
        tmp = tmp->next;
    }
    if (flag == 1)
    {
        printf("User Available\n");
    }
    else
    {
        printf("NO Available users\n\n");
        return;
    }
    Node *tem = hs[hashValue(USER)].key;
    HashMap *hm_user = ret_map(tem, USER);
    Node *res_tem = hs[hashValue(user)].key;
    HashMap *hm_receiver = ret_map(res_tem, user);

    UsersList *li = hsm->user_list;
    if (cmpareString(hsm->admin, user))
    {
        printf("Admin Cannot be the Group Member\n");
        return;
    }
    if ((!blkChk(hm_user, user)) || (!blkChk(hm_receiver, USER)))
    {
        printf("Blocked Persons Cannot be Added\n");
        return;
    }
    if (li == NULL && (!cmpareString(hsm->admin, user)))
    {
        li = (UsersList *)malloc(sizeof(UsersList));
        strcpy(li->user_List, user);
        li->next = NULL;
        hsm->user_list = li;
        tng->hashmapGrp = hsm;
        hg[hashValue(grpname)].key = tng;
    }
    else if (li != NULL)
    {
        UsersList *litem = li;
        UsersList *newList = (UsersList *)malloc(sizeof(UsersList));
        strcpy(newList->user_List, user);
        newList->next = NULL;

        if ((!cmpareString(litem->user_List, user)))
        {
            while (litem->next != NULL)
            {
                if ((!cmpareString(litem->user_List, user)) && (!cmpareString(litem->next->user_List, user)))
                    litem = litem->next;
                else
                {
                    free(newList);
                    printf("User Already  in Group\n\n");
                    return;
                }
            }
            litem->next = newList;
            litem = litem->next;
            litem->next = NULL;
            hsm->user_list = li;
            tng->hashmapGrp = hsm;
            hg[hashValue(grpname)].key = tng;
        }
        else
        {
            free(newList);
            printf("User Already  in Group\n\n");
            return;
        }
    }
}

int check(char *grpname) // function to check wheather the group available or not
{
    NodeGrp *tem = hg[hashValue(grpname)].key;
    if (tem == NULL)
    {
        return 1;
    }
    int flag = 0;
    HashMapGrp *obj;
    while (tem != NULL)
    {
        if (cmpareString(tem->grpName, grpname))
        {
            obj = tem->hashmapGrp;
            flag = 1;
            break;
        }
        tem = tem->next;
    }
    if (flag == 0)
    {
        return 1;
    }
    if (cmpareString(obj->admin, USER)) // if group available and here checking wheather the admin is accessing the group
        return 0;
    UsersList *uli = obj->user_list;
    while (uli != NULL)
    {
        if (cmpareString(uli->user_List, USER))
        {
            return 0;
        }
        uli = uli->next;
    }
    return 2;
}

void availabeGrp() // function to display the the users available group
{

    NodeGrp *tng;
    HashMapGrp *thm;
    UsersList *tli;
    for (int i = 0; i < 10; i++)
    {

        if (hg[i].key != NULL)
        {
            tng = hg[i].key;
            while (tng != NULL)
            {
                if (tng->hashmapGrp != NULL) // display based in the posistion of the user
                {
                    thm = tng->hashmapGrp;
                    if (cmpareString(thm->admin, USER))
                    {
                        printf("You are in as ADMIN-----> %s\n\n", tng->grpName);
                    }
                    else
                    {
                        tli = thm->user_list;
                        while (tli != NULL)
                        {
                            if (cmpareString(tli->user_List, USER))
                            {
                                printf("You are in as MEMBER----->%s\n\n", tng->grpName);
                            }
                            tli = tli->next;
                        }
                    }
                }
                tng = tng->next;
            }
        }
    }
}

void sendGrpMes(char *grpName, void *ptr) // function to send message in the group
{
    NodeGrp *tng = hg[hashValue(grpName)].key; // tgn->refers to the temporary list contains the data of thee group
    HashMapGrp *thm;                           // thm is the reference for the hashmap contains the list of messages
    Grp_mes *tgm, *mes;                        // tgm and mes are the reference of the list of messages

    while (tng != NULL)
    {
        if (cmpareString(tng->grpName, grpName))
        {
            thm = tng->hashmapGrp;
            break;
        }
        tng = tng->next;
    }
    tgm = thm->mes;

    mes = (Grp_mes *)ptr;
    if (tgm == NULL)
    {
        tgm = mes;
        tgm->next = NULL;
        tng->hashmapGrp->mes = tgm;
        hg[hashValue(grpName)].key = tng;
    }
    else
    {
        Grp_mes *tra = tgm;
        while (tra->next != NULL)
        {
            tra = tra->next;
        }
        tra->next = mes;
        tra = tra->next;
        tra->next = NULL;
        tng->hashmapGrp->mes = tgm;
        hg[hashValue(grpName)].key = tng;
    }
}

void disGrpMes(char *grpName) // it display the group message
{
    Grp_mes *tgm = getmes(grpName);
    if (tgm == NULL)
    {
        printf("No message available\n");
        return;
    }
    while (tgm != NULL)
    {
        if (cmpareString(tgm->username, USER))
        {
            printf("\t\t\t\tSend by---> %s\n", tgm->username);
            printf("\t\t\t\tMessage---> %s\n", tgm->mes);
            printf("\t\t\t\tTime   ---> %s\n", tgm->time);
        }
        else
        {
            printf("\tSend by---> %s\n", tgm->username);
            printf("\tMessage---> %s\n", tgm->mes);
            printf("\tTime   ---> %s\n", tgm->time);
        }
        printf("\n");
        tgm = tgm->next;
    }
}

void displayGrp() // function to display the grp members in the table view based on the posistion
{
    for (int i = 0; i < 10; i++)
    {
        if (hg[i].key != NULL)
        {
            NodeGrp *tmp = hg[i].key;
            while (tmp != NULL)
            {
                printf("GrpName--->%s\n", tmp->grpName);
                printf("Admin--->%s\n", tmp->hashmapGrp->admin);
                UsersList *li = tmp->hashmapGrp->user_list;
                while (li != NULL)
                {
                    printf("---------------->%s\n\n", li->user_List);
                    li = li->next;
                }

                tmp = tmp->next;
            }
            printf("\n\n");
        }
    }
}

void add_users(char *grpname, char *user) // adding users to the group
{

    NodeGrp *tng = hg[hashValue(grpname)].key;
    HashMapGrp *hsm;
    if (tng == NULL)
    {
        printf("No--> Such Group Available\n\n"); // checking if the group available
        return;
    }
    int flag = 0;
    while (tng != NULL)
    {
        if (cmpareString(tng->grpName, grpname))
        {
            flag = 1;
            hsm = tng->hashmapGrp;
            break;
        }
        tng = tng->next;
    }

    if (cmpareString(hsm->admin, USER)) // checking if you are the admin gor the group
    {
        adduser(grpname, user);
        return;
    }
    else
    {
        printf("You are not Admin to add person\n\n");
        return;
    }
}

void remove_user(char *grpname, char *receiver) // function to remove user from the group
{

    NodeGrp *temp = hg[hashValue(grpname)].key;
    if (temp == NULL)
    {
        printf("No group Found\n"); // checking wheather the group is present or not
        return;
    }
    HashMapGrp *tempmap;
    int flag = 0;
    while (temp != NULL)
    {
        if (cmpareString(grpname, temp->grpName))
        {
            tempmap = temp->hashmapGrp;
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if (flag == 0)
    {
        printf("There is no grp with this name\n");
        return;
    }
    flag = 0;
    UsersList *ul; // ul holds the list of users
    for (int i = 0; i < 10; i++)
    {
        if (tempmap != NULL)
        {
            if (cmpareString(USER, tempmap->admin)) // checking weather the admin is removing persons from the group
            {
                flag = 1;
                ul = tempmap->user_list;
                break;
            }
        }
    }
    if (flag == 0)
    {
        printf("You are nat a Admin to remove the Group\n");
        return;
    }
    if (ul == NULL)
    {
        printf("No User Found to remove ;(\n"); // represent the list contains no users
        return;
    }
    UsersList *dealc = ul, *pre = ul;
    if (cmpareString(ul->user_List, receiver))
    {
        ul = ul->next;
        free(dealc);
        tempmap->user_list = ul;
        temp->hashmapGrp = tempmap;
        hg[hashValue(grpname)].key = temp;
        printf("Removed SuccussFully :)\n");
    }
    else
    {
        while (dealc != NULL)
        {
            if (cmpareString(dealc->user_List, receiver))
            {
                pre->next = dealc->next;
                free(dealc);
                tempmap->user_list = ul;
                temp->hashmapGrp = tempmap;
                hg[hashValue(grpname)].key = temp;
                printf("Removed SuccussFully :)\n");
                return;
            }
            pre = dealc;
            dealc = dealc->next;
        }
        printf("User Not Found:(\n");
    }
}



void login_fun()
{
    int boolean = 1;
    char c[100];
    printf("Enter UserName To Login:-\n");
    while (boolean)
    {
        scanf("%s", c);
        if (containsUser(c))
        {
            printf("Logged in successFully :)\n\n\n");
            strcpy(USER, c);
            boolean = 0;
        }
        else
        {
            printf("No Account found :(\n");
            printf("Please Enter a Valid User Name!!!\n");
        }
    }

    displayTable();
}
void loginGrp()
{
    printf("Enter GroupName\n");
    char grp_name[50], mes[100];
    void *ptr;
    time_t time;
    int boolean = 1, tc;

    scanf("%s", grp_name);
    if (check(grp_name) == 1)
    {
        printf("No Group Available:(\n");
        return;
    }
    else if (check(grp_name) == 2)
    {
        printf("You Are Not in the Group\n");
        return;
    }
    else
    {
        while (boolean)
        {
            printf("To Send Message Enter 1:-\n");
            printf("To View Message Enter 2:-\n");
            printf("To Exit Enter         3:-\n");
            scanf("%d", &tc);
            if (tc == 1)
            {
                char *time_str = ctime(&time);
                time_str[strlen(time_str) - 1] = '\0';
                scanf("%s", mes);
                Grp_mes *tem = (Grp_mes *)malloc(sizeof(Grp_mes));
                strcpy(tem->mes, mes);
                strcpy(tem->username, USER);
                strcpy(tem->time, "time");
                ptr = tem;
                sendGrpMes(grp_name, ptr);
                disGrpMes(grp_name);
            }
            else if (tc == 2)
            {
                disGrpMes(grp_name);
            }
            else if (tc == 3)
            {
                break;
            }
            else
            {
                printf("Please Provide a Valid Nummber:-\n");
            }
        }
    }
}
int grpChat()
{
    printf("<=> Welcome To GroupChat <=>\n\n");
    int n, boolean = 1, tc;
    char grp[50], usr[50];

    while (boolean)
    {
        printf("To Login in Group Enter         1:-\n");
        printf("To View Available Group For You 2:-\n");
        printf("To Create Group Enter           3:-\n");
        printf("To Add Persons to Group Enter   4:-\n");
        printf("To Remove Persons Enter         5:-\n");
        printf("To logout Enter                 6:-\n");
        printf("To Go to Personal Chat Enter    7:-\n");
        printf("To Terminate WhatsApp Enter     8:-\n");

        scanf("%d", &n);
        if (n == 1)
        {
            loginGrp();
        }
        else if (n == 2)
        {
            availabeGrp();
        }
        else if (n == 3)
        {
            printf("Enter Grp Name:-\n");
            scanf("%s", grp);
            boolean = createGrp(grp);

            while (boolean)
            {
                printf("To Add Group-Members Enter    1:-\n");
                printf("To View Available Users Enter 2:-\n");
                printf("To End Enter 3:-\n");
                scanf("%d", &tc);
                if (tc == 1)
                {
                    printf("Enter User Name:-\n");
                    scanf("%s", usr);
                    adduser(grp, usr);
                }
                else if (tc == 2)
                {
                    displayTable();
                }
                else if (tc == 3)
                {
                    break;
                }
                else
                {
                    printf("Enter a Valid Number\n");
                }
            }
            boolean = 1;
        }
        else if (n == 4)
        {
            printf("Enter Grp Name:-\n");
            scanf("%s", grp);
            printf("Enter User Name:-\n");
            scanf("%s", usr);
            add_users(grp, usr);
        }
        else if (n == 5)
        {
            printf("Enter Grp Name:-\n");
            scanf("%s", grp);
            printf("Enter User Name:-\n");
            scanf("%s", usr);
            remove_user(grp, usr);
        }
        else if (n == 6)
        {
            log_out();
        }
        else if (n == 7)
        {
            perChat();
            return 0;
        }
        else if (n == 8)
        {
            return 1;
        }
        else
        {
            printf("Enter a Valid Number\n\n");
        }
    }
}
void perChat()
{
    int boolean = 1;
    char c[50];
    while (boolean)
    {
        printf("Enter Your Friend's User Name\n");
        scanf("%s", c);
        if (cmpareString(c, USER))
        {
            printf("Sender Receiver Cannot be the Same Person\n\n");
        }
        else if (!containsUser(c))
        {
            printf("No Account found!!!\n");
        }
        else
        {
            printf("Enjoy Your Chatting!:)\n\n\n");
            strcpy(res_user, c);
            break;
        }
    }
}

void log_out()
{
    strncpy(USER, "", strlen(USER));
    strncpy(res_user, "", strlen(res_user));

    printf("To Login new User and Receiver Enter 1:-\n");
    printf("To Teminate whatsApp enter           2:-\n");

    int tc;
    scanf("%d", &tc);

    if (tc == 1)
    {
        login_fun();
    }
    else
        return;
}

void send_txt()
{
    void *ptr;
    char c[10];
    int tc = 0;
    time_t t;

    List *ob = (List *)malloc(sizeof(List));
    printf("Enter the message to send:-\n");
    //scanf("%s", mes);
    fgets(mes,99,stdin);
    time(&t);
    strcpy(mes, encode(mes));

    strcpy(ob->message, mes);
    strcpy(ob->time, ctime(&t));
    strcpy(ob->u_name, USER);
    ptr = ob;
    sendMes(ptr);
    displayMes();
    return;
}

int main()
{

    int boolean = 1, itr;
    char c[10];
    char userName[50];

    printf("<=> !!!Welcome to whatsapp!!! <=>\n\n");
    HashSet *hs = newmalloc(10);
    HashGrp *hgs = mallocHashGrp(10);

    printf("Create Sample Accounts\n\n");
    while (boolean)
    {
        printf("To Stop creating Account Enter 0\n");
        printf("Enter UserName to add:)\n");

        scanf("%s", c);
        if (*c == '0')
        {
            break;
        }
        add(c);
    }

    login_fun();
    boolean = 1;

    while (boolean)
    {
        printf("To Personal Chat Enter 1:-\n");
        printf("To Group Chat Enter    2:-\n");
        scanf("%d", &itr);
        if (itr == 1)
        {
            perChat();
            break;
        }
        else if (itr == 2)
            if (grpChat())
            {
                return 0;
            }
            else
                break;
        else
            printf("Enter a valid Number!!!\n");
    }

    while (boolean)
    {
        printf("To Send Message Enter       1:-\n");
        printf("To View Message Enter       2:-\n");
        printf("To Search Messages Enter    3:-\n");
        printf("To Delete Message Enter     4:-\n");
        printf("To Block User Enter         5:-\n");
        printf("To Unblock User Enter       6:-\n");
        printf("To Message other User Enter 7:-\n");
        printf("To LogOut Entirely Enter    8:-\n");
        printf("To GoTo Group Enter         9:-\n");
        printf("To Terminate whatsapp Enter 10:-\n");

        int tc;
        scanf("%d", &tc);

        if (tc == 1)
        {
            send_txt();
        }
        else if (tc == 2)
        {
            displayMes();
        }
        else if (tc == 3)
        {
            char tem[100];
            printf("Enter Message to Search:-\n");
            scanf("%s", tem);
            searchMes(tem);
        }
        else if (tc == 4)
        {
            char delmes[100];
            printf("Enter Message to Delete\n");
            scanf("%s", delmes);
            del_mes(delmes);
        }
        else if (tc == 5)
        {
            block_user();
        }
        else if (tc == 6)
        {
            unblock_user();
        }
        else if (tc == 7)
        {
            perChat();
        }
        else if (tc == 8)
        {
            log_out();
            while (boolean)
            {
                printf("To Personal Chat Enter 1:-\n");
                printf("To Group Chat Enter    2:-\n");
                scanf("%d", &itr);
                if (itr == 1)
                {
                    perChat();
                    break;
                }
                else if (itr == 2)
                    if (grpChat())
                    {
                        return 0;
                    }
                    else
                        break;
                else
                    printf("Enter a valid Number!!!\n");
            }
        }
        else if (tc == 9)
        {
            if (grpChat())
            {
                return 0;
            }
        }
        else if (tc == 10)
            break;
        else
        {
            printf("Please Enter valid Number!!!\n");
        }
    }

    displayGrp();
}
