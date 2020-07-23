#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 40  //航班最大值
#define FSCAN fscanf(fp, "%d\t\t%c\t\t%c\t\t%d:%d\t\t%d:%d\t\t%d:%d", &flyy[i].flight_number,&flyy[i].departure_airport, &flyy[i].arrival_airport, &flyy[i].departure_time_hour, &flyy[i].departure_time_min, &flyy[i].fly_time_hour, &flyy[i].fly_time_min, &flyy[i].stay_time_hour, &flyy[i].stay_time_min) //將文件裡的內容按照格式讀入
#define PRINT_TITLE printf("flight no\tdeparture\tarrival\t\tdeparture\tfly time\tstay time\n\t\tairport\t\tairport\t\ttime\n");  //print出航班標題
#define PRINT_INFO printf("%d\t\t%c\t\t%c\t\t%02d:%02d\t\t%02d:%02d\t\t%02d:%02d\n", flyy[i].flight_number, flyy[i].departure_airport, flyy[i].arrival_airport, flyy[i].departure_time_hour, flyy[i].departure_time_min, flyy[i].fly_time_hour, flyy[i].fly_time_min, flyy[i].stay_time_hour, flyy[i].stay_time_min)  //print出航班資料

struct fly{
    int flight_number;  //航班編號
    char departure_airport;  //起飛機場
    char arrival_airport;  //抵達機場
    int departure_time_hour;  //班次時間
    int departure_time_min;
    int fly_time_hour;  //飛行時間
    int fly_time_min;
    int stay_time_hour;  //停留時間
    int stay_time_min;

}flyy[SIZE];

void main_menu();  //主選單
void add_flight();  //新增航班
void reduce_flight();  //刪除航班
void search_flight();  //查詢航班
void all_flight();  //查看所有航班
void airport_ac();  //查看機場飛機數量
void check_airport(char *a_airport);  //機場判斷，是否為ABCDE
void check_time(int *h_hour, int *m_min);  //時間判斷，是否符合24時制
void switch_p(int *p_ac, char *p_d_airport, char *p_a_airport);  //關於機場容納數量的正switch
void switch_n(int *n_ac, char *n_d_airport, char *n_a_airport);  //關於機場容納數量的反switch

char airport_name[5] = {'A', 'B', 'C', 'D', 'E'};  //5個機場的名字
int max_ac[5] = {15, 10, 5, 5, 10};  //各個機場最大容納量
int ac[5] = {6, 5, 3, 2, 5};  //各個機場原有飛機數量

int main()
{
    FILE *fp;
    fp = fopen("flight.txt", "r");
    if(fp == NULL){
        printf("can't open the file\n");
        exit(0);
    }

    char a[10000];
    int i = 0;
    int select;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    PRINT_TITLE;

    while(fgets(a, 10000, fp) != NULL){

        FSCAN;

        switch_p(ac, &flyy[i].departure_airport, &flyy[i].arrival_airport);  //計算各個機場現有飛機數量

        if(flyy[i].flight_number != 0){  //print所有航班資訊
            PRINT_INFO;
            i++;
        }
    }

    printf("\n");
    for(int i=0 ; i<5 ; i++)
        printf("airport %c:  now: %d   Max: %d \n", airport_name[i], ac[i], max_ac[i]);  //print機場內現有飛機數量及最大容納量

    fclose(fp);

    while(1){

        main_menu();

        choice:
        SetConsoleTextAttribute(hConsole, 0xF);
        printf("\n                                    please enter your choice:                                    \n");
        SetConsoleTextAttribute(hConsole, 0x7);
        scanf("%d", &select);

        switch(select){
        case 1: add_flight();
                break;
        case 2: reduce_flight();
                break;
        case 3: search_flight();
                break;
        case 4: all_flight();
                break;
        case 5: airport_ac();
                break;
        case 6: SetConsoleTextAttribute(hConsole, 0xF);
                printf("Thank you for using the system, see you next time!");
                SetConsoleTextAttribute(hConsole, 0xE);
                printf("\n                                                                                          ︵\n");  //鴨鴨飛機排班系統的標誌
                printf("                                                                                     .__ ( ·)<  ( bye~~ )\n");
                printf("                                                                                      \\_____)\n");
                SetConsoleTextAttribute(hConsole, 0x3);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                SetConsoleTextAttribute(hConsole, 0x7);
                return 0;
        default: goto choice;  //如果輸入的數字不是1~6，重新輸入

                break;
        }
    }
}

