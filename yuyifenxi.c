#include "tool.h"

//TODO generateQuad
void generateQuad(char res[], char args1[], char op[], char args2[])
{
    if(quadGL.length>=MAXQUADSIZE)
    {
    printf("your sentence is too much to overflow!\n");
    printf("the question is happened in the %d th sentence", progindex);
    exit(0);
    }
     strcpy(quadGL.quad[quadGL.length].result,res);
     strcpy(quadGL.quad[quadGL.length].arg1,args1);
     strcpy(quadGL.quad[quadGL.length].op,op);
     strcpy(quadGL.quad[quadGL.length].arg2,args2);
    quadGL.length++;
    // 生成四元组 往四元组顺序表内放入
}
//TODO transformTAC
void transformTAC()//转化为三地址码
{
    printf("The midCode generate successly!\n");
    printf("\n\n\n-------------------\n");
    for (int i = 0;i<quadGL.length;i++)
    {
        printf("%s  =   %s  %s  %s\n", quadGL.quad[i].result, quadGL.quad[i].arg1, quadGL.quad[i].op, quadGL.quad[i].arg2);        
    printf("-------------------\n"); 
    }
}
//TODO makeResultOfQuad
void makeResultOfQuad(char res[])
{
    char temp[MAXTOKENSIZE];
    //将quad里面的长度，也就是新的t几中的几转化为char*
    itoa(quadGL.t,temp,10);
    quadGL.t++;
    // 巧妙地生成t几
    res[0] = 't';//第一位放入t
    strcpy(res + 1, temp);//给res第二位放入数字
    res[2] = '\0';
    return res;
}
//TODO initialAll
void initialAll()
{
    // 初始化四元组
    quadGL.length = 0;
    quadGL.t = 0;
    quadGL.quad = (QuadGroup *)malloc(MAXQUADSIZE * sizeof(QuadGroup));
    // 程序字符串初始化
    progL.length = 0;
    progL.scanP = 0;
    progL.str = (char *)malloc(MAXPROGSIZE * sizeof(char));
    // token初始化
    tokenL.length = 0;
    tokenL.token = (char *)malloc(MAXTOKENSIZE * sizeof(char));
    // 其余变量
    progindex = 0;
    valuetype = none;//枚举
}
//TODO inputInProgL
void inputInProgL()
{
    char ch;
    printf("\nplease input your code（begin is open And # is end）\n");
    while(1)
    {
        scanf("%c", &ch);
        if(ch=='#')//遇到#便跳出循环
        {
            break;
        }
        //输入到progL顺序表中
        progL.str[progL.length] = ch;
        progL.length++;
    }
    progL.str[progL.length] = '\0';//将最后一位打上终止符
    progL.length++;
}
//TODO tokenChar
void tokenChar(char ch)//将字符放入token顺序表中，并将长度++
{
    if(tokenL.length>=MAXTOKENSIZE)//词的长度超出token顺序表的容量报错
    {
        printf("your word's length is too long\n");
        printf("the question is happened in the %d th sentence", progindex);
        exit(0);//退出程序
    }
    tokenL.token[tokenL.length] = ch;
    tokenL.length++;
}
//TODO scanProg
void scanProg()
{
    // 初始化token
    for (int i=0;i<MAXTOKENSIZE;i++)
    {
        tokenL.token[i] = NULL;
    }
    tokenL.length = 0;
    char ch;//字符
    int flag=0;//用于在字符串的时候检测

    //跳过无意义的空格
    // 先扫描再++
    ch = progL.str[progL.scanP];
    progL.scanP++;
    
    while(ch==' ')
    {
        ch = progL.str[progL.scanP];
        progL.scanP++;
    }
    // 上面都在寻找空格，跳出循环后，ch指向的是空格下一位

    // 判断变量名，数字，字符串并token
    // 变量名
    if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))
    {
        while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9')))
        {
            //token
            tokenChar(ch);//将字符token进函数并检查是否满了
            //继续扫描程序代码里的下一个字符
            ch = progL.str[progL.scanP];
            progL.scanP++;
        }
        // token结束后
        // 此时扫描到了最后一个字符的后一个字符，那么scanp所指的是最后一个字符的后两个
        // 所以要--
        progL.scanP--;
        syn = VARNAME;
        tokenChar('\0');//将终止符号token
        // 还要检测两个一词一码
        for (int k = 0;k<RWTABSIZE;k++)
        {
            if(strcmp(tokenL.token,rwtab[k])==0)
            {
                syn = 1 + k;
                break;
            }
        }
    }//检测数字
    else if ((ch >= '0')&&(ch<='9'))
    {
        while((ch>='0')&&(ch<='9'))
        {
            tokenChar(ch);//将字符token进函数并检查是否满了
            //继续扫描程序代码里的下一个字符
            ch = progL.str[progL.scanP];
            progL.scanP++;
        }
         progL.scanP--;
         syn = NUMBER;
         tokenChar('\0');//将终止符号token
    }
    else if(ch=='\"')
    {//检测是否遇到字符串
        tokenChar(ch);
        ch = progL.str[progL.scanP];
        progL.scanP++;//收集字符串的“
        while(ch!='\"')
        {
            tokenChar(ch);
            ch = progL.str[progL.scanP];
            progL.scanP++;
            if(ch=='\0')
            {
                flag = 1;
                break;
            }//再次遇到"或者遇到整个字符串走完了的情况，跳出循环
        }
        if(flag==1)
        {
            printf("you should give a sign \" after a strng\n");//转义符号对单引号进行转义
            printf("the question is happened in the %d th sentence", progindex);
            exit(0);
        }
        tokenChar('\"');//将”放入token
        syn = STRINGTYPE;//修改类型为string
        tokenChar('\0');    
    }
    else
    {
        //其他符号
        switch (ch)
        {
        case '('/* constant-expression */:
            /* code */
            syn = LBRACKET;
            break;
        case')':
            syn = RBRACKET;
            break;
        case ':'://如果检测到了冒号，便继续检测后面的符号是什么
            ch=progL.str[progL.scanP];
            progL.scanP++;
            if(ch=='=')
            {
                syn = ASSIGN;
            }
            else
            {
            printf("if you wanna assign a value, you should give a = after: to achieve!");
            printf("the question is happened in the %d th sentence", progindex);
            exit(0);
            }
            break;
            case '+':
                syn = ADD;
                break;
            case '-':
                syn = SUB;
                break;
            case '*':
                syn = MUL;
                break;
            case '/':
                syn = DIV;
                break;
            case ';':
                syn = SEMI;
                break;
            case '#':
                syn = POUND;
                break;
        }
    }
}
//TODO makeFactor
void makeFactor(char varValue[])
{
    //解析因子
    //因子可能是字符串，数字,变量或者左括号
    if(syn==LBRACKET)
    {
        scanProg();
        // 分析括号中的因子
        //将再次解析的结果复制给varValue
        anylseExpression(varValue);
        // 如果扫描后发现左括号，则任务完成，返回
        if(syn==RBRACKET)
        {
            scanProg();//扫描后返回
        }
        else
        {//如果只有左括号而没有右括号的话
            printf("you should give a ) when you have a (\n");
            printf("the question is happened in the %d th sentence", progindex);
            exit(1);
        }
    }else if(syn==VARNAME)//检测为变量名
    {
        // 将token到的变量名复制给varvalue，随后递归回去处理
        //判断当前扫描的为变量
        strcpy(varValue, tokenL.token);//strcpy两边数组最后大小一致，或左大右小
        scanProg();
    }else if(syn==STRINGTYPE)//检测为string
    {
        // 将字符串赋值给varvalue
        if(valuetype==strings||valuetype==none)//这里为了保证类型一致，字符串不能和数字相加 
        {
            valuetype = strings;//修改现在扫描的类型是字符串
            strcpy(varValue, tokenL.token);
            scanProg();
        }
        else
        {
            printf("String can not do operate with integer\n");
            printf("the question is happened in the %d th sentence", progindex);
            exit(0);
        }
        
    }else if(syn==NUMBER)
    {
        if(valuetype==integers||valuetype==none)//同上
        {
            valuetype = integers;//判断现在扫描的是数字
            strcpy(varValue, tokenL.token);
            scanProg();
        }
        else{
            printf("Integer can not do operate with string\n");
            printf("the question is happened in the %d th sentence", progindex);
            exit(0);
        }
    }
    else//如果既不是变量也不是数字也不是括号,那就是非法出错的情况
    {
         printf("your input sentence have a error\n");
         printf("the question is happened in the %d th sentence", progindex);
         exit(1);
    }
}


