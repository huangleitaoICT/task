#include <stdio.h>
#include <fcntl.h>  
#include <termios.h>
#include <unistd.h> 
#include <errno.h>  

int main(int, char**) {
    
    //连接
    int fd;//串口标识符
    fd=open("/dev/ttyACM0",O_RDWR|O_NOCTTY);
    if(fd == -1)
     printf("\n  Error! in Opening ttyACM0\n");
  else
     printf("\n  ttyACM0 Opened Successfully\n");

    //设置串口参数
    termios SerialPortSettings;//初始化串口参数的实例，在头文件termios中已经有这个结构体定义
    tcgetattr(fd,&SerialPortSettings);//获取当前设置的串口参数

    //配置波特率，通常读写速度一致
    cfsetispeed(&SerialPortSettings,B115200);//设置读取的速度
    cfsetospeed(&SerialPortSettings,B115200);//设置写入的速度

    //配置其他串口参数
    SerialPortSettings.c_cflag&=~PARENB;//取消奇偶校验
    SerialPortSettings.c_cflag &= ~CSIZE;//清除停止位
    SerialPortSettings.c_cflag |= CS8;//设置数据位 8
    SerialPortSettings.c_cflag|=CREAD|CLOCAL;//打开接受串口，否则无法从串口读取数据
    SerialPortSettings.c_cc[VMIN]=10;//读取数据的最小字符数
    SerialPortSettings.c_cc[VTIME]=0;//等待读取数据的时间，0代表一直等待

    //载入串口参数
    tcsetattr(fd,TCSANOW,&SerialPortSettings);

    //写入数据
    char write_buffer[]="write";//写入的数据内容
    int bytes_written =0 ;//写入数据的大小
    bytes_written = write(fd,write_buffer,sizeof(write_buffer));//调用函数


    //读取数据
    char read_buffer[32];//读取的数据内容
    int  bytes_read = 0;//读取的数据大小                           
    bytes_read = read(fd,&read_buffer,32);//调用函数
    
    //关闭串口
    close(fd);
}
