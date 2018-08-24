#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>


//test
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
 
//#define IPSTR "42.121.252.58" //服务器IP地址;
#define IPSTR "220.181.112.215" //服务器IP地址;
#define PORT 80
#define BUFSIZE 1024
 
int sound_main(int argc, char **argv)
{
        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        unsigned char str1[4096], str2[4096], buf[BUFSIZE * 50], *str;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;
         
         //创建套接字
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("创建网络连接失败,本线程即将终止---socket error!\n");
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("连接到服务器失败,connect error!\n");
                exit(0);
        }
        printf("与远端建立了连接\n");
        memset(str2, 0, 4096);
        strcat(str2, "theDataToPost");
        str=(char *)malloc(128);
        len = strlen(str2);
        sprintf(str, "%d", len);
 
        memset(str1, 0, 4096);
        strcat(str1, "GET http://tsn.baidu.com/text2audio?lan=zh&ctp=1&cuid=abcdxxx&tok=24.9477fe25d97ec4a544a796e4042e6321.2592000.1530338225.282335-11324740&tex=百度你好&vol=9&per=0&spd=5&pit=5 HTTP/1.1\n");

        //strcat(str1, "Host: www.cnblogs.com\n");
        strcat(str1, "Host: tsn.baidu.com\n");
        strcat(str1, "Content-Type: text/html\n");

        strcat(str1, "Content-Length: ");
        strcat(str1, str);
        strcat(str1, "\n\n");
 
        strcat(str1, str2);
        strcat(str1, "\r\n\r\n");
        printf("%s\n",str1);
 
        ret = write(sockfd,str1,strlen(str1));
        if (ret < 0) {
                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                exit(0);
        }else{
                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
        }
 
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);


		int fd; 
		int wret;
		fd = open("my.mp3", O_WRONLY | O_CREAT);
		if(fd < 0)
			perror("open eror");



 
		int k =0;
        while(1){
                sleep(2);
                tv.tv_sec= 0;
                tv.tv_usec= 0;
                h= 0;
                printf("--------------->1");
                h= select(sockfd +1, &t_set1, NULL, NULL, &tv);
                printf("--------------->2");
 
                //if (h == 0) continue;
                if (h < 0) {
                        close(sockfd);
                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
                        return -1;
                };
 
                if (h > 0){
                        memset(buf, 0, BUFSIZE*50);
                        i= read(sockfd, buf, BUFSIZE*50);
                        if (i==0){
                                close(sockfd);
                                printf("读取数据报文时发现远端关闭，该线程终止！\n");
                                return -1;
                        }
						printf("%s",buf);
				}

				wret = write(fd, buf, BUFSIZE*50);
				if(wret < 0)
					perror("write error");
#if 0
				for(k = 0; k <= i; k++)
					if(k % 32 == 0)
						printf("\n[%04x]", k);
					else
						printf("%02x ", buf[k]);
				printf("\n");
#endif

		}
		close(sockfd);
		close(fd);
 
 
        return 0;
}
