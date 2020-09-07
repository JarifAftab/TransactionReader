#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
  Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
              unsigned int month_date, unsigned year_date, bool buy_sell_trans,
              unsigned int number_shares, double trans_amount) {
    symbol=ticker_symbol;
    day=day_date;
    month = month_date;
    year = year_date;
    amount=trans_amount;
    shares = number_shares;
    cgl = 0;
    share_balance = 0;
    acb_per_share = 0;
    acb = 0;

    if(buy_sell_trans == true){
      trans_type = "Buy";
    }
    else{
      trans_type = "Sell";
    }
    trans_id = assigned_trans_id;
    assigned_trans_id+=1;
    p_next = nullptr;
    

  }



// Destructor
// TASK
//
 Transaction::~Transaction() {
   
  }


// TASK
// Overloaded < operator.
//
 bool Transaction::operator<(Transaction const &other) {
    if(this->get_year() < other.get_year()){ //Checks if the year is less return true
      return true;
    }
    else if(this->get_year() == other.get_year() ){ //If years are the same, check the months

      if(this->get_month() < other.get_month()){ //If months is less return true
        return true;
      }
      else if(this->get_month() == other.get_month()){ //If they are the same, check day next
        
        if(this->get_day()< other.get_day()){
        
          return true;
        }

        else{
          return false;
        }
      }
      else{ //If month is greater immediately return false
        return false;
      }

    }
    else{ //If year is greater, than immedietely return false
      return false;
    }
  }

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
  History::History() {
    p_head = nullptr;
  }


// Destructor
// TASK
//
  History::~History() {

    while(p_head != nullptr){

      Transaction * currentHead = p_head;
      p_head = p_head->get_next();
      delete currentHead;
      currentHead = nullptr;

    }
    

  }



// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
    
    ece150::open_file(); //Getting ready to read file
    while(ece150::next_trans_entry() == true){
      
      Transaction * newTrans = new Transaction{
        
        ece150::get_trans_symbol(),
        ece150::get_trans_day(),
        ece150::get_trans_month(),
        ece150::get_trans_year(),
        ece150::get_trans_type(),
        ece150::get_trans_shares(),
        ece150::get_trans_amount()
      
      };
      //newTrans->print();

      insert(newTrans);
    }
     
     //p_head->get_next()->print();

  }


// insert(...): Insert transaction into linked list.
//
  void History::insert(Transaction *p_new_trans) {

    /*
    p_new_trans->assigned_trans_id++;
    p_new_trans->trans_if= assigned_trans_id;
    */
    if(p_head == nullptr){
      p_head = p_new_trans;
    }
    else{ 
      Transaction * next = p_head; 
      while(1){
        if(next->get_next() != nullptr){
          next = next->get_next();
        }
        else{
          //next->get_next() = p_new_trans;
          next->set_next(p_new_trans);
          break;
        }

      }
    }

  }



// TASK
// sort_by_date(): Sort the linked list by trade date.
//

Transaction * find_largest(Transaction * begin){
  Transaction * iterator = begin;
  Transaction * currentMin = begin;
  while(iterator != nullptr){
    if( *currentMin < *iterator  ){
      currentMin = iterator;
    }
    iterator = iterator->get_next();
  } 
  return currentMin;

}

Transaction * address_before_largest(Transaction * begin, Transaction * end){

    Transaction * iterator = begin;
    if(iterator->get_trans_id() == end->get_trans_id()){
      return iterator;
    }
    else{
      while(iterator->get_next()->get_trans_id() != end->get_trans_id() ){
        iterator = iterator->get_next();
      } 
    }
    return iterator;

}

Transaction * find_fit(Transaction * begin, Transaction * compare){ //returns the beginning of linked list and places the compare in an orderd manner in the linkedlist
  Transaction * sIterator = begin;
  Transaction * lIterator = begin->get_next();
  //compare->set_next(nullptr);

  while((sIterator != nullptr) || (lIterator != nullptr)){
    if(*sIterator < *compare && *lIterator < *compare) {
      sIterator->set_next(compare);
      compare->set_next(lIterator);
      return begin;
    }
    else{
      sIterator->get_next();
      lIterator->get_next();
    }
    
  }
  //This part sees if anywhere from the smallest to largest (non-inclusive) if the element fits

  if(*compare < *begin){
    compare->set_next(begin);
    return compare;
  }
  //If it is smaller than the beginning element, have it point to the beginning and return this as the new beginning
  else{
    sIterator->set_next(compare);//this lags 1 behind Iterator, so this one will be the final entry in the list
    compare->set_next(nullptr);
    return begin;
  }
  //Otherwise, take the end of the list, point it to compare and have compare as the new end



}

