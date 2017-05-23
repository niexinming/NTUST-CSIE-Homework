/*
 * 加入尾端-逾時?
 * 加入尾端-逾時? - No - 加入隊伍
 * L Yes - 詢問前一人可以讓我插隊嗎？ 不行或是插了仍然逾時就閃人
 * 插隊規則 如果A被差會造成A逾時 那不允許插隊
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Customer {
    private:
        int counter()
        {
            static int cn = 0;
            return ++cn;
        }

    public:
        int id;
        int arrive_time;
        int serve_time;
        int allow_wait_time;
        bool departed;
        Customer *previous;

        Customer(int arr_time, int serv_time, int allw_time, Customer *prev) :
            arrive_time(arr_time),
            serve_time(serv_time),
            allow_wait_time(allw_time),
            previous(prev),
            departed(false) {
                this->id = this->counter();
            }

        int wait_time() {
            if(!this->previous) {
                return this->serve_time;
            } else {
                return this->serve_time + this->previous->wait_time();
            }
        }

        void inspect() {
            printf("(#%d, %d, %d, %d)\n",
                    this->id, this->arrive_time, this->serve_time, this->allow_wait_time);
        }

        bool accept_cutin(Customer from) {
            if(this->)
        }
};

int main()
{
    vector<Customer*> customers;
    Customer *prev = NULL;
    int arr, ser, allw;
    while(true) {
        cout << "input arrtive_time serve_time allow_wait_time: (-1 for end) \n";
        if(! (cin >> arr))
            break;
        if(arr == -1)
            break;
        if(! (cin >> ser >> allw))
            break;

        Customer *cust = new Customer(arr, ser, allw, prev);
        cust->inspect();
        prev = cust;
        customers.push_back(cust);
    }


    return 0;
}
