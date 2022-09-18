#include <data.h>
#include <stdio.h>
#include <stdlib.h>

int convertCharToNumber(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else {
    return -1;
  }
}

char convertNumberToChar(int n) {
  if (n >= 0 && n <= 9) {
    return n + '0';
  } else if (n >= 10 && n <= 15) {
    return n - 10 + 'A';
  } else {
    return 0;
  }
}

//Returns a^b Using bits.
int power(int a, int b){
    int res=1;
    //Initialise answer to 1 
    
    while(b>0){
        
        //For every set bit, change res 
        if(b&1) res*=a;
        
        //Modify a for every bit in b
        a*=a; 
        
        b>>=1;
        //Traverse through bits.
        
    }
    return res;
    //Completed in log(n) time.
}


Data convert_to_base_n(Data src, unsigned char n) {
  
  Data new_data;
  new_data.base= n;
  new_data.sign= src.sign;
  new_data.number_bits=src.number_bits;
  //new_data is of type Data to be returned.
  //Set the basic unchanging values
  
  //src.data is assigned to x in base 10
  int x= convert_for_base(src), i=0;

  //first is the head node 
  DataNode *first;
  first = (DataNode *)malloc(sizeof(DataNode));
  first = NULL;
  //It has src.data converted to base n

  unsigned char s[32];
  s[0]= convertNumberToChar(0);
  /*
  For the case of 0, x becomes 0
  and thus x can't enter while loop,
  and its every base form is 0 ,  
  so for the special case, we reserve first index for the 0 case.
  */
  
  while(x){
    //x is of integer datatype
    //s is an unsigned char array that stores the src.data in base n representation.
    
    s[i++]= convertNumberToChar(x%n);
    x/=n;
    
  }
    
    
  if(i==0) 
    i++;
  //For src.data = 0 length should be made 1 (manually)    
    
  new_data.len = i;
  //Updata length in new_data
  //The length of its nth base representation.
    
    
  while(i--){
    //We have the data stored in s 
    //But the data is reversed.
      
    DataNode *nn,*temp;
    nn = (DataNode *)malloc(sizeof(DataNode));
    //Create a new DataNode that stores the data for every i > 0 
    
    nn->number = s[i];
    nn->next = NULL;
    //Initialise it to the last unused character in s 
    
    if(first==NULL){ 
      //Update the head if the head is NULL 
      first=nn;
    }
    else{
        
      //for every new characeter stored in s,
      //Add that to the linked list at the end.
      temp = first;
      while(temp->next != NULL) 
        temp = temp->next;
        
      temp->next = nn; 
      // nn is linked at the end. 
        
    } 
  }
  
  new_data.data = first;
  //Update the new_data.src to the head DataNode
  //head ~ first
  //It has data of base n
  
  return new_data;

}



/*
//Helper Method

  This function simply returns a non-negative integer value,
  It doesn't look for signed bit,
  This method is only used for easy convertion of numbers from one base to the other.
*/

int convert_for_base(Data src){
    
    int ans=0, multiplier= power(src.base,src.len-1) ;
    //multiplier returns maximum base ^ log(src.data , base) i.e, 
    //It returns the maximum power of base <= src.data
    
    //Traverse through data to convert a number of one base to the integer with base = 10
    for(DataNode *node = src.data; node; node=node->next) {
        
      ans+=convertCharToNumber(node->number)*multiplier;
      multiplier/=src.base;    
    
    }
    
    return ans;
}

int convert_to_int(Data src) {
      
    //Converts to integer: "considers sign"  
    int ans= convert_for_base(src);
    //ans is integer representation of number in src.data 
    
    Data binary = convert_to_base_n(src, 2);
    //binary contains the data containing the binary representation of src.data 
    
    unsigned char one = convertNumberToChar(1), zer = convertNumberToChar(0);
    //one  = '1'  zer = '0'
    
    if(src.sign == 1 &&   
      binary.len == src.number_bits && 
      binary.data -> number == one){
          
        //When MSB is a set bit and sign = 1
        
        
        
        //1's complement of the binary number
        for(DataNode *node = binary.data; node; node=node->next) {
            
          if(node -> number == one) 
            node -> number = zer;
            
          else 
            node -> number = one;
            
        //Swap 0 -> 1 and 1 -> 0 
        }
        
        //1's complement + 1 = 2's complement 
        //If sign =1 then the number is negative 
        
        ans = -1 * ( 1 + convert_for_base(binary));
          
    }
    
    //For sign = 0 ans is +ve as initialised.
    return ans;
}

