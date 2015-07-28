// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
// Simple distructor
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {

   // just erase everything by poping everything off using built in
   // empty function
   while (!empty()){
      erase(begin()); // function implmented in .h file
   }
   TRACE ('l', (void*) this);
}


//
// iterator listmap::insert (const value_type&)
// insert argument
// TODO
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);

   // pointer that to make things inserted properly
   listmap<Key,Value,Less>::iterator it = begin();

   // check if the list is empty, and handle that case first
   if (empty()){
      node* tmp = new node(anchor(), anchor(), pair);
      // set the anchor pointers
      it.where->prev = tmp;
      it.where->next = tmp;
      return iterator(tmp);
   }

   // iterate over list and insert where appropriate
   while (it != end()){
      // if the key is already here, overwrite it, and return our it
      if (pair.first == it->first){
         it->second = pair.second;
         return it;
      }

      // if our value is less than the current one, insert it as
      // appropriate
      if(less(pair.first, it->first)){
         // make new node
         node* tmp = new node(it.where, it.where->prev, pair);
         // reassign pointers
         it.where->prev->next = tmp;
         it.where->prev = tmp;
         // return the tmp node as an iterator
         return iterator(tmp);
      }
      ++it;
   }

   // oops! we've reached the end, time to append it to the end
   node* tmp = new node(anchor(), it.where, pair);
   it.where->next->prev = tmp; // technically sets anchor's prev
   it.where->next = tmp;
   return iterator(tmp);
}

//
// listmap::find(const key_type&)
//
// TODO
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);
   // start off by making an iterator that points at the front
   listmap::iterator it = begin();


   // iterate down the list until you find a key that matches the key
   // being searched for, or reach the end
   while (it != end()){
      if (it->first == that) break;
      ++it;
   }
   return it;
}

//
// iterator listmap::erase (iterator position)
// TODO
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {

   // make temp iterator pointing at position
   listmap::iterator it = position;
   ++it; // move it up a notch

   // use iterator erase function
   position.erase();

   TRACE ('l', &*position);
   return it; // return the iterator
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}


// IMPLEMETED ERASE FUNCTION HERE!!!
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase(){
   // error handling incase of nullptr
   if (where == nullptr){
      // TODO: throw exception
      return;
   }
   // reassign pointers
   where->next->prev = where->next;
   where->prev->next = where->prev;
   // free the memory!
   delete where;
}
