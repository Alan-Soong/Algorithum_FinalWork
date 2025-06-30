# 关于Linux运行下的注意事项

## 程序介绍

下面是对于程序代码的介绍：

> * generate_data.cpp：生成测试数据的文件
> * generate_data：对应的可执行文件
> * main.cpp：主程序文件，负责算法执行
> * main：对应的可执行文件
> * text_data.txt：数据文件，由generate_data.cpp生成
> * College-Oriented_result.txt：基于高校选择的结果，由main.cpp生成
> * Student-Oriented_result.txt：基于学生选择的结果，由main.cpp生成
> * run_all.sh：运行两个程序，需要指定参数
> * run_main.sh：只运行主程序，直接读取即可

## 程序运行

1. 运行所有文件，指定学生总数和学校总数
```shell
bash run_all.sh --students A --colleges B
# 或者
./run_all.sh --students A --colleges B
```
2. 运行主文件，无需指定参数
```shell
bash run_main.sh
# 或者
./run_main.sh
```