// Implementation of the templated Vector class
// ECE 6122 lab 3
// Anisha Gartia

#include <iostream> // debugging
#include <stdlib.h>
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
	: elements(NULL)
	, count(0)
	, reserved(0)
{
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
	//: count(rhs.count)
	//, reserved(rhs.reserved) 
{
  count = rhs.count;
  reserved = rhs.reserved;
  elements = (T*)malloc(reserved * sizeof(T));
  for (int i = 0; i<count; i++){
    new(&elements[i]) T(rhs.elements[i]);
  }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  if(count > 0){
    for (int i=0; i<count; i++){
      elements[i].~T();
    }
    free(elements);
    reserved = 0;
    count = 0;
  }
  if (reserved > 0){
    free(elements);
    reserved = 0;   
  }
  elements = (T*)malloc(rhs.reserved * sizeof(T));
  reserved = rhs.reserved;
  for (int i=0; i<rhs.count; i++){
    new(elements+i) T(rhs.elements[i]);
  } 
  count = rhs.count;

  return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
//agnote: What does this do
// Creates a vector with unfilled memory spaces for nReserved elements
template <typename T>
Vector<T>::Vector(size_t nReserved)
	: count(0)
	, reserved(nReserved)
{ // Initialize with reserved memory
  elements = (T*)malloc(nReserved * sizeof(T));
}

// Contruct a vector with n copies of t
template <typename T>
Vector<T>::Vector(size_t n, const T& t)
	: reserved(n)
	, count(n)
{ // Initialize with "n" copies of "t"
  elements = (T*)malloc(n * sizeof(T));
  for (int i=0; i<n; i++){
    new(elements+i) T(t);
  }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory     
  if (count >0){
    T* newElements = (T*)malloc(n * sizeof(T));
    for (int i=0; i<count; i++){
      new(newElements+i) T(elements[i]);
      elements[i].~T();
    }   
    free(elements);
    elements = newElements;
    reserved = n;
  }
  else if (reserved > 0){
    free(elements);
    elements = (T*)malloc(n * sizeof(T));
    reserved = n;
  }
  else{
    elements = (T*)malloc(n * sizeof(T));
    reserved = n;
  }
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  for (int i=0; i<count; i++){
    elements[i].~T();
  }
  free(elements);
  count = 0;
  reserved = 0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
  if (reserved > count){
    new(elements+count) T(rhs);
    count++;
  }   
  else if(reserved == 0){
    elements = (T*)malloc(sizeof(T));
    new(elements) T(rhs);
    reserved++;
    count++;
  }
  else {
    reserved = reserved+1;
    T* newElements = (T*)malloc(reserved * sizeof(T));
    for (int i = 0; i< count; i++){
      new(newElements+i) T(elements[i]);
      elements[i].~T();
    }
    free(elements);
    new(newElements+count) T(rhs);
    elements = newElements;
    count++;
  }
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
  if (reserved > count){
    new(elements+count) T(elements[count-1]);
    for (int i = count-2; i >= 0; i--){
      elements[i+1] = elements[i];
    }
    elements[0] = rhs;
    count++;
  }
  else if(reserved == 0){
    elements = (T*)malloc(sizeof(T));
    new(elements) T(rhs);
    reserved++;
    count++;
  }
  else {
    reserved = reserved + 1;
    T* newElements = (T*)malloc(reserved* sizeof(T));
    new(newElements) T(rhs);
    for (int i = 0; i< count; i++){
      new(newElements+i+1) T(elements[i]);
      elements[i].~T();
    }
    free(elements);
    count++;
    elements = newElements;
  }
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
  elements[count-1].~T();
  count--; 
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
  T* newElements = (T*)malloc((reserved) * sizeof(T)); 
  elements[0].~T();
  for (int i = 0; i< count-1; i++){
    new(newElements+i) T(elements[i+1]);  
    elements[i+1].~T();
  }
  free(elements);
  elements = newElements;
  count--;
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  return *elements;  
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  return *(elements+count-1);
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{ // const element access
  return *elements[i];  
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{//nonconst element access
  return *(elements+i);
}

template <typename T>
size_t Vector<T>::Size() const
{
 return count; 
}

template <typename T>
bool Vector<T>::Empty() const
{
  bool retVal;
  if(count == 0){
    retVal = true;
  }
  else{
    retVal = false;
  }
  return retVal;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  for (int i=0; i<count; i++){
    elements[i].~T();
  }
 count = 0; 
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
  size_t i = 0;
  while((elements+i) != it.current){     
    ++i;
  }
  for (size_t j = i; j<count-1; j++){
    elements[j].~T();
    new(elements+j)  T(elements[j+1]);
  }
  elements[count-1].~T();
  count--;
  

/*
  T* newElements = (T*)malloc((reserved) * sizeof(T));
  size_t i = 0;
  while((elements+i) != it.current){
    new(newElements+i) T(elements[i]);
    elements[i].~T();
    ++i;
  }
  elements[i].~T();
  for(size_t j = i; j<count-1; j++){
    new(newElements+j) T(elements[j+1]);
    elements[j+1].~T();
  }
  free(elements);
  elements = newElements;
  count--;
*/
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
  if(count < reserved){
    size_t i = count-1;
    new(elements+count) T(elements[count-1]);
    while((elements+i) != it.current){
      elements[i] = elements[i-1];     
      --i;
    }
    elements[i] = rhs;
    count++;
  }
  /*
  else if (count >= reserved){
    const T temp = elements[count-1];
    size_t i = count -1;
    while((elements+i) != it.current){
      elements[i] = elements[i-1];
      --i;
    }
    Push_Back(temp);
    elements[i] = rhs;
  }
  */
  else if (count >= reserved){
    T* newElements = (T*)malloc((count+1)*sizeof(T));
    size_t i = 0;
    while((elements+i) != it.current){
      new(newElements+i) T(elements[i]);
      elements[i].~T();
      i++;
    }
    new(newElements + i) T(rhs);
    i++;
    for(size_t j =i; j<= count; j++){
      new(newElements+i) T(elements[i-1]);
      elements[i-1].~T();
      i++;
    }
  free(elements);
  elements = newElements;
  count++;
  }
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
	: current(NULL)
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
  //current = (T*)malloc(sizeof(T));
  current = c;
  //cout << "cc " << c << endl;
  //new(current) T(*c);
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
 // current = (T*)malloc(sizeof(T));
 // new(current) T(*rhs.current);  
  current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++()
{
  current = current + 1;
  //current = current + sizeof(T);
  return *this; 
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
//  T* retVal = (T*)malloc(sizeof(T));
//  new(retVal) T(*current);

  VectorIterator<T> retVal = *this;
  //cout << "++ before " << current << endl;
  //current = current + sizeof(T);
  //free(current);
  //new(current) T(*(&retVal+sizeof(T))); 
  
  current = current + 1;
  //cout << "++ after " << current << endl;
  //current = current + sizeof(T);
  return retVal; 
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
  bool retVal = false;
  if ((current - rhs.current) == 0){
    retVal = false;
  }
  else{
    retVal = true;
  }
  return retVal; 
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
  bool retVal;
  if ((current - rhs.current) == 0){
    retVal = true;
  }
  else{
    retVal = false;
  }
  return retVal; 
}




