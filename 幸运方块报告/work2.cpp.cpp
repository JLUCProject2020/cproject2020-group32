#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
using namespace std;
const int n=17,m=17,tot=30,kittot=10,maxhp=10;
int turn;
int map[50][50],cx,cy,step,skilluse;
int dir[][2]={{-1,0},{1,0},{0,-1},{0,1}};
struct player
{
    int px,py;
    int hp,life;
    int kit;
    int armor,weapon,leftuse;
}p[5];
player *tail;

int port(int np,int tx,int ty);
void activate(int np,int tx,int ty);
inline int R()
{
    return (rand()*rand())^(rand()*rand());
}
inline void wait(int Time)
{
    Sleep(100*Time);
}

void pmenu()
{
    system("cls");
    puts("\n\n\n\n                □       □    □   □□□ □    □ □      □");
    puts("                □       □    □ □       □  □     □  □");
    puts("                □       □    □ □       □□         □");
    puts("                □       □    □ □       □□         □");
    puts("                □       □    □ □       □  □       □");
    puts("                □□□□ □□□□   □□□ □    □     □\n");
    puts("                □□□   □         □□     □□□ □    □");
    puts("                □    □ □       □    □ □       □  □");
    puts("                □□□   □       □    □ □       □□");
    puts("                □    □ □       □    □ □       □□");
    puts("                □    □ □       □    □ □       □  □");
    puts("                □□□   □□□□   □□     □□□ □    □\n");
    printf("\t\t\t\t");
}
void selectkit()
{
    int i;
    char input;
    for(i=0;i<=1;i++)
    {
        while(1)
        {
            pmenu();
            printf("请P%d选择职业\n",i+1);
            printf("\t\t[1):骑士\t\t[2):法师\n");
            printf("\t\t[3):弓箭手\t\t[4):牧师\n");
            input=getch();
            if(input>='1'&&input<='4')
            {
                (tail+i)->kit=input-'0';
                break;
            }
        }
    }
}
void makemap()
{
    int i,j;
    for(i=0;i<=15;i++)
    {
        pmenu();
        puts(">地图生成中<");
        printf("\t\t\t");
        for(j=1;j<=i;j++)
            printf("■");
        for(j=i+1;j<=15;j++)
            printf("□");
        wait(1);
    }
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            if((i^j)&1)
                map[i][j]=R()%(tot+kittot)+1;
    map[1][1]=map[n][m]=0;
}
void setplayer()
{
    int i;
    (tail+0)->px=(tail+0)->py=1;
    (tail+1)->px=n;
    (tail+1)->py=m;
    for(i=0;i<=1;i++)
    {
        (tail+i)->hp=maxhp;
        (tail+i)->life=3;
    }
}
void pmap()
{
    system("cls");
    int i,j;
    printf("\t\t╔");
    for(i=1;i<=m;i++) printf("═");
    printf("╗\n");
    for(i=1;i<=n;i++)
    {
        printf("\t\t║");
        for(j=1;j<=m;j++)
        {
            if(p[0].px==i&&p[0].py==j){    printf("P1");    continue;}
            if(p[1].px==i&&p[1].py==j){    printf("P2");    continue;}
            if(cx==i&&cy==j)
            {
                if(map[i][j]>0)    printf("■");
                else    printf("＋");
                continue;
            }
            if(map[i][j]>0)    printf("□");
            if(map[i][j]==0)    printf("·");
        }
        printf("║\n");
    }
    printf("\t\t╚");
    for(i=1;i<=m;i++) printf("═");
    printf("╝\n");
    for(i=0;i<=1;i++){
        if(turn==i)    printf(">"); else printf(" ");
        printf("P%d",i+1);
        switch(p[i].kit){
            case 1:printf("[骑士]"); break;
            case 2:printf("[法师]"); break;
            case 3:printf("[弓箭手]"); break;
            case 4:printf("[牧师]"); break;
        }
        printf("\t生命x%d 血量:%d 护甲:%d 武器:%d(%d)\n",p[i].life-1,p[i].hp,p[i].armor,p[i].weapon,p[i].leftuse);
    }
}
int resplayer(int np)
{
    pmap();
    printf("P%d死亡！\n",np+1);
    wait(20);
    (tail+np)->life--;
    (tail+np)->armor=(tail+np)->weapon=(tail+np)->leftuse=0;
    (tail+np)->hp=maxhp;
    if((tail+np)->life<=0)
    return 1;
    if(np==0)
        port(np,1,1);
    if(np==1)
        port(np,n,m);
    pmap();
    printf("P%d已复活！\n",np+1);
    wait(20);
    return 0;
}
int getweapon(int np,int watk,int wlft)
{
    char input;
    while((tail+np)->leftuse>0)
    {
        pmap();
        printf("P%d已装备武器，是否替换？(y/n)",np+1);
        input=getch();
        if(input=='Y'||input=='y')
        {
            (tail+np)->weapon=watk;
            (tail+np)->leftuse=wlft;
            return 1;
        }
        if(input=='N'||input=='n')
        return 0;
    }
    (tail+np)->weapon=watk;
    (tail+np)->leftuse=wlft;
    return 1;
}
int damage(int np,int val)
{
    pmap();
    printf("P%d受到了%d点伤害！\n",np+1,val);
    wait(20);
    if((tail+np)->armor>0)
    {
        (tail+np)->armor-=val;
        if((tail+np)->armor<0)
        {
            (tail+np)->hp+=(tail+np)->armor;
            (tail+np)->armor=0;
        }
    }
    else
        (tail+np)->hp-=val;
    if((tail+np)->hp<=0)
        if(resplayer(np))
            return 1;
    return 0;
}
void block1(int np)
{
    pmap();
    puts("╔╦╗----【闪电】----");
    puts("╠？╣~对自己造成 3 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np,3);
}
void block2(int np)
{
    pmap();
    puts("╔╦╗----【草莓蛋糕】----");
    puts("╠？╣~为自己恢复 3 点血量。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=min((tail+np)->hp+3,maxhp);
}
void block3(int np)
{
    pmap();
    puts("╔╦╗----【木质弓箭】----");
    puts("╠？╣~对敌人造成 2 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,2); 
}
void block4(int np)
{
    pmap();
    puts("╔╦╗----【逗你玩宝箱】----");
    puts("╠？╣~什么都没有发生。");
    puts("╚╩╝");
    wait(40);
}
void block5(int np)
{
    pmap();
    puts("╔╦╗----【初阶匕首】----");
    puts("╠？╣~装备一把攻击力: 3 ,耐久: 2 的武器。");
    puts("╚╩╝");
    wait(40);
    getweapon(np,3,2);
}
void block6(int np)
{
    pmap();
    puts("╔╦╗----【初阶皮甲】----");
    puts("╠？╣~获得 3 点护甲值。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=3;
}
void block7(int np)
{
    pmap();
    puts("╔╦╗\t----【整装待发】----");
    puts("╠？╣~装备一把攻击力: 2 ,耐久: 2 的武器；获得 2 点护甲值。");
    puts("╚╩╝");
    wait(40);
    getweapon(np,2,2);
    (tail+np)->armor+=2;
}
void block8(int np)
{
    pmap();
    puts("╔╦╗\t----【装备升级】----");
    puts("╠？╣~使你的武器获得 +1 攻击力和 +1 耐久度；获得 1 点护甲值。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse>0)
    {
        (tail+np)->weapon++;
        (tail+np)->leftuse++;
    }
    (tail+np)->armor++;
}
void block9(int np)
{
    pmap();
    puts("╔╦╗----【冰冻陷阱】----");
    puts("╠？╣~立刻结束本回合。");
    puts("╚╩╝");
    wait(40);
    if(np==turn)
    step=0;
}
void block10(int np)
{
    pmap();
    puts("╔╦╗----【天降甘霖】----");
    puts("╠？╣~为双方各恢复 4 点血量。");
    puts("╚╩╝");
    wait(40);
    (tail+0)->hp=min((tail+0)->hp+4,maxhp);
    (tail+1)->hp=min((tail+1)->hp+4,maxhp);
}
void block11(int np)
{
    pmap();
    puts("╔╦╗----【幻影传送】----");
    puts("╠？╣~使你瞬间移动到一个随机的位置。");
    puts("╚╩╝");
    wait(40);
    while(1)
    {
        int tx=R()%n+1,ty=R()%m+1;
        if(tx!=(tail+((np^1)))->px||ty!=(tail+((np^1)))->py)
        {
            port(np,tx,ty);
            break;
        }
    }
}
void block12(int np)
{
    pmap();
    puts("╔╦╗----【快速移动】----");
    puts("╠？╣~再掷一次骰子。");
    puts("╚╩╝");
    wait(40);
    if(np!=turn)
    return ;
    int ts=R()%5+2;
    pmap();
    printf("掷到了%d点！",ts);
    wait(20);
    step+=ts;
}
void block13(int np)
{
    pmap();
    puts("╔╦╗----【精制短剑】----");
    puts("╠？╣~装备一把攻击力: 4 ,耐久: 3 的武器。");
    puts("╚╩╝");
    wait(40);
    getweapon(np,4,3);
}
void block14(int np)
{
    pmap();
    puts("╔╦╗----【精制铁甲】----");
    puts("╠？╣~获得 5 点护甲值。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=5;
}
void block15(int np)
{
    pmap();
    puts("╔╦╗----【幸运虹光】----");
    puts("╠？╣~双方各触发一次随机幸运方块效果。");
    puts("╚╩╝");
    wait(40);
    map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
    activate(np,(tail+np)->px,(tail+np)->py);
    map[p[(np^1)].px][p[(np^1)].py]=R()%(tot+kittot)+1;
    activate((np^1),p[(np^1)].px,p[(np^1)].py);
}
void block16(int np)
{
    pmap();
    puts("╔╦╗----【圣洁之露】----");
    puts("╠？╣~为自己恢复所有血量。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=maxhp;
}
void block17(int np)
{
    pmap();
    puts("╔╦╗\t----【恐怖恶魔】----");
    puts("╠？╣~对自己造成 4 点伤害；使你的武器失去 1 点耐久度。");
    puts("╚╩╝");
    wait(40);
    damage(np,4);
    if((tail+np)->leftuse>0)
    (tail+np)->leftuse--;
    if((tail+np)->leftuse==0)
    (tail+np)->weapon=0;
}
void block18(int np)
{
    pmap();
    puts("╔╦╗----【穿刺炸弹】----");
    puts("╠？╣~对 11x11 内的所有角色造成 3 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np,3);
    if(abs((tail+0)->px-(tail+1)->px)<=5&&abs((tail+0)->py-(tail+1)->py)<=5)
    damage((np^1),3);
}
void block19(int np)
{
    pmap();
    puts("╔╦╗----【王者对决】----");
    puts("╠？╣~立刻将对手移动到你的身边。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->px>1)
    port((np^1),(tail+np)->px-1,(tail+np)->py);
    else
    port((np^1),(tail+np)->px+1,(tail+np)->py);
}
void block20(int np)
{
    pmap();
    puts("╔╦╗----【正义之锤】----");
    puts("╠？╣~装备一把攻击力: 2 ,耐久: 5 的正义之锤。");
    puts("╚╩╝");
    wait(40);
    getweapon(np,2,5);
}
void block21(int np)
{
    pmap();
    puts("╔╦╗----【灵魂之火】----");
    puts("╠？╣~使自己的血量变为: 5 。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=5;
}
void block22(int np)
{
    pmap();
    puts("╔╦╗\t\t----【改装】----");
    puts("╠？╣~消耗掉所有护甲值；使你的武器获得 +2 攻击力。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    {
        (tail+np)->weapon+=2;
        (tail+np)->armor=0;
    }
}
void block23(int np)
{
    pmap();
    puts("╔╦╗\t\t----【精准掷击】----");
    puts("╠？╣~对对手造成等同于你武器攻击力的伤害，摧毁你的武器。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    {
        damage((np^1),(tail+np)->weapon);
        (tail+np)->weapon=(tail+np)->leftuse=0;
    }
}
void block24(int np)
{
    pmap();
    puts("╔╦╗----【镜像反转】----");
    puts("╠？╣~交换你与对手的武器，护甲，血量值。");
    puts("╚╩╝");
    wait(40);
    swap((tail+np)->hp,(tail+(np^1))->hp);
    swap((tail+np)->armor,(tail+(np^1))->armor);
    swap((tail+np)->weapon,(tail+(np^1))->weapon);
    swap((tail+np)->leftuse,(tail+(np^1))->leftuse);
}
void block25(int np)
{
    pmap();
    puts("╔╦╗----【沉重之甲】----");
    puts("╠？╣~获得 6 点护甲值，结束你的回合。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=6;
    if(turn==np)
    step=0;
}
void block26(int np)
{
    pmap();
    puts("╔╦╗\t\t----【献祭】----");
    puts("╠？╣~消耗 4 点血量，装备一把攻击力: 5 ,耐久: 2 的血之复仇。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp-=4;
    if((tail+np)->hp<=0)
    {
        resplayer(np);
        return ;
    }
    getweapon(np,5,2);
}
void block27(int np)
{
    pmap();
    puts("╔╦╗----【救赎】----");
    puts("╠？╣~获得 1 条生命，对自己造成 12 点伤害。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->life++;
    damage(np,12);
}
void block28(int np)
{
    pmap();
    puts("╔╦╗----【幸运风暴】----");
    puts("╠？╣~触发 3 次随机幸运方块效果。");
    puts("╚╩╝");
    wait(40);
    for(int i=1;i<=3;i++)
    {
        map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
        activate(np,(tail+np)->px,(tail+np)->py);
    }
}
void block29(int np)
{
    pmap();
    puts("╔╦╗----【龙卷风】----");
    puts("╠？╣~清除所有角色的武器和护甲值。");
    puts("╚╩╝");
    wait(40);
    (tail+0)->armor=(tail+0)->weapon=(tail+0)->leftuse=0;
    (tail+1)->armor=(tail+1)->weapon=(tail+1)->leftuse=0;
}
void block30(int np)
{
    pmap();
    puts("╔╦╗\t----【金色祝福】----");
    puts("╠？╣~双方各装备一把攻击力: 2 ,耐久: 3 的武器，各获得 3 点护甲值。");
    puts("╚╩╝");
    wait(40);
    getweapon(0,2,3);
    getweapon(1,2,3);
    (tail+0)->armor+=3;
    (tail+1)->armor+=3;
}
void block101(int np)
{
    pmap();
    puts("╔╦╗----【正义之斧】----");
    puts("╠？╣~装备一把攻击力: 3 ,耐久: 3 的武器。");
    puts("╚╩╝");
    wait(40);
    getweapon(np,3,3);
}
void block102(int np)
{
    pmap();
    puts("╔╦╗----【圣光战马】----");
    puts("╠？╣~获得 3 点护甲值,剩余移动次数 +3 。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=3;
    if(np==turn)
    step+=3;
}
void block103(int np)
{
    pmap();
    puts("╔╦╗\t----【识破】----");
    puts("╠？╣~使对方武器攻击力和护甲值降为 2 。");
    puts("╚╩╝");
    wait(40);
    if((tail+(np^1))->leftuse)
    (tail+(np^1))->weapon=2;
    if((tail+(np^1))->armor>2)
    (tail+(np^1))->armor=2;
}
void block104(int np)
{
    pmap();
    puts("╔╦╗\t----【神圣祝福】----");
    puts("╠？╣~使你的武器攻击力 +1 ,耐久 +2 ,获得 2 点护甲值，");
    puts("╚╩╝如果你没有武器，则装备一把攻击力: 2 ,耐久: 2 的武器。");
    wait(40);
    if((tail+np)->leftuse)
    {
        (tail+np)->weapon++;
        (tail+np)->leftuse+=2;
    }
    else    getweapon(np,2,2);
    (tail+np)->armor+=2;
}
void block105(int np)
{
    pmap();
    puts("╔╦╗----【光耀之甲】----");
    puts("╠？╣~清除你的武器，使你的护甲值变为 7 。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    (tail+np)->weapon=(tail+np)->leftuse=0;
    (tail+np)->armor=7;
}
void block106(int np)
{
    pmap();
    puts("╔╦╗\t----【公平较量】----");
    puts("╠？╣~使你的武器与对手相同，对手的护甲值与你相同。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->weapon=(tail+(np^1))->weapon;
    (tail+np)->leftuse=(tail+(np^1))->leftuse;
    (tail+(np^1))->armor=(tail+np)->armor;
}
void block107(int np)
{
    pmap();
    puts("╔╦╗----【勇敢奉献】----");
    puts("╠？╣~消耗 1 点血量，对对方造成 3 点伤害。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp-=1;
    if((tail+np)->hp<=0)
    resplayer(np);
    damage((np^1),3);
}
void block108(int np)
{
    pmap();
    puts("╔╦╗\t----【光明圣印】----");
    puts("╠？╣~获得 2 点护甲值，触发一次随机幸运方块效果。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=2;
    map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
    activate(np,(tail+np)->px,(tail+np)->py); 
}
void block109(int np)
{
    pmap();
    puts("╔╦╗\t----【血蹄战马】----");
    puts("╠？╣~清除你的武器和护甲值，装备一把攻击力: 4 ,耐久: 2 的光之愤怒，");
    puts("╚╩╝剩余移动次数 +6");
    wait(40);
    (tail+np)->weapon=(tail+np)->leftuse=(tail+np)->armor=0;
    getweapon(np,4,2);
    if(turn==np)
    step+=6;
}
void block110(int np)
{
    pmap();
    puts("╔╦╗\t----【极耀之光】----");
    puts("╠？╣~对 17x17 内的敌人造成等同于你武器攻击力的伤害，");
    puts("╚╩╝使你的武器耐久 -1 。");
    wait(40);
    if((tail+np)->leftuse)
    {
        if(abs((tail+0)->px-(tail+1)->px)<=8&&abs((tail+0)->py-(tail+1)->py)<=8)
        damage((np^1),(tail+np)->weapon);
        (tail+np)->leftuse--;
        if((tail+np)->leftuse<=0)
        (tail+np)->weapon=0;
    }
}
void block201(int np)
{
    pmap();
    puts("╔╦╗----【魔法飞弹】----");
    puts("╠？╣~对 17x17 内的敌人造成 3 点伤害。");
    puts("╚╩╝");
    wait(40);
    if(abs((tail+0)->px-(tail+1)->px)<=8&&abs((tail+0)->py-(tail+1)->py)<=8)
    damage((np^1),3);
}
void block202(int np)
{
    pmap();
    puts("╔╦╗----【雷电之牢】----");
    puts("╠？╣~在对手周围生成 4 个[闪电]。");
    puts("╚╩╝");
    wait(40);
    int i,tx,ty;
    for(i=0;i<4;i++)
    {
        tx=(tail+(np^1))->px+dir[i][0];
        ty=(tail+(np^1))->py+dir[i][1];
        if(tx>=1&&tx<=n&&ty>=1&&ty<=m&&(tx!=(tail+np)->px||ty!=(tail+np)->py))
            map[tx][ty]=1;
    }
}
void block203(int np)
{
    pmap();
    puts("╔╦╗----【凝霜之息】----");
    puts("╠？╣~在对手周围生成 4 个[冰冻陷阱]。");
    puts("╚╩╝");
    wait(40);
    int i,tx,ty;
    for(i=0;i<4;i++)
    {
        tx=(tail+(np^1))->px+dir[i][0];
        ty=(tail+(np^1))->py+dir[i][1];
        if(tx>=1&&tx<=n&&ty>=1&&ty<=m&&(tx!=(tail+np)->px||ty!=(tail+np)->py))
            map[tx][ty]=9;
    }
}
void block204(int np)
{
    pmap();
    puts("╔╦╗----【暗能炸弹】----");
    puts("╠？╣~对对方 9x9 内的所有玩家造成 3 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage((np^1),3);
    if(abs((tail+0)->px-(tail+1)->px)<=4&&abs((tail+0)->py-(tail+1)->py)<=4)
    damage(np,3);
}
void block205(int np)
{
    pmap();
    puts("╔╦╗----【魔能之火】----");
    puts("╠？╣~对对方造成 2~5 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,R()%4+2);
}
void block206(int np)
{
    pmap();
    puts("╔╦╗----【远古祝福】----");
    puts("╠？╣~使你的武器攻击力 +1 ,耐久 +4 ；获得 4 点护甲值。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    {
        (tail+np)->weapon++;
        (tail+np)->leftuse+=4;
    }
    (tail+np)->armor+=4;
}
void block207(int np)
{
    pmap();
    puts("╔╦╗----【烈焰风暴】----");
    puts("╠？╣~对所有角色造成 4 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,4);
    damage(np,4);
}
void block208(int np)
{
    pmap();
    puts("╔╦╗\t----【风暴法杖】----");
    puts("╠？╣~装备一把攻击力: 2 ,耐久: 3 的武器，对对方造成 2 点伤害。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,2);
    getweapon(np,2,3);
}
void block209(int np)
{
    pmap();
    puts("╔╦╗\t----【远古魔法阵】----");
    puts("╠？╣~在你的周围生成 8 个幸运方块，剩余移动次数 +4。");
    puts("╚╩╝");
    wait(40);
    int i,j,tx,ty;
    for(i=-1;i<=1;i++)
    {
        for(j=-1;j<=1;j++)
        {
            tx=(tail+np)->px+i;
            ty=(tail+np)->py+j;
            if((i==0&&j==0)||tx<1||tx>n||ty<1||ty>m)
            continue;
            if((tail+(np^1))->px!=tx||(tail+(np^1))->py!=ty)
            map[tx][ty]=R()%(tot+kittot)+1;
        }
    }
    if(turn==np)
    step+=4;
}
void block210(int np)
{
    pmap();
    puts("╔╦╗\t----【星界魔法】----");
    puts("╠？╣~获得 2 点护甲值，在你的周围生成 4 个[魔法飞弹]。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=2;
    int i,tx,ty;
    for(i=0;i<4;i++)
    {
        tx=(tail+np)->px+dir[i][0];
        ty=(tail+np)->py+dir[i][1];
        if(tx>=1&&tx<=n&&ty>=1&&ty<=m&&(tx!=(tail+(np^1))->px||ty!=(tail+(np^1))->py))
            map[tx][ty]=tot+1;
    }
}
void block301(int np)
{
    pmap();
    puts("╔╦╗----【强化弓箭】----");
    puts("╠？╣~对对方造成 3 点伤害，剩余移动次数 -1。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,3);
    if(turn==np)    step=max(step-1,0);
}
void block302(int np)
{
    pmap();
    puts("╔╦╗----【快速射击】----");
    puts("╠？╣~对对方造成等同你武器耐久的伤害。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    damage((np^1),(tail+np)->leftuse);
}
void block303(int np)
{
    pmap();
    puts("╔╦╗----【远程狙击】----");
    puts("╠？╣~对 17x17 以外的敌人造成 4 点伤害。");
    puts("╚╩╝");
    wait(40);
    if(abs((tail+0)->px-(tail+1)->px)>8||abs((tail+0)->py-(tail+1)->py)>8)
    damage((np^1),4);
}
void block304(int np)
{
    pmap();
    puts("╔╦╗----【武器打磨】----");
    puts("╠？╣~使你的武器攻击力 +2 ,耐久 -1 。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    {
        (tail+np)->weapon+=2;
        (tail+np)->leftuse--;
        if(!(tail+np)->leftuse)
        (tail+np)->weapon=0;
    }
}
void block305(int np)
{
    pmap();
    puts("╔╦╗----【穿甲弓箭】----");
    puts("╠？╣~使对方的血量 -2 。");
    puts("╚╩╝");
    wait(40);
    (tail+(np^1))->hp-=2;
    if((tail+(np^1))->hp<=0)
    resplayer((np^1));
}
void block306(int np)
{
    pmap();
    puts("╔╦╗\t----【隐蔽】----");
    puts("╠？╣~获得 4 点护甲值，剩余移动次数 -2 。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->armor+=4;
    if(turn==np)
    step-=2;
}
void block307(int np)
{
    pmap();
    puts("╔╦╗\t----【回旋镖】----");
    puts("╠？╣~对对方造成 1 点伤害，装备一把攻击力: 4 ,耐久: 1 的武器。");
    puts("╚╩╝");
    wait(40);
    damage(np^1,1);
    getweapon(np,4,1);
}
void block308(int np)
{
    pmap();
    puts("╔╦╗\t----【追踪术】----");
    puts("╠？╣~剩余移动次数 +1 ，触发一次随机幸运方块效果。");
    puts("╚╩╝");
    wait(40);
    if(turn==np)    step++;
    map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
    activate(np,(tail+np)->px,(tail+np)->py);
}
void block309(int np)
{
    pmap();
    puts("╔╦╗\t----【风行术】----");
    puts("╠？╣~消耗所有护甲值，每消耗 1 点护甲值，。");
    puts("╚╩╝使你的剩余移动次数 +1 。");
    wait(40);
    if(turn==np)
    {
        step+=(tail+np)->armor;
        (tail+np)->armor=0;
    }
}
void block310(int np)
{
    pmap();
    puts("╔╦╗\t----【坚强意志】----");
    puts("╠？╣~获得 2 点护甲值，如果这不是你的回合，");
    puts("╚╩╝则改为获得 5 点护甲值。");
    wait(40);
    if(turn!=np)    (tail+np)->armor+=5;
    else    (tail+np)->armor+=2;
}
void block401(int np)
{
    pmap();
    puts("╔╦╗----【圣洁之泉】----");
    puts("╠？╣~为自己恢复 4 点血量。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=min((tail+np)->hp+4,maxhp);
}
void block402(int np)
{
    pmap();
    puts("╔╦╗----【治疗之环】----");
    puts("╠？╣~为 13x13 内的所有角色恢复 5 点血量。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=min((tail+np)->hp+5,maxhp);
    if(abs((tail+0)->px-(tail+1)->px)<=6&&abs((tail+0)->py-(tail+1)->py)<=6)
    (tail+(np^1))->hp=min((tail+(np^1))->hp+5,maxhp);
}
void block403(int np)
{
    pmap();
    puts("╔╦╗----【生命汲取】----");
    puts("╠？╣~使对方血量 -2 ,为你恢复 2 点血量。");
    puts("╚╩╝");
    wait(40);
    (tail+(np^1))->hp-=2;
    if((tail+(np^1))->hp<=0)    resplayer((np^1));
    (tail+np)->hp=min((tail+np)->hp+2,maxhp);
}
void block404(int np)
{
    pmap();
    puts("╔╦╗----【控心术】----");
    puts("╠？╣~使对方的血量变为 5 。");
    puts("╚╩╝");
    wait(40);
    (tail+(np^1))->hp=5;
}
void block405(int np)
{
    pmap();
    puts("╔╦╗----【血之契约】----");
    puts("╠？╣~如果你持有武器，使你的武器攻击力 +2 ,耐久 +3 ，并消耗 1 点生命值。");
    puts("╚╩╝");
    wait(40);
    if((tail+np)->leftuse)
    {
        (tail+np)->weapon+=2;
        (tail+np)->leftuse+=3;
        (tail+np)->hp-=1;
        if((tail+np)->hp<=0)
        resplayer(np);
    }
}
void block406(int np)
{
    pmap();
    puts("╔╦╗----【反噬】----");
    puts("╠？╣~使双方的血量变为 11-当前血量。");
    puts("╚╩╝");
    wait(40);
    (tail+0)->hp=11-(tail+0)->hp;
    (tail+1)->hp=11-(tail+1)->hp;
}
void block407(int np)
{
    pmap();
    puts("╔╦╗\t----【圣光惩击】----");
    puts("╠？╣~对对方造成等同于其武器攻击力的伤害，");
    puts("╚╩╝使对方的武器耐久度 -1 。");
    wait(40);
    if((tail+(np^1))->leftuse)
    {
        damage((np^1),(tail+(np^1))->weapon);
        (tail+(np^1))->leftuse--;
        if((tail+(np^1))->leftuse<=0)
        (tail+(np^1))->weapon=0;
    }
}
void block408(int np)
{
    pmap();
    puts("╔╦╗\t----【正义之手】----");
    puts("╠？╣~清除对方的所有护甲，并为自己恢复等量的血量。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=min((tail+np)->hp+(tail+(np^1))->armor,maxhp);
    (tail+(np^1))->armor=0;
}
void block409(int np)
{
    pmap();
    puts("╔╦╗\t----【光辉圣印】----");
    puts("╠？╣~恢复 2 点血量，触发一次随机幸运方块效果。");
    puts("╚╩╝");
    wait(40);
    (tail+np)->hp=min((tail+np)->hp+2,maxhp);
     map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
    activate(np,(tail+np)->px,(tail+np)->py);
}
void block410(int np)
{
    pmap();
    puts("╔╦╗\t----【光灵之力】----");
    puts("╠？╣~为对方恢复 2 点血量，自己装备一把攻击力: 4 ,耐久: 4 的光灵剑。");
    puts("╚╩╝");
    wait(40);
    (tail+(np^1))->hp=min((tail+(np^1))->hp+2,maxhp);
    getweapon(np,4,4);
}
void activate(int np,int tx,int ty)
{

    if(map[tx][ty]>0)
    {
        pmap();
        printf("P%d触发了幸运方块！\n",np+1);
        wait(20);
        int tmp=map[tx][ty];
        map[tx][ty]=0;
        if(tmp<=tot)
        switch(tmp)
        {
            case 1:block1(np); break;
            case 2:block2(np); break;
            case 3:block3(np); break;
            case 4:block4(np); break;
            case 5:block5(np); break;
            case 6:block6(np); break;
            case 7:block7(np); break;
            case 8:block8(np); break;
            case 9:block9(np); break;
            case 10:block10(np); break;
            case 11:block11(np); break;
            case 12:block12(np); break;
            case 13:block13(np); break;
            case 14:block14(np); break;
            case 15:block15(np); break;
            case 16:block16(np); break;
            case 17:block17(np); break;
            case 18:block18(np); break;
            case 19:block19(np); break;
            case 20:block20(np); break;
            case 21:block21(np); break;
            case 22:block22(np); break;
            case 23:block23(np); break;
            case 24:block24(np); break;
            case 25:block25(np); break;
            case 26:block26(np); break;
            case 27:block27(np); break;
            case 28:block28(np); break;
            case 29:block29(np); break;
            case 30:block30(np); break;
        }
        if(tmp>tot)
        switch(tmp-tot+100*(tail+np)->kit)
        {
            case 101:block101(np); break;
            case 102:block102(np); break;
            case 103:block103(np); break;
            case 104:block104(np); break;
            case 105:block105(np); break;
            case 106:block106(np); break;
            case 107:block107(np); break;
            case 108:block108(np); break;
            case 109:block109(np); break;
            case 110:block110(np); break;
            case 201:block201(np); break;
            case 202:block202(np); break;
            case 203:block203(np); break;
            case 204:block204(np); break;
            case 205:block205(np); break;
            case 206:block206(np); break;
            case 207:block207(np); break;
            case 208:block208(np); break;
            case 209:block209(np); break;
            case 210:block210(np); break;
            case 301:block301(np); break;
            case 302:block302(np); break;
            case 303:block303(np); break;
            case 304:block304(np); break;
            case 305:block305(np); break;
            case 306:block306(np); break;
            case 307:block307(np); break;
            case 308:block308(np); break;
            case 309:block309(np); break;
            case 310:block310(np); break;
            case 401:block401(np); break;
            case 402:block402(np); break;
            case 403:block403(np); break;
            case 404:block404(np); break;
            case 405:block405(np); break;
            case 406:block406(np); break;
            case 407:block407(np); break;
            case 408:block408(np); break;
            case 409:block409(np); break;
            case 410:block410(np); break;
        }
    }
}
int port(int np,int tx,int ty)
{
    if(tx<1||tx>n||ty<1||ty>m||(tx==(tail+(np^1))->px&&ty==(tail+(np^1))->py))
    return 0;
    (tail+np)->px=tx,(tail+np)->py=ty;
    if(map[tx][ty]>0)
    {
        activate(np,tx,ty);
    }
    return 1;
}
int attack()
{
    if(abs((tail+0)->px-(tail+1)->px)+abs((tail+0)->py-(tail+1)->py)>2)
    {
        printf("\n不在攻击范围！\n");
        wait(20);
        return 0;
    }
    if((tail+turn)->leftuse>0)
    {
        if(damage((turn^1),(tail+turn)->weapon))
        return 1;
        (tail+turn)->leftuse--;
        if(!(tail+turn)->leftuse)
        (tail+turn)->weapon=0;
    }
    else
    if(damage((turn^1),1))
        return 1;
    step--;
    int tx=(tail+(turn^1))->px,ty=(tail+(turn^1))->py;
    if((tail+(turn^1))->px<(tail+turn)->px)
        tx=(tail+(turn^1))->px-1;
    if((tail+(turn^1))->px>(tail+turn)->px)
        tx=(tail+(turn^1))->px+1;
    if((tail+(turn^1))->py<(tail+turn)->py)
        ty=(tail+(turn^1))->py-1;
    if((tail+(turn^1))->py>(tail+turn)->py)
        ty=(tail+(turn^1))->py+1;
    port((turn^1),tx,ty);
    return 0;
}
void useskill(int np)
{
    pmap();
    printf("P%d使用了技能!\n",np+1);
    switch((tail+np)->kit)
    {
        case 1:
        {
            puts("【英勇前进】装备一把2/2的武器,剩余移动次数 +1 。");
            wait(20);
            getweapon(np,2,2);
            if(np==turn)
            step++;
            break;
        }
        case 2:
        {
            puts("【远古智慧】触发一次随机幸运方块效果。");
            wait(20);
            map[(tail+np)->px][(tail+np)->py]=R()%(tot+kittot)+1;
            activate(np,(tail+np)->px,(tail+np)->py);
            break;
        }
        case 3:
        {
            puts("【精准射击】对 11x11 内的敌人造成 2 点伤害。");
            wait(20);
            if(abs(p[0].px-p[1].px)<=5&&abs(p[0].py-p[1].py)<=5)
            damage(np^1,2);
            break;
        }
        case 4:
        {
            puts("【治愈之光】为自己恢复 2 点血量。");
            wait(20);    
            (tail+np)->hp=min((tail+np)->hp+2,maxhp);
            break;
        }
    }
}
int goturn()
{
    pmap();
    printf(">>P%d正在掷骰子中",turn+1);
    int i;
    step=R()%5+2;
    skilluse=1;
    cx=cy=0;
    char input,linput=0;
    for(i=1;i<=3;i++)
    {
        wait(5);
        printf("。");
    }
    pmap();
    printf("掷到了%d点！",step);
    wait(20);
    while(step>0)
    {
        pmap();
        printf("[方向键移动,a键攻击,s键使用技能,o键结束,g键认输)剩余次数(%d):",step);
        input=_getch();
        if(input=='a')
        {
            step-=attack();
            if((tail+0)->life<=0||(tail+1)->life<=0)
            return 0;
            continue;
        }
        if(input=='o')
            break;
        if(input=='g')
        {
            (tail+turn)->life=1;
            (tail+turn)->hp=0;
            resplayer(turn);
            return 0;
        }
        if(input=='s')
        {
            if(step<2)
            {
                puts("\n剩余移动次数不足(最少 2 点)！");
                wait(20);
                continue;
            }
            if(skilluse<=0)
            {
                puts("\n你本回合只能使用一次技能！");
                wait(20);
                continue;
            }
            useskill(turn);
            skilluse--;
            step-=2;
            if((tail+0)->life<=0||(tail+1)->life<=0)
            return 0;
        }
        if(input==-32) input=_getch();
        if(input==linput)
        {
            step-=port(turn,cx,cy);
            cx=cy=0;
            if((tail+0)->life<=0||(tail+1)->life<=0)
            return 0;
        }
        linput=input;
        if(input==75) cx=(tail+turn)->px,cy=(tail+turn)->py-1;
        if(input==72) cx=(tail+turn)->px-1,cy=(tail+turn)->py;
        if(input==77) cx=(tail+turn)->px,cy=(tail+turn)->py+1;
        if(input==80) cx=(tail+turn)->px+1,cy=(tail+turn)->py;
    }
    return 1;
}
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    srand(time(NULL));
    tail=p;
    pmenu();
    puts(">按任意键进入游戏<");
    getch();
    selectkit();
    makemap();
    setplayer();
    while(goturn()) turn^=1;
    if((tail+0)->life<=0&&(tail+1)->life<=0) puts("平局！");
    else if((tail+0)->life<=0) puts("P2获胜！");
    else puts("P1获胜！");
    return 0;
}