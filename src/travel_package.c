#include<stdio.h>
#include"define.h"


typedef struct USER
{
    int friend_list[MAX_FRIENDS];
    int friend_count;
}user;

typedef struct PACKAGE
{
    int pid;
    int price;
    int area;
    struct PACKAGE *next;
    struct PACKAGE *prev;
}package;

user user_db[MAX_N];

package pkg_pool[MAX_PACKAGES];
int     pool_cnt;
package *area_list[MAX_M];

package *new_package_node()
{
    package *node = &pkg_pool[pool_cnt++];
    node->next  = NULL;
    node->prev  = NULL;
    node->pid   = 0;
    node->price = 0;

    return node;
}

int hash(int pid)
{
    return pid%MAX_PACKAGES;
}

int tb[MAX_PACKAGES];
int chain[MAX_PACKAGES];

void add_to_hash(int key, int data)
{
    int h = hash(key);
    chain[data] = tb[h];
    tb[h] = data;
}

int find_in_hash(int key)
{
    int h = hash(key);
    int k = tb[h];

    while(k!=-1)
    {
        if(pkg_pool[k].pid == key)  return k;

        k = chain[k];
    }

    return -1;
}

int fav[MAX_N][MAX_M];
int N, M;
int list[MAX_M];
int price1[MAX_M];
int priority[MAX_M];
int count[MAX_M];

void init(int n, int m)
{
    N        = n+1;
    M        = M+1;
    pool_cnt = 0;

    register i,j;
    for(i = 0; i<MAX_PACKAGES;i++)
    {
        pkg_pool[i].next  = 0;
        pkg_pool[i].prev  = NULL;
        pkg_pool[i].pid   = 0;
        pkg_pool[i].area  = 0;
        pkg_pool[i].price = MAX_PACKAGES;
        
        tb[i]    = -1;
        chain[i] = -1;
    }

    for(i = 0;i<M;i++)
    {
        area_list[i] = new_package_node();
    }

    for(i = 0;i<N;i++)
    {
        user_db[i].friend_count = 0;
        
        for(j = 0;j<M;j++)
        {
            fav[i][j] = 0;
        }
    }
}

void be_friend(int uid1,int uid2)
{
    int a = user_db[uid1].friend_count;
    int b = user_db[uid2].friend_count;

    user_db[uid1].friend_list[a] = uid2;
    user_db[uid2].friend_list[b] = uid1;

    user_db[uid1].friend_count += 1;
    user_db[uid2].friend_count += 1;
}

void add(int pid, int area, int price)
{
    int data = pool_cnt;

    add_to_hash(pid,data);

    package *node = new_package_node();
    node->pid = pid;
    node->price = price;
    node->area = area;

    package *head = area_list[area];
    package *curr = head;
    package *curr2;

    if(curr->next != NULL)
    {
        curr = curr->next;
        bool insert = false;

        while(curr->next != NULL)
        {
            if(curr->price<node->price)
            {
                curr = curr->next;
            }
            else if( curr->price>node->price)
            {
                curr2       = curr->prev;
                curr2->next = node;
                node->prev  = curr2;
                node->next  = curr;
                curr->prev  = node;

                insert = true;
                break;
            }
            else
            {
                if(curr->pid<node->pid)
                {
                    curr = curr->next;
                }
                else
                {
                    curr2 = curr2->prev;
                    curr2->next = node;
                    node->prev = curr2;
                    node->next = curr;
                    curr->prev = node;
                    insert = true;
                    break;
                }
            }
        }

        if(insert == false)
        {
            if( (curr->price< node->price) || 
                ( (curr->price == node->price) && (curr->pid<node->pid)  )
              )
            {
                curr->next = node;
                node->prev = curr;
            }
            else
            {
                curr2 = curr->prev;
                curr2->next = node;
                node->next = curr;
                curr->prev = node;
                node->prev = curr2;
            }
        }
    }
    else
    {
        curr->next = node;
        node->prev = curr;
    }
}

void reserve(int uid,int pid)
{
    int val  = find_in_hash(pid);
    int area = pkg_pool[val].area;

    fav[uid][area]+=1;

    package *node  = &pkg_pool[val];
    package *prev1 = node->prev;
    prev1->next = node->next;

    if(node->next)  node->next->prev = prev1;

    return;
}

int recommend(int uid)
{
    int i,j;
    for(i = 0; i<M;i++)
    {
        count[i] = 0;
    }

    int num_friends = user_db[uid].friend_count;
    
    for(i = 0;i<num_friends;i++)
    {
        for(j = 0; j<M; j++)
        {
            count[j] += fav[user_db[uid].friend_list[i]][j];
        }
    }

    for(i = 0; i<M; i++)
    {
        count[i] += fav[uid][i];
    }

    for(i = 1;i<M;i++)
    {
        if(area_list[i]->next!=NULL)
        {
            list[i]   = area_list[i]->next->pid;
            price1[i] = area_list[i]->next->price;
        }
        else
        {
            list[i]   = -1;
            price1[i] = 1001;
        }
    }

    for(i = 1;i<M;i++)
    {
        int cnt = 0;
        for(j = 1;j<M; j++)
        {
            if( i == j)             continue;

            if(count[i]<count[j])    cnt++;
        } 
        priority[i] = cnt;
    }

    int ans_pid = 0, ans_priority = 12, ans_price = 1001;

    for(i = 1; i<M;i++)
    {
        if(list[i] == -1)   continue;

        if(ans_priority>priority[i])
        {
            ans_pid      = list[i];
            ans_priority = priority[i];
            ans_price    = price1[i];
        }
        else if(ans_priority == priority[i])
        {
            if(ans_price > price1[i])
            {
                ans_pid      = list[i];
                ans_priority = priority[i];
                ans_price    = price1[i];
            }
            else if(ans_price == price1[i] && ans_pid>list[i])
            {
                ans_pid      = list[i];
                ans_priority = priority[i];
                ans_price    = price1[i];
            }
        }
    }

    return ans_pid;
}


