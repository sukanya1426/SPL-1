#include<stdio.h>

void keyExpansion(){

}
void initialRound(){

}
void subBytes(){

}
void shiftRows(){

}
void mixColumns(){

}
void addRoundkey(){

}

void aes_encrypt(){

    int numberOfRound = 1;
    keyExpansion();
    initialRound();
    for(int i=0;i<numberOfRound;i++){
        subBytes();
        shiftRows();
        mixColumns();
        addRoundkey();
    }
    //final round 
    subBytes();
    shiftRows();
    addRoundkey();

}



int main()
{
     char message[] = " This is a txt file ";
     char key[16]={
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16,
     };

     aes_encrypt();


}