//TODO anylseExpressionDeep
void anylseExpressionDeep(char varValue[])
{
    char varValue2[MAXTOKENSIZE];
    char res[MAXTOKENSIZE];
    char op1[MAXTOKENSIZE];
    //调用makeFactor 得到的varValue是不能再被分隔的因子
    makeFactor(varValue);

    // 递归结束后，会将变量值返回回来，并且此时遇到的是变量值后面的东西
    //检查*/ 优先级大
    while(syn==MUL||syn==DIV)
    {
        // 递归解析
        if(syn==MUL)
        {
            op1[0] = '*';
            op1[1] = '\0';
        }
        else
        {
            op1[0] = '/';
            op1[1] = '\0';
        }
        //寻找参数2  因为这里是要放入四元组，所以要有两个参数
        scanProg();
        makeFactor(varValue2);
        // 注意此时，在makeFactor中，还有一次scan
        // 此时syn是变量值后面的符号
        makeResultOfQuad(res);//生成t几
        // 生成四元组
        generateQuad(res,varValue,op1,varValue2);
        // 将res作为新的varValue
        strcpy(varValue, res);
    }
    // 跳出循环后，将式子返回给加减法去解析
}
//TODO antlseExpression
void anylseExpression(char varValue[])
{
    //因为是赋值表达式 所以有op（+-*/），两个变量值和一个结果
    char op[MAXTOKENSIZE];   
     char varValue2[MAXTOKENSIZE];
    char res[MAXTOKENSIZE];
    
    anylseExpressionDeep(varValue);
    // 调用结束后，此时varValue便是变量值，并且此时遇到的是变量值后面的东西
    //检查加减 优先级最小
    while(syn==ADD || syn==SUB)// 
    {
        if(syn==ADD)
        {
            op[0] = '+';
            op[1] = '\0';//将加号放到op里
        }
        else
        {
            op[0] = '-';
            op[1] = '\0';
        }//记录符号
        //继续往后寻找,后面的参数
        scanProg();
        // 继续向下递归，寻找变量值二
        anylseExpressionDeep(varValue2);
        // 找到便是第二个参数 两个参数结合放入四元组
        makeResultOfQuad(res);//生成result=>tn
        generateQuad(res, varValue, op, varValue2);//生成四元组
        // 再将生成的tn代替原来的参数1,继续进行检验解析
        strcpy(varValue, res);
    }
    // 循环结束，说明找不到+-号，则返回
}
//TODO anylseWorkDeep
void anylseWorkDeep()// 更深层次解析 
{
    //该函数负责处理赋值号以及赋值号之前的部分，并将赋值号之后的部分，向下递归，调用其他函数进行处理
    
    char varName[MAXTOKENSIZE];//记录变量名，也就是标识符 
    char varValue[MAXTOKENSIZE];//记录变量值
    // 这里最好采用数组而不是采用malloc去开辟空间，如果用malloc需要当不使用这个函数时
    // 将空间手动free掉，否则会造成内存泄漏
 
    if(syn==VARNAME)// 先判断当前所扫描到的词是否符合标识符
    {
        // 如果符合便记录变量名
        strcpy(varName, tokenL.token);
        //寻找下一个部分
        scanProg();
        // 判断下一部分是不是assign（赋值号）
        if(syn==ASSIGN)
        {
            // 递归向下检查右半部分
            scanProg();//继续向后扫描，也就是扫描赋值号右边部分的语句
            anylseExpression(varValue);//进入分析右半部分表达式的函数，递归向下
            //这个函数执行完毕后，此时varValue的内容便等于赋值号后面的部分了（数字，变量名，tn）
            
            //将参与赋值operate的结果和arg，生成四元式
            generateQuad(varName,varValue,"","");
            // 即会生成如下样式 a=5 或  b="233" c=t2
        }
        else
        {   //发现后面没有赋值号，则报错，输出出错位置并退出
            printf("you should make assign operator like :=!\n");
            printf("the question is happened in the %d th sentence", progindex);
            exit(1);
        }
        }

}
//TODO anylseSentence
void anylseSentence()  // 分析除了begin以外的每个语句  
{
    scanProg();
    anylseWorkDeep();//更深层次解析第一个句子
    progindex++;//解析完毕后，扫描语句数++，分析有无分号
    while (syn == SEMI) //;的时候循环
    {
        valuetype = none;//检查完一个句子后寻找下一个句子，将type置为空，等待下一次类型检查使用
        scanProg();
        anylseWorkDeep();
        progindex++;//记录一条语句
    }
    if(syn!=END)
    {
        // 如果发现跳出循环后syn不是end
        printf("you should let a ; in the end of a sentence!");
        printf("the question is happened in the %d th sentence", progindex);
        exit(0);//退出程序
    }
}
//TODO lrparser
void lrparser()//最左规约
{
    scanProg();//扫描程序数组的一个词
    // if(遇到begin)
    if(syn==BEGIN)
    {
        progindex++;//表示现在扫描了一个语句了
        //对begin后面的语句进行解析
        anylseSentence();
        // 当从这个函数出来的时候，遇到的便是end了
        if (syn == END)
        {
            return; //成功
        }    
        else// 判断有无end 没有便提示
        {
            printf("you should give a end finally!\n");
            exit(0);
        }
    } 
    else
    {
        printf("you should give a begin firstly\n");
        exit(0);
    }
}
//TODO main
void main()
{ 
    // 准备工作,初始化相关变量
    initialAll();
    // 输入程序字符串，输入到prog程序代码顺序表中
    inputInProgL();    
    // 解析prog程序代码顺序表中的内容，递归向下，检查语义（包括部分语法检查和语义检查中的类型检查），并转换生成四元组
    lrparser();
    // 用三地址码表示四元组，并输出出来
    transformTAC();
}

