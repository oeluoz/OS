# shell的简单实现
## 实现的功能
- 执行系统内置程序
- Tab自动补全，能够补全文件路径，但是补全命令还存在问题，初步判定是  
    `readline()`函数设置问题，因为补全策略有文件路径和命令两种
- shell内置命令：**exit** **history** **cd**
- 管道
- 输入输出重定向
   
    
  

## 使用
- `sudo apt-get install libreadline-dev` 安装readline库
- `make all` 编译
- `./shell` 运行

## 参考文件
- [readline库的简单使用](https://phoenixxc.github.io/posts/3faf94c3/)
- [shell输出改变字体颜色](https://www.cnblogs.com/lr-ting/archive/2013/02/28/2936792.html)
- [shell的自己实现](https://blog.csdn.net/YinJianxiang/article/details/76386394)