int findCapacity(Transaction * p_head){

  Transaction * iterator = p_head;
  int count  = 0;

  while(iterator != nullptr){

    iterator = iterator->get_next();
    count++;

  }
  return count;
  
}

void sortArray(Transaction ** arr, int cap){

    for(int i = 0; i < cap; i++){
    Transaction * minVal = arr[i];
    int minIndex = i;
    for(int j = i + 1; j < cap; j++){

      if(*(arr[j]) < *(minVal)){
        minVal = arr[j];
        minIndex = j;
      }

    }
    arr[minIndex] = arr[i];
    arr[i] = minVal;

  }

}

void History::sort_by_date(){

  int size = findCapacity(p_head);
  Transaction ** arr_ptr = new Transaction * [size];

  Transaction * iterator = p_head;

   for(int i = 0; i < size; i++){

     arr_ptr[i] = iterator;
     iterator = iterator->get_next(); 

   }
   sortArray(arr_ptr, size);
  //This part goes through the array and reconnects head to all the elements into the array
   p_head = arr_ptr[0];
   for(int i = 0; i < size-1; i++){
     
     arr_ptr[i]->set_next(arr_ptr[i+1]);

   }
   //Sets the last element to nullptr

   arr_ptr[size-1]->set_next(nullptr);

}




// TASK
// sort_by_date(): Sort the linked list by trade date.
//



// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//


void History::update_acb_cgl(){

  //sort_by_date();

  double runningACB = 0; //The running total for ACB
  Transaction * iterator = p_head; //The iterator will start at the head

  unsigned int sharesBalance = 0; //The running total for share Balance

  double acbShare = 0;

  double runningCGl = 0;

  while(iterator != nullptr){

    if(iterator->get_trans_type() == 1){

      runningACB +=iterator->get_amount();
      iterator->set_acb(runningACB);

      sharesBalance += iterator->get_shares();
      iterator->set_share_balance(sharesBalance);

      acbShare = runningACB/sharesBalance;
      iterator->set_acb_per_share(acbShare);


      iterator = iterator->get_next();

    }
    else if(iterator->get_trans_type() == 0){
      
      runningACB = runningACB - (iterator->get_shares() * acbShare);
      iterator->set_acb(runningACB);

      sharesBalance -= iterator->get_shares();
      iterator->set_share_balance(sharesBalance);

      runningCGl = iterator->get_amount()- (iterator->get_shares() * acbShare);
      iterator->set_cgl(runningCGl);

      acbShare = acbShare;
      iterator->set_acb_per_share(acbShare);




      iterator = iterator->get_next();
    }

  }
  



}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//

double History::compute_cgl(unsigned int year){

  //update_acb_cgl();

  double netCGL = 0;
  Transaction * iterator = p_head;

  while(iterator != nullptr){
    if(iterator->get_year() == year){

      netCGL += iterator->get_cgl();

    }
    iterator = iterator->get_next();
  }
  return netCGL;


}




// TASK
// print() Print the transaction history.

  void History::print() {
    std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
    Transaction * next = p_head; 
    while(1){
      next->print();
      if(next->get_next() != nullptr){
        next = next->get_next();
      }
      else{
        break;
      }
    }
    std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
  }




// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

//Possible Issue: When adding entry to lnkedlist, it still has connection, so lIterators ends up connecting to those ones and iterates through the orginal.

/*
  Transaction * newHead = nullptr;
  Transaction * tempnode = p_head;
  Transaction *before_min;
  Transaction * current_min = p_head;

  while(tempnode->get_next() != nullptr){
    before_min = tem
  }
  //Find min

  */
  /*
  Transaction * newHead = p_head;
  p_head = p_head->get_next();
  
  

  Transaction * newAdd = newHead; //Thi variable grabs the current value from the old linked list
  newAdd->set_next(nullptr); //Isolates the new addition to the linked list from the old


  while(p_head != nullptr){

    newAdd = p_head; //Grabs next value in linked list
    p_head = p_head->get_next(); //iterate to next item in original list
    newAdd->set_next(nullptr); //disconnects the newAdd from the old linked list
    newHead = find_fit(newHead, newAdd); //orders it in new linked list

    

  }
  p_head = newHead;
  newHead = nullptr;
  */

  /*
  while(p_head != nullptr){

    Transaction * temp = p_head;
    Transaction * i = p_head;

    p_head = p_head->get_next();
    
    temp->set_next(nullptr);

    if(*temp < *i){
      temp->set_next(i->get_next());
      i->set_next(temp);
    }
    else{
      
      while(*(i->get_next()) < *temp ){
        i = i->get_next();
      }

      temp->set_next(i->get_next());
      i->set_next(temp);

    }

  }

*/