void main_menu()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("\n\n");
    SetConsoleTextAttribute(hConsole, 0xE);
    printf("                                                     ︵                                                    \n");  //鴨鴨飛機排班系統的標誌
    printf("                                                .__ ( ·)>                                                  \n");
    printf("                                                 \\_____)                                                  \n");
    SetConsoleTextAttribute(hConsole, 0x3);
    printf("                                    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                    \n");
    SetConsoleTextAttribute(hConsole, 0xF);
    printf("                                    歡 迎 來 到 鴨 鴨 飛 機 排 班 系 統                                    \n");  //I love duck
    printf("                                    ===================================                                    \n");
    printf("                                    ||  1. 新 增 航 班               ||                                    \n");
    printf("                                    ||  2. 刪 除 航 班               ||                                    \n");
    printf("                                    ||  3. 查 詢 航 班               ||                                    \n");
    printf("                                    ||  4. 查 看 所 有 航 班         ||                                    \n");
    printf("                                    ||  5. 查 看 機 場 飛 機 數 量   ||                                    \n");
    printf("                                    ||  6. 離 開                     ||                                    \n");
    printf("                                    ===================================                                    \n");
    SetConsoleTextAttribute(hConsole, 0x7);
}

void add_flight()
{
    FILE *fp;
    fp = fopen("flight.txt", "a+");
    if(fp == NULL){
        printf("Open the file failure...\n");
        exit(0);
    }

    int add_number;
    char answer;
    int flag = 0, pr_flight_number;  //用於檢視是否輸入重複的flight number所用的變數
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("Do you want to add flight?\n");  //防手殘設定
    printf("Enter 'y' to continue, enter 'n' to go back: \n");
    scanf(" %c", &answer);
    if(answer == 'n')
        return;

    printf("How many flights do you want to increase?\n");
    scanf("%d", &add_number);

    fseek(fp, 0, SEEK_END);  //從file的最後開始新增航班

    for(int i=0 ; i<add_number ; i++){

        printf("\n");

        printf("flight no.: ");
        scanf("%d", &pr_flight_number);
        while(1){  //檢視是否輸入重複的flight number
            for(int k=0 ; k<SIZE ; k++){
                if(pr_flight_number == flyy[k].flight_number)
                    flag++;
            }
            if(flag == 0)
                break;
            else{
                SetConsoleTextAttribute(hConsole, 0xA);
                printf("The flight number had scheduled, please enter another flight number.\n");
                printf("Enter 0 to return main menu.\n");
                SetConsoleTextAttribute(hConsole, 0x7);
                scanf("%d", &pr_flight_number);
                if(pr_flight_number == 0){
                    fclose(fp);
                    return;
                }
                flag = 0;
                continue;
            }
        }
        flyy[i].flight_number = pr_flight_number;

        printf("departure airport: ");
        scanf(" %c", &flyy[i].departure_airport);
        check_airport(&flyy[i].departure_airport);

        printf("arrival airport: ");
        scanf(" %c", &flyy[i].arrival_airport);
        check_airport(&flyy[i].arrival_airport);

        printf("departure time(hour:min): ");
        scanf("%d:%d", &flyy[i].departure_time_hour, &flyy[i].departure_time_min);
        check_time(&flyy[i].departure_time_hour, &flyy[i].departure_time_min);

        printf("fly time(hour:min): ");
        scanf("%d:%d", &flyy[i].fly_time_hour, &flyy[i].fly_time_min);
        check_time(&flyy[i].fly_time_hour, &flyy[i].fly_time_min);

        printf("stay time(hour:min): ");
        scanf("%d:%d", &flyy[i].stay_time_hour, &flyy[i].stay_time_min);
        check_time(&flyy[i].stay_time_hour, &flyy[i].stay_time_min);

        switch_p(ac, &flyy[i].departure_airport, &flyy[i].arrival_airport);  //計算各機場現有飛機數量

        if(ac[0]>=0 && ac[1]>=0 && ac[2]>=0 && ac[3]>=0 && ac[4]>=0  && ac[0]<=15 && ac[1]<=10 && ac[2]<=5 && ac[3]<=5 && ac[4]<=10){  //判斷該機場是否有飛機、機場容納量是不是已達最大值
            fprintf(fp, "%d\t\t%c\t\t%c\t\t%02d:%02d\t\t%02d:%02d\t\t%02d:%02d\n",
                                                            flyy[i].flight_number,
                                                            flyy[i].departure_airport,
                                                            flyy[i].arrival_airport,
                                                            flyy[i].departure_time_hour,
                                                            flyy[i].departure_time_min,
                                                            flyy[i].fly_time_hour,
                                                            flyy[i].fly_time_min,
                                                            flyy[i].stay_time_hour,
                                                            flyy[i].stay_time_min);
        }

        else{  //如果新增的航班不成立，機場容納量就不會變動，也不會寫進file裡
            switch_n(ac, &flyy[i].departure_airport, &flyy[i].arrival_airport);  //將剛剛加減的飛機數量反算回去
            SetConsoleTextAttribute(hConsole, 0xC );
            printf("\n* Sorry, the airport is full/empty. *\n\n");
            SetConsoleTextAttribute(hConsole, 0x7 );
            fclose(fp);
            return add_flight();  //返回詢問是否要新增航班
        }
    }

    SetConsoleTextAttribute(hConsole, 0xB );
    printf("\n* Added successfully! *\n\n");
    SetConsoleTextAttribute(hConsole, 0x7 );

    fclose(fp);

    return ;
}

