# 2020广工赛

#### 介绍
2020广工赛

#### 我如何提交代码？
1、git add .
2、git commit -m "debug"
3、git push origin master

#### 请即使更新主代码，使用命令

git remote add upstream https://gitee.com/gahowong/GGS2020.git

#### 如何贡献代码？

git push origin BRANCH_NAME
#### 使用说明

实验目的:
	学习MPU9250 九轴传感器(三轴加速度+三轴陀螺仪+三轴磁力计)的使用.
	
硬件资源:
	1,DS0(连接在PB1)  
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面) 
	3,ALIENTEK 2.8/3.5/4.3/7寸LCD模块(包括MCU屏和RGB屏,都支持) 
	4,KEY0按键(连接在PH3)
	5,MPU9250传感器(连接在PH4/PH5上,INT脚未用到).
	
实验现象:
	本实验,程序先初始化MPU9250等外设，然后利用MPL库，初始化MPU9250及使能DMP，最后，在死循环里面
	不停读取：温度传感器、加速度传感器、陀螺仪、磁力计、MPL姿态解算后的欧拉角等数据，通过串口上报
	给上位机（温度不上报），利用上位机软件（ANO_TC匿名科创地面站v4.exe），可以实时显示MPU9250的传
	感器状态曲线，并显示3D姿态，可以通过KEY0按键开启/关闭数据上传功能。同时，在LCD模块上面显示温度
	和欧拉角等信息。DS0来指示程序正在运行。
#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

