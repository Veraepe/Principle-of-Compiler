#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQUADSIZE 30          //四元组顺序表的size
#define MAXPROGSIZE 100        //prog代码表的size
#define MAXTOKENSIZE 30     //token顺序表的size
#define RWTABSIZE 2     //rwtsize
#define BEGIN 1         //begin
#define END 2           //end
#define VARNAME 10   //变量名
#define NUMBER 11  //变量值
#define ADD 13       //+
#define SUB 14       //-
#define MUL 15       //*
#define DIV 16       //chu
#define LBRACKET 27  //(
#define RBRACKET 28  //)
#define ASSIGN 18     //:=
#define SEMI 26      //;
#define POUND 0      //#
#define STRINGTYPE  12 //"string"

typedef enum
{
    integers,
    strings,
    none
}Valuetype;//赋值语句右半段 类型检查

// 定义四元组
typedef struct a{
    char result[MAXTOKENSIZE];
    char arg1[MAXTOKENSIZE];
    char op[MAXTOKENSIZE];
    char arg2[MAXTOKENSIZE];
}QuadGroup;

typedef struct asqlist
{
    QuadGroup* quad;
    int length;
    int t;//用来表示t几
}QuadGroupList;//四元组顺序表

typedef struct b
{
    char* str;
    int length;
    int scanP;//当前扫描到的位置的后一个位置，也就是下次将被扫描的位置上
}ProgramList;//程序代码顺序表

typedef struct c
{
    char* token;
    int length;
} TokenList;//词法token顺序表



// 定义全局变量
QuadGroupList quadGL;
ProgramList progL;
TokenList tokenL;
char *rwtab[2]= {"begin", "end"};;//记录两个一次一码
int syn;
Valuetype valuetype;//负责检测扫描到的是字符串还是数字
int progindex;//检测扫描到第几个句子了

// 类型检查 字符串不能和数字相加  
// 字符串输入格式为"content" 数字输入格式为123456
//不要将双引号当做符号
void generateQuad(char res[], char arg1[], char op[], char arg2[]);//生成四元式 放入四元式顺序表
void transformTAC();//遍历四元式顺序表，将四元式转换为三地址码，然后输出出来
void makeResultOfQuad(char res[]);//生成tn,将tn放到res里，随后便可放到四元式里面
void initialAll();//初始化函数，用于初始化刚开始的四元组顺序表，程序顺序表之类的东西
void inputInProgL();//输入函数，用于从控制台将代码输入到程序代码顺序表中
void scanProg();//负责去扫描语句，并判断所扫描到的语句是什么类型，修改并syn 供其他函数识别所扫描到的符号或变量类型
                //如 遇到数字，则将syn置为11,遇到分号，则将syn置于26
void tokenChar(char ch);//将字符放入token数组中，token顺序表表示当前解析的词

//向下递归解析
void makeFactor(char varValue[]);//解析因子。比如将句子中的字符串或者数字等不可再被拆分的部分，返回到上一个函数
                                // 返回到上个函数，提供为变量值，辅助生成四元式。
                                // 也承担了解析括号的作用
void anylseExpressionDeep(char varValue[]);//分析优先级高的乘除号，生成四元式
void anylseExpression(char varValue[]);//分析优先级较低的加减号，生成四元式
void anylseSentence();//分析每一条句子，分析变量名和赋值号，分析完后，进入下一个函数分析赋值号后面的那段表达式，之后生成四元式。遇到分号则分析下一条句子
void lrparser(); //作为递归的开始和结束，用来去分析是否有begin和end，随后调用anylseSentence