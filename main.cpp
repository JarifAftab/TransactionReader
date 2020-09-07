#include <iostream>
#include"Transaction.hpp"
#include"History.hpp"
#include "History_Transaction_definitions.hpp"
unsigned int Transaction::assigned_trans_id = 0;
int main() {
//Transaction myTrans {"ticker",23,5,2019,true,5,60};
//Transaction myTrans2 {"ticker",22,5,2019,true,5,60};
History H{};
  

  

H.read_history();
//std::cout<<findCapacity(H.get_p_head());
H.sort_by_date();
//H.sort_by_date();
H.update_acb_cgl();
H.print();
std::cout<<"The CGL for 2018 is:"<<H.compute_cgl(2018)<<std::endl;
std::cout<<"The CGL for 2019 is:"<<H.compute_cgl(2019)<<std::endl;

H.~History();





}