Data left_shift(Data src, int n) {
    
  //head contains the binary data
  Data head = convert_to_base_n(src,2);
  
  Data new_data;
  
  new_data.base= 2;
  //Base should be changed to 2 
  
  new_data.sign= head.sign;
  new_data.number_bits= head.number_bits;    
  
  DataNode *var = head.data;
  //var points to the binary representation of src.data 
  
  while(var->next != NULL){ 
      
      //Iterate to the last block of binary data
      var = var->next;
  }

  //new length exceeds by n
  new_data.len= head.len + n;
    
    
  //Add 0's to the end of list.
  while(n--){
      
      DataNode *nn = (DataNode *)malloc(sizeof(DataNode));
      nn -> number = convertNumberToChar(0);
      //Creates a new node for every shift 
      //Initialise the new node nn 
      nn -> next = NULL;
      
      //Link the newly created node to the end of var 
      var -> next = nn;
      //Point the var to the end.
      var = var -> next;
      
  }

  
  /*
  
    If the number of bits after the operation is greater than number of bits allowed to the initial src,
    Truncate the extra bits in the binary data (head),
    but it's given that a number shouldn't start with 0's,
    Truncate the 0's if the data has initial zeroes.
    
    If the data is 0 -> NULL 
    then the data should not be altered.
    
  */
  while(head.data->next != NULL && 
       (new_data.len > src.number_bits || head.data->number==convertNumberToChar(0))) {
           
      head.data = head.data -> next;
      new_data.len--;
      //new length is modified 
    }

  //data is updated in the new_data
  //head contains the n-left shifted binary representation of src.data
  new_data.data = head.data;  
  
  
  return new_data;
}


Data right_shift(Data src, int n) {
  
  Data head = convert_to_base_n(src, 2);
  //head contains binary number equivalent to src.data 
  
  Data new_data;
  new_data.base= 2;
  //The data is now binary with base = 2 
  
  new_data.sign = src.sign;
  
  //number_of_shifts is the integer representing the maximum count,
  //the binary number should itertate through.
  int number_of_shifts = head.len - n -1;
  
  //new length becomes head.len - n (n < head.len)
  new_data.len  = 1+number_of_shifts;
  
  new_data.number_bits = src.number_bits;
  //number of bits remain same.
  
  DataNode *var = head.data;
  while(number_of_shifts > 0){

      //The rightmost n bits are truncated by making adress = NULL
      var = var -> next;
      number_of_shifts--;
  }
  var -> next = NULL;
  
  if(n >= head.len && (!src.sign || (src.sign && head.number_bits != head.len) )){
      
      //If n >= length of data then data becomes 0
      //and length is no more than 1.
      //All the bits have been shifted
      var -> number = convertNumberToChar(0);
      new_data.len = 1;
    
  }
  
  
  else if(src.sign && head.number_bits == head.len){

    //If it's a signed value, 
    //Arithmetic shift is implemented 
    //Length is unchangwd. 
    new_data.len = head.len;
    
  }


  int min = (n < head.number_bits-1)? n: head.number_bits-1;
  //min keeps the count of number of 1's
  //that are to be added at the start of linkedList

  while(src.sign && min && head.number_bits == head.len) {
    
    //Add 1's to the linkedList in Arithmetic right shift fashion
    DataNode *nn = (DataNode *)malloc(sizeof(DataNode));
    nn -> number = convertNumberToChar(1);
    nn -> next = head.data;
    head.data = nn;
    min--;
    
  }
   //updating right shift
  
  
  //Update the data 
  new_data.data = head.data;
  
  return new_data;
}