void reduce_flight()
{
    int del_flight, number;
    int flag = 0;  //用於刪除航班時檢查是否有此航班
    char a[10000], answer;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("Do you want to delete flight?\n");  //防手殘設定
    printf("Enter 'y' to continue, enter 'n' to go back: \n");
    scanf(" %c", &answer);
    if(answer == 'n')
        return;

    printf("How many flights do you want to delete?\n");
    scanf("%d", &number);

    for(int j=0 ; j<number ; j++){

        FILE *fp, *fr;
        fp = fopen("flight.txt", "a+");
        fr = fopen ("reduce.txt", "w+");
        if(fp==NULL || fr==NULL){
            printf("Open the file failure...\n");
            exit(0);
        }

        printf("Which flight you want to delete?\n");  //藉由航班編號刪除該航班
        printf("Please enter the flight number:\n");
        scanf("%d", &del_flight);
        while(1){ //檢查是否有此航班
            for(int k=0 ; k<SIZE ; k++){
                if(del_flight == flyy[k].flight_number)
                    flag++;
            }
            if(flag == 0){
                SetConsoleTextAttribute(hConsole, 0xA);
                printf("Couldn't find the flight number, please enter another flight number.\n");
                printf("Enter 0 to return main menu.\n");
                SetConsoleTextAttribute(hConsole, 0x7);
                scanf("%d", &del_flight);
                if(del_flight == 0)
                    break;
                flag = 0;
                continue;
            }
            else{
                SetConsoleTextAttribute(hConsole, 0xB);
                printf("\n* Deleted successfully! *\n\n");
                SetConsoleTextAttribute(hConsole, 0x7);
                break;
            }
        }

        flag = 0;  //如果一次要刪除一筆以上的話，需要將flag歸0

        fprintf(fr, "flight no\tdeparture\tarrival\t\tdeparture\tfly time\tstay time\n");  //因為要成立一個新的file取代舊的，所以要先打這些標題資訊
        fprintf(fr, "\t\tairport\t\tairport\t\ttime\n");

        for(int i=0 ; i<SIZE ; i++){

            FSCAN;

            if(del_flight==flyy[i].flight_number && flyy[i].flight_number!=0)
                switch_n(ac, &flyy[i].departure_airport, &flyy[i].arrival_airport);  //將刪掉的那筆航班所影響的機場容量數做反算

            if(del_flight!=flyy[i].flight_number && flyy[i].flight_number!=0){  //除了刪掉的那筆航班資料外其餘都存進新的txt檔裡
                fprintf(fr, "%d\t\t%c\t\t%c\t\t%02d:%02d\t\t%02d:%02d\t\t%02d:%02d\n",
                                                                flyy[i].flight_number,
                                                                flyy[i].departure_airport,
                                                                flyy[i].arrival_airport,
                                                                flyy[i].departure_time_hour,
                                                                flyy[i].departure_time_min,
                                                                flyy[i].fly_time_hour,
                                                                flyy[i].fly_time_min,
                                                                flyy[i].stay_time_hour,
                                                                flyy[i].stay_time_min);
            }
            else
                flyy[i].flight_number = 0;  //將刪掉的那筆資料設為0，才不會重複讀到
        }

        fclose(fp);
        fclose(fr);

        remove("flight.txt");
        rename("reduce.txt","flight.txt");
    }

    return ;
}

