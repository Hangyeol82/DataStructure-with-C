#include <stdio.h>
int main()
{
    char charType;
    int integerType;
    float floatType;
    double doubleType;
    printf("Size of char: %ld byte\n",sizeof(charType));        //charType의 자료형인 char의 크기를 출력
    printf("Size of int: %ld bytes\n",sizeof(integerType));     //intergerType의 자료형인 int의 크기를 출력
    printf("Size of float: %ld bytes\n",sizeof(floatType));     //floatType의 자료형인 float의 크기를 출력
    printf("Size of double: %ld bytes\n",sizeof(doubleType));   //doubleType의 자료형인 double의 크기를 출력
    printf("-----------------------------------------\n");
    printf("Size of char: %ld byte\n",sizeof(char));            //자료형 char의 크기를 출력
    printf("Size of int: %ld bytes\n",sizeof(int));             //자료형 int의 크기를 출력
    printf("Size of float: %ld bytes\n",sizeof(float));         //자료형 float의 크기를 출력
    printf("Size of double: %ld bytes\n",sizeof(double));       //자료형 double의 크기를 출력
    printf("-----------------------------------------\n");
    printf("Size of char*: %ld byte\n",sizeof(char*));          
    printf("Size of int*: %ld bytes\n",sizeof(int*));           
    printf("Size of float*: %ld bytes\n",sizeof(float*));       
    printf("Size of double*: %ld bytes\n",sizeof(double*));     //64비트 컴퓨터의 포인터 크기는 8 bytes로 동일
    return 0;
}