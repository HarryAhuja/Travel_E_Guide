#include"define.h"

typedef struct HOTEL
{
    int price;
    int rating;
    int reservation[100];
}hotel;

hotel hotel_db[40000];

typedef struct NODE
{
    int bid;
    struct NODE *next;
}node;

node *area[100][100];

void init(int m)
{

}

void add_bnb(int bid, int y,int x, int price, int rating)
{
    hotel_db[bid].price  = price;
    hotel_db[bid].rating = rating;

    // Add node to aread[x][y] suc that list sorted according to criteria
}

int reserve(int bid, int cday, int nights)
{
    int i = cday;
    while(i<cday+nights)
    {
        if(hotel_db[bid].reservation[i] != 0)    break;
        i++;
    }

    if(i == cday+nights)
    {
        hotel_db[bid].reservation[cday] = 2;

        i = cday+1;
        while(i<cday+nights)
        {
            hotel_db[bid].reservation[i] = 1;
            i++;
        }

    }
    else    return 0;
}

int cancel(int bid, int cday)
{
    if(hotel_db[bid].reservation[cday]!=2)  return 0;

    hotel_db[bid].reservation[cday] = 0;

    int i = cday+1;

    while(i<cday+nights)        // doubt on end point
    {
        if(hotel_db[bid].reservation[i] != 0 || hotel_db[bid].reservation[i] != 2)
        {
            hotel_db[bid].reservation[i] = 0;
        }
        i++;
    }
}

int search(int option, int y, int x, int cday, int nights, top_bid[5])
{
    if(option == 0)
    {
        //TODO
    }
    else
    {
        int top_hotels[9][5];
        //TODO
    }
}