#include "shell.h"
#define EXIT_STATUS_S 1

/**
 * @brief  shell程序
 * @note   
 * @retval 
 */
int shell() {
    pid_t pid;
    char cmd[50];
    char ** args;


    do{
        /*获取当前路径*/
        char *file_path_getcwd;
        file_path_getcwd=(char *)malloc(80);
        getcwd(file_path_getcwd,80);
        
        /*设置终端输出文字颜色*/
        printf("\033[1;32mleo@LAPTOP-MVSJ24RV:");
        printf("\033[35m%s",file_path_getcwd);
        printf("\33[0m$");

        /*读取用户输入*/
        fflush(stdin);
        gets(cmd);
        
        /*删除多余空格*/
        trim(cmd);
        args = getParameter(cmd); /*通过一个二级指针返回，不能确定二级指针的长度，二级指针的最后一个元素是NULL*/

        if(strcmp(args[0],"exit")==0) {
            return EXIT_STATUS_S;
        }else if (strcmp(args[0],"cd")==0) {
            cd(args);
        }else {
            pid = fork();
            if(pid == 0) { /*命令执行子线程，必须要等待子线程执行完毕父线程才退出*/
                execvp(args[0],args);  //最后加一个NULL作为参数的终结符号，参数就是完整指令，不能去掉开始的命令
            }else if(pid > 0) {
                wait();
                // execv("./shell",NULL);
            }else {
                printf("创建线程失败！\n");
            }
        }
        
    }while(1);
}
/**
 * @brief  删除命令中多余的空格
 * @note   
 * @param  args: 输入命令字符串
 * @retval None
 */
void trim(char* args) {
    int i,length;
    length = strlen(args);
    for(i=0;args[i]!='\0';i++) {
        if((args[i]==args[i+1]&&args[i]==' ') || (i==0&&args[i]==' ') ||args[i]==' '&&args[i+1]=='\0') { //最后元素可能为空格
            for(int j=i;j<length;j++) {
                args[j]=args[j+1];
            }
            i--;
        }
    }
}

/**
 * @brief  将输入的以空格分隔的字符串存放到数组中
 * @note   
 * @param  args: 空格分隔的字符串
 * @retval 二级指针，存放每一个字符串的指针
 */
char** getParameter(char* args) {
    int i,j,k,m,count;
    count = 0;
    char *temp;
    char** tempargs = (char**) malloc(sizeof(char*)*10);
    for(i=j=0; ; j++) { 
        if(args[j] == ' ' || args[j] == '\0') {
            temp = (char *)malloc(sizeof(char)*10);
            for(k = 0; i<j ;) { /*获取一个参数*/
                temp[k++] = args[i++];
            }
            temp[k] = '\0';
            tempargs[count++] = temp;
            i = j + 1;
            if(args[j] == '\0') 
                break;
        }
    }
    char**parameter = (char**) malloc(sizeof(char*)*(count+1));
    for(int i=0; i<count; i++) {
        parameter[i] = tempargs[i];
    }
    
    parameter[count]=NULL;
    return parameter;
}

/**
 * @brief  shell cd命令
 * @note   
 * @param  args: "cd path"命令解析的二级指针
 * @retval 成功返回 0
 */
int cd(char** args) {
    if(args[1] == NULL) {
        fprintf(stderr, "sh: expected argument to \"cd\"\n");
    }else {
        if(chdir(args[1])!=0)
            perror("shell");
    }
    return 0;
}

/**
 * @brief  主函数
 * @note   
 * @retval 
 */
int main() {
    shell();
    return 0;
}
