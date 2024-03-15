#include <stdio.h>
int main()
{
    printf("2021041055 이한결\n");
    int i;
    int *ptr;
    int **dptr;
    i = 1234;
    printf("[checking values before ptr = &i] \n");
    printf("value of i == %d\n", i);                    //i의 값 출력
    printf("address of i == %p\n", &i);                 //i의 주소값 출력
    printf("value of ptr == %p\n", ptr);                //ptr값 출력
    printf("address of ptr == %p\n", &ptr);             //ptr의 주소값 출력
    ptr = &i; /* ptr is now holding the address of i */
    printf("\n[checking values after ptr = &i] \n");
    printf("value of i == %d\n", i);                    //i의 값 출력
    printf("address of i == %p\n", &i);                 //i의 주소값 출력
    printf("value of ptr == %p\n", ptr);                //ptr의 값(i의 주소값) 출력
    printf("address of ptr == %p\n", &ptr);             //ptr의 주소값 출력
    printf("value of *ptr == %d\n", *ptr);              //ptr이 가르키는 값(i의 값) 출력
    dptr = &ptr; /* dptr is now holding the address of ptr */
    printf("\n[checking values after dptr = &ptr] \n");
    printf("value of i == %d\n", i);                    //i의 값 출력
    printf("address of i == %p\n", &i);                 //i의 주소값 출력
    printf("value of ptr == %p\n", ptr);                //ptr의 값(i의 주소값) 출력
    printf("address of ptr == %p\n", &ptr);             //ptr의 주소값 출력
    printf("value of *ptr == %d\n", *ptr);              //ptr이 가르키는 값(i의 값) 출력
    printf("value of dptr == %p\n", dptr);              //dptr의 값 출력
    printf("address of dptr == %p\n", &dptr);           //dptr의 주소값 출력
    printf("value of *dptr == %p\n", *dptr);            //dptr의 주소값이 가르키는 값(ptr의 주소값) 출력
    printf("value of **dptr == %d\n", **dptr);          //dptr의 주소값이 가르키는 값이 가르키는 값(ptr의 주소값이 가르키는 값인 i의 값) 출력
    *ptr = 7777; /* changing the value of *ptr */
    printf("\n[after *ptr = 7777] \n");              
    printf("value of i == %d\n", i);                    
    printf("value of *ptr == %d\n", *ptr);              
    printf("value of **dptr == %d\n", **dptr);          //*ptr을 바꾸면 ptr이 가르키는 값, i의 값을 바꾼것과 같다
    **dptr = 8888; /* changing the value of **dptr */
    printf("\n[after **dptr = 8888] \n");               
    printf("value of i == %d\n", i);                    
    printf("value of *ptr == %d\n", *ptr);              
    printf("value of **dptr == %d\n", **dptr);          //**dptr을 바꾸면 dptr이 가르키는 값인 ptr이 가르키는 값, i의 값을 바꾼것과 같다
    return 0;
}