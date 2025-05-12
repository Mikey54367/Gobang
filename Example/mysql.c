#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

int main()
{
    //1.初始化mysql句柄
    MYSQL* mysql=mysql_init(NULL);
    if(mysql==NULL)
    {
        printf("init failed\n");
        return -1;
    }
    //2.连接服务器
    if(mysql_real_connect(mysql,"127.0.0.1","dzgs","","gobang",0,NULL,0)==NULL)
    {
        printf("connect failed: %s",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }
    //3.设置客户端字符集
    if(mysql_set_character_set(mysql,"utf8")!=0)
    {
        printf("set character failed: %s",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }
    //4.选择要操作的数据库
    //5.执行sql语句
    char *sql = "insert stu values(null,'Mike',18,53,69,88)";
    int ret=mysql_query(mysql,sql);
    if(ret!=0)
    {
        printf("%s\n",sql);
        printf("mysql query failed: %s",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }
    //6.如果是查询语句，则需保存结果到本地
    //7.获取结果集中的结果条数
    //8.遍历保存到本地的结果集
    //9.释放结果集
    //10.关闭连接，释放句柄
    mysql_close(mysql);

    return 0;
}