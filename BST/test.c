#include <stdio.h>
#define FNAME   "data.txt"
#define MAX_MEMBER    4
#define MAX_NAME_LEN 20

typedef struct _Member Member;
struct _Member
{
    char name[MAX_NAME_LEN+1];
    int num;
};

Member src[MAX_MEMBER]=
{
    {"홍길동",1},{"강감찬",4},{"을지문덕",2},{"김구",3}
};

Member dest[MAX_MEMBER];
void Save();//src의 회원 정보 파일에 쓰기
void Load();//파일에 회원 정보를 dest로 읽기
void List();//dest 내용 출력
int main()
{
    Save();
    Load();
    List();
    return 0;
}
void Save()
{
    FILE *fp = fopen(FNAME, "w"); // 파일 포인터를 반환받음

    if (fp)
    {
        fwrite(src, sizeof(Member), MAX_MEMBER, fp);
        fclose(fp);
    }
    else
    {
        printf("데이터 저장 실패\n");
    }
}

void Load()
{
    FILE *fp = fopen(FNAME, "r"); // 파일 포인터를 반환받음

    if (fp)
    {
        fread(dest, sizeof(Member), MAX_MEMBER, fp);
        fclose(fp);
    }
    else
    {
        printf("처음으로 회원 관리 프로그램을 사용을 환영합니다.\n");
    }
}

void List()
{
    int i = 0;
    for(i=0;i<MAX_MEMBER; i++)
    {
        printf("이름:%s 번호:%d\n",dest[i].name,dest[i].num);
    }
}