#include "shell.h"
#define EXIT_STATUS_S 1


extern void add_history PARAMS((const char *));

/* Add the contents of FILENAME to the history list, a line at a time.
   If FILENAME is NULL, then read from ~/.history.  Returns 0 if
   successful, or errno if not. */
extern int read_history PARAMS((const char *));

/* Write the current history to FILENAME.  If FILENAME is NULL,
   then write the history list to ~/.history.  Values returned
   are as in read_history ().  */
extern int write_history PARAMS((const char *));

/* Return a NULL terminated array of HIST_ENTRY which is the current input history.  
Element 0 of this list is the beginning of time.  If there is no history, return NULL.*/
extern HIST_ENTRY **history_list PARAMS((void));

/**
 * @brief  shell程序
 * @note   
 * @retval 
 */
int shell() {
    pid_t pid;

    char*cmd; //读取用户输入
    char*prompt;
    char hostname[40]={" @ LAPTOP-MVSJ24RV "};
    char** args;
    char* username;
    int printfReturn=0;

    /*输出配色方案*/
    char colorUser[50] = {"\001\033[1;32m\002"}; //绿色高亮显示用户名字 hostname
    char colorDirectory[50] = {"\001\033[1;36m\002"};//紫色显示当前文件目录
    char colorReset[50] = {"\001\033[0m\002"}; //显示恢复默认值
    char colorShow[50] = {"\001\033[5;32m\002"};

    /*从~/.history读出，之前每次放在循环里面，
    因此每次都会让history的list长度加倍*/
    read_history(NULL);
    do{
        /*获取当前路径*/
        char *file_path_getcwd;
        file_path_getcwd = (char *)malloc(80);
        getcwd(file_path_getcwd,80);
        
        /*获取当前用户*/
        username = getUserName();
        
        /*提示信息拼接*/
        prompt = (char*)malloc(sizeof(char)*128);
        memset(prompt,0,sizeof(char)*128);
        strcat(prompt,"\001\033[1;36m# \033[0m\002");
        strcat(prompt,colorUser);
        strcat(prompt,username);
        strcat(prompt,hostname);
        strcat(prompt,"\001\033[0m\002in ");
        strcat(prompt,colorDirectory);
        strcat(prompt,file_path_getcwd);
        strcat(prompt,"\001\033[1;33m\002 $ ");
        strcat(prompt,colorReset);
        
        fflush(stdin);
        cmd = readline(prompt);
        fflush(stdin);

        /*去除命令中多余的空格*/
        trim(cmd);

        /*将字符串中命令存放到二级指针，空格分隔，每一个存放到一个数组元素中*/
        args = getParameter(cmd); /*通过一个二级指针返回，不能确定二级指针的长度，二级指针的最后一个元素是NULL*/

        /*执行指定的命令*/
        int operationCode = whichOperation(args);
        if(operationCode !=INSTRUCTION_WRONG)
            add_history(cmd); //有效命令才能加入到history文件里面
        switch (operationCode) {
            case INSTRUCTION_WRONG:
                printf("shell: command not found :%s\n",args[0]);
                break;
            case INSTRUCTION_CD: 
                break;
            case INSTRUCTION_EXIT:
                write_history(NULL);
                return EXIT_STATUS_S;
            case INSTRUCTION_HISTORY:
                history();
                break;
            case INSTRUCTION_SYS_EXE:
                pid = fork(); 
                if(pid == 0) { /*命令执行子线程，必须要等待子线程执行完毕父线程才退出*/
                    execvp(args[0],args);  //最后加一个NULL作为参数的终结符号，参数就是完整指令，不能去掉开始的命令
                }else if(pid > 0) {
                    wait(pid);
                }else {
                    printf("创建线程失败！\n");
                }
                break;
            default:
                break;
        }
        free(prompt); 
    }while(1);
}

/**
 * @brief  得到对应命令的INSTRUCTION_*数字，用来switch对应的操作
 * @note   
 * @retval 
 */
int whichOperation(char**args) {
    if(strcmp(args[0],"cd")==0)
        return INSTRUCTION_CD;
    else if(strcmp(args[0],"exit")==0)
        return INSTRUCTION_EXIT;
    else if(strcmp(args[0],"history")==0)
        return INSTRUCTION_HISTORY;
    //这里判断指定的命令是否在/bin/目录下，需要考虑一个问题：./命令执行程序
    else if(systemExe(args)==SYSTEM_EXE)
        return INSTRUCTION_SYS_EXE;
    else if(args[0][0]=='.' && args[0][0]=='/')
        return INSTRUCTION_SYS_EXE;
    else return INSTRUCTION_WRONG;
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
 * @brief  shell内置命令，输出history信息
 * @note   
 * @retval 正常退出返回0
 */
int history() {
    HIST_ENTRY ** history;
    history=history_list();
    for(int i=0;history[i]!=NULL;i++) {
        printf("%s\n",history[i]->line);
    }
    return 0;
}

/**
 * @brief  获取用户名字
 * @note   
 * @retval 
 */
char * getUserName() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid (uid);
    if(pw) {
        return pw->pw_name;
    }
    return NULL;
}

/**
 * @brief  判断输入的指令是否为系统可执行的命令
 * @note   
 * @param  args: 输入信息转换为的二级指针
 * @retval 1:命令为系统可执行命令 0:系统不能执行的命令
 */
int systemExe(char** args) {
    DIR *d;
    struct dirent *dir;
    d = opendir("/bin/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(strcmp(args[0],dir->d_name)==0){ //待执行命令为系统命令
                closedir(d);
                return SYSTEM_EXE;
            }
        }
        closedir(d);
    }
    return NOT_SYSTEM_EXE;
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