void search_flight()
{
    FILE *fp;
    fp = fopen("flight.txt", "r+");
    if(fp == NULL){
        printf("Open the file failure...\n");
        exit(0);
    }

    int choice;
    int flight_choice, departure_time_hour_choice, departure_time_min_choice;
    char airport_choice;
    int flag1 = 0, flag2 = 0;  //用於判斷是否藉由機場/出發時間找到對應的航班(因為不像航班編號的查詢必定只有一筆，所以設flag來做判斷)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    again:
    printf("Search by\n1: flight number\n2: airport\n3: departure time\n0: return main menu\n");  //藉由1.航班編號 2.機場 3.飛機出發時間 查詢
    scanf("%d", &choice);

    switch(choice){
        case 1: printf("Please enter the flight number: ");
                scanf("%d", &flight_choice);

                for(int i=0 ; i<SIZE ; i++){

                    FSCAN;

                    if(flight_choice==flyy[i].flight_number && flyy[i].flight_number!=0){
                        PRINT_TITLE;
                        PRINT_INFO;
                        fclose(fp);  //因為找到對應的航班後之後會直接return回main menu，不會做底下fclose的動作，將導致程式有bug，所以這裡要先關檔
                        return;
                    }
                }
                SetConsoleTextAttribute(hConsole, 0xB);
                printf("\nThere is no flight.\n");
                SetConsoleTextAttribute(hConsole, 0x7);
                break;

        case 2: printf("Please enter the airport: ");
                scanf(" %c", &airport_choice);
                check_airport(&airport_choice);  //先檢查是否輸入正確的機場名稱

                for(int i=0 ; i<SIZE ; i++){

                    FSCAN;

                    if(airport_choice==flyy[i].arrival_airport && flyy[i].arrival_airport!=0 && flyy[i].flight_number!=0){
                        PRINT_TITLE;
                        PRINT_INFO;
                        flag1++;
                    }
                }

                if(flag1 == 0){
                    SetConsoleTextAttribute(hConsole, 0xB);
                    printf("\nThere is no flight.\n");
                    SetConsoleTextAttribute(hConsole, 0x7);
                }
                break;

        case 3: printf("Please enter the departure time(hour:min): ");
                scanf("%d:%d", &departure_time_hour_choice, &departure_time_min_choice);
                check_time(&departure_time_hour_choice, &departure_time_min_choice);  //先檢查是否輸入正確的時間


                for(int i=0 ; i<SIZE ; i++){

                    FSCAN;

                    if(departure_time_hour_choice==flyy[i].departure_time_hour && departure_time_min_choice==flyy[i].departure_time_min && flyy[i].flight_number!=0){
                        PRINT_TITLE;
                        PRINT_INFO;
                        flag2++;
                    }
                }

                if(flag2 == 0){
                    SetConsoleTextAttribute(hConsole, 0xB);
                    printf("\nThere is no flight.\n");
                    SetConsoleTextAttribute(hConsole, 0x7);
                }
                break;

        case 0: fclose(fp);
                return;

        default: goto again;  //如果輸入的數字不是1~3，重新輸入
                 break;
    }

    fclose(fp);

    return ;
}

