// OJ Problem Set 3 : High precision multiplication
// 修改优化后的最终版。尽量简化了循环，删去了不必要的判定代码
#include <stdio.h>
#include <string.h>
# define num 500 // 注意此处的字符串长度必须不小于500，否则在后面会出现溢出（checkpoint4）

int main(void)
{
    char a[num] = {0};
    int aflag = 0; // 用于指示a的正负，默认（0）为+
    int na, nb, dota = 0, dotb = 0, dot = 0; // dota dotb 表示小数点前有几位
    char b[num] = {0};
    int bflag = 0; // 与a同理
    scanf("%s", a);
    scanf("%s", b);

    
    // **输入处理模块**
    // 补丁0：检查负数
    if (a[0] == '-')
    {
        for (int i = 0; i<strlen(a); i++)  // 不必循环整个字符串，利用strlen(a)即可
        {
            a[i] = a[i+1]; // 将负号后面的数字往前移
        }
        aflag = 1;
    }

    if (b[0] == '-')
    {
        for (int i = 0; i<strlen(b); i++)
        {
            b[i] = b[i+1]; // 同上
        }
        bflag = 1;
    }

    // 处理小数点：找到小数点，记录小数点前的数字位数，同时去除小数点
    // 和上一个负号处理的目的相同，都是为了转为整数乘法
    for (int i = strlen(a); i >= 0; i--)
    {
        if (a[i] == '.')
        {
            dota = i;
            for (int i2 = i; i2<strlen(a); i2++)
        {
            a[i2] = a[i2+1];
        }
        break;
        } 
    } 

    // 记录处理后的整数总位数，在前面先处理也可以，但我乐得这么干:)
    na = strlen(a);

    // 同上
    for (int i = strlen(b); i >= 0; i--)
    {
        if (b[i] == '.')
        {
            dotb = i;
            for (int i2 = i; i2<strlen(b); i2++)
        {
            b[i2] = b[i2+1];
        }
        break;
        }
    }

    
    // 同上
    nb = strlen(b);

    // 补丁1：检查0，如果字符串a, b有任意一个经处理后，全部的有效元素都为0，那么直接输出0（checkpoint6）
    int fa = 0,fb = 0;
    for (int i = 0; i< na; i++)
    {
        if (a[i] != 0 && a[i] != '0')
        fa = 1;
    }
    for (int i = 0; i< nb; i++)
    {
        if (b[i] != 0 && b[i] != '0')
        fb = 1;
    }
    if (!fa || !fb)
    {   
        printf("0");
        return 0;
    }

    // **核心算法模块**
    // 首先，不管三七二十一，把所有结果都丢到对应的位数上去
    int c[num *2] = {0};
    for (int i = na -1; i >= 0; i--)
    {
        for (int j = nb-1; j >= 0; j--)
        {   
            // 利用int强制转换char的数字元素，结合ASCII表规律得到正确的整数值
            long long int x = ((int)a[i] - '0') * ((int)b[j] - '0'); 
            // 注意，此处我把数字【反着】存进了整数列表c，也就是位数大的数字反而存在后面，位数小的存在前面
            // 这样做，实际上顺便可以处理 输入的a,b前面带0的情况
            c[na + nb - i - j -2] += x; 
        }
    }
    // 接下来对每个位数进行进十
    for (int i = 0; i< num*2-1; i++)
    {
        if (c[i] >= 10)
        {
            c[i+1] += c[i]/10;
            c[i] = c[i] % 10;
        }
    }
    
    
    
    // 得到结果的位数
    int wsc=0;
    for (int i = num * 2 -1; i>=0; i--)
    {
        if (c[i] != 0)
        {
            wsc = i+1;
            break;
        }
    }

    // **打印结果模块**
    // 判断是否需要负号，是则打印
    if ((aflag && !bflag) || (bflag && !aflag))
    printf("-");

    // 判断是否有小数点
    if (dota || dotb)
    dot = 1;

    if (!dot) // 没有小数点，就直接打印
    {
        for (int i = wsc -1; i>=0; i--)
            printf("%d", c[i]);
    }
    else
    {   
        // 如果a,b中的某个数没有小数点，就把代表 小数点前有几位 的dota和dotb改成a和b的长度
        // 这么做的原因是：如果没有这一步，计算小数乘整数时会出错
        if (!dota)
        dota = na;
        if (!dotb)
        dotb = nb;
        int xiao = na + nb - dota - dotb; // 计算结果的小数位数
        // 补丁2：把结果最后的多余0都改成-1，方便后续判断
        for (int i = 0; i< xiao; i++)
        {   
        if (c[i] == 0)
        {
            c[i] = -1;
        }
        else
        break;
        }
        // 如果 结果的有效（也就是非0的部分）位数 小于结果的小数位数（比如0.04*0.04），那么先打印一个0
        // 如果不这么干，那么在打印整数部分的循环中不会打印出0，因为不符合循环条件。
       if (wsc <= xiao)
        {
            printf("0");
        }
        // 打印整数部分
        for (int i = wsc - 1; i > xiao -1; i--)
            printf("%d", c[i]);
        // 打印小数点
        for (int i = 0; i< xiao; i++)
        {   
            // 补丁3：如果小数部分是xx.0000，那么把小数点省掉
            if (c[i] != -1)
            {
                printf(".");
                break;
            }
        }
        // 输出小数部分
        for (int i = xiao-1; i>-1; i--)
        {   
            if(c[i] != -1)
            printf("%d", c[i]);
        }
    }
    return 0;
}