void airport_ac()
{
    for(int i=0 ; i<5 ; i++)
        printf("\nairport %c:  now: %d   Max: %d \n", airport_name[i], ac[i], max_ac[i]);

    return ;
}

void all_flight()
{
    FILE *fp;
    fp = fopen("flight.txt", "r");
    if(fp == NULL){
        printf("Open the file failure...\n");
        exit(0);
    }

    char ch;

    while((ch=getc(fp)) != EOF){
        printf("%c", ch);
    }

    fclose(fp);

    return ;
}

void check_airport(char *a_airport)
{
    char airport;
    airport = *a_airport;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(1){
        if(airport<'A' || airport>'E'){
            SetConsoleTextAttribute(hConsole, 0xC);
            printf("The airport doesn't exist.\n");
            printf("Please enter the airport again.\n");
            SetConsoleTextAttribute(hConsole, 0x7);
            scanf(" %c", &airport);
            continue;
        }
        else
            *a_airport = airport;
            break;
        }
}

void check_time(int *h_hour, int *m_min)
{
    int hour, min;
    hour = *h_hour;
    min = *m_min;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(1){
        if(hour>=24 || hour<0 || min>=60 || min<0){
            SetConsoleTextAttribute(hConsole, 0xC);
            printf("Time error.\n");
            printf("Please enter the correct time.\n");
            SetConsoleTextAttribute(hConsole, 0x7);
            scanf("%d:%d", &hour, &min);
            continue;
        }
        else{
            *h_hour = hour;
            *m_min = min;
            break;
        }
    }
}

void switch_p(int *p_ac, char *p_d_airport, char *p_a_airport)
{
    char d_airport, a_airport;
    d_airport = *p_d_airport;
    a_airport = *p_a_airport;

    switch(d_airport){
            case'A' : p_ac[0]--; break;
            case'B' : p_ac[1]--; break;
            case'C' : p_ac[2]--; break;
            case'D' : p_ac[3]--; break;
            case'E' : p_ac[4]--; break;
        }

    switch(a_airport){
            case'A' : p_ac[0]++; break;
            case'B' : p_ac[1]++; break;
            case'C' : p_ac[2]++; break;
            case'D' : p_ac[3]++; break;
            case'E' : p_ac[4]++; break;
        }
}

void switch_n(int *n_ac, char *n_d_airport, char *n_a_airport)
{
    char d_airport, a_airport;
    d_airport = *n_d_airport;
    a_airport = *n_a_airport;

    switch(d_airport){
            case'A' : n_ac[0]++; break;
            case'B' : n_ac[1]++; break;
            case'C' : n_ac[2]++; break;
            case'D' : n_ac[3]++; break;
            case'E' : n_ac[4]++; break;
        }

    switch(a_airport){
            case'A' : n_ac[0]--; break;
            case'B' : n_ac[1]--; break;
            case'C' : n_ac[2]--; break;
            case'D' : n_ac[3]--; break;
            case'E' : n_ac[4]--; break;
        }
}
