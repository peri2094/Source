//컴파일 : g++ 30sec_gm.cpp -o 30sec_gm `pkg-config --cflags --libs opencv` -lwiringPi -lpthread
//opcncv 드라이버 인식 : dmesg | tail

/*
##########################################
17-12-08 	23:41 void* My(void* Para) 쓸모없는 함수로 추정, 삭제
	 	23:52 char *timefunc(struct tm *t); 쓸모없는 함수로 추정, 주석처리 + time과 mail 관련 변수 삭제
	 	00:01 int iv_num 변수 전역변수에서 지역변수 main 안으로 복귀
		03:22 void fucn() 함수 쓸모없는 함수로 추정, 주석처리
17-12-11	18:20 보험용 영상 촬영
17-12-12	17:36 핫스팟으로 구동, APP 파일 2개 만들것 (192.168.43.203, 192.168.43.214)
##########################################
*/



/* 기능

1. gpio, opencv, pthread 등 초기 세팅 설정
2. 키패드 '*' 입력 시 비밀번호 입력 가능
3. 맞는 비밀번호 입력 시 서보모터가 작동 ( 잠금 해제 및 잠금 )
4. 비밀번호 3회 오류 시 영상 저장, 영상과 log기록을 메일로 전송 ( 30초간 시스템 일시 정지 )
5. 키패드 '#' 입력 시 비밀번호 세팅 모드
6. 비밀번호 입력, 비밀번호 확인
7. APP 어플로 smart_door에 접속 가능 ( 실시간 streaming, 원격 잠금, 잠금해제 )

*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include <pthread.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <softPwm.h>
#include <time.h>

//doorlock define
#define RED 21
#define GED 22

#define SW1  1		// 1
#define SW2  2		// 2
#define SW3  3		// 3
#define SW4  4		// 4
#define SW5  5		// 5
#define SW6  6		// 6
#define SW7  23		// 7
#define SW8  24		// 8
#define SW9  25		// 9
#define STAR 27		// *
#define SW0  28		// 0
#define WELL 29		// #

#define SERVO 0		//motor

#define N1 digitalRead(SW1)
#define N2 digitalRead(SW2)
#define N3 digitalRead(SW3)
#define N4 digitalRead(SW4)
#define N5 digitalRead(SW5)
#define N6 digitalRead(SW6)
#define N7 digitalRead(SW7)
#define N8 digitalRead(SW8)
#define N9 digitalRead(SW9)
#define NS digitalRead(STAR)
#define N0 digitalRead(SW0)
#define NW digitalRead(WELL)

//size define
#define width 320
#define height 240


using namespace cv;
using namespace std;

//################## cv
int fd;
pthread_mutex_t m_lock;
Mat frame;
VideoCapture capture(0);


char recvstat = 0;
char tarkcingmode = 0;

//쓰레드 함수
void *tcomu_function(void *data);		//쓰레드1 : 캠의 한 frame을 불러옴
void *tserver_function(void *data);		//쓰레드2 : 클라이언트의 접속대기, 클라이언트의 명령 수행
void *tracker_function(void *data);		//쓰레드3 : 쓰레드1이 불러온 frame을 서버에 올림 (streaming)
void *mail_function(void *data);		//쓰레드4 : 비밀번호가 3번 틀리고 영상 촬영 후 메일로 전송

void inputpassword(char *save, int pin);
void inputpassword(char *save, int pin, int pc);


int remotesocket_test = 0;

char* recvData = new char[1024]; //데이터 저장 변수 선언


char lock_mode = 0;			//모터 제어 변수
int mail_x = 0;				//메일 전송 변수
int b = 0;				// * or # 검출 변수
int c = 0;				//inputpassword 함수에서 while을 빠져나오기 위한 변수, main 에서 0으로 초기화
int k = 0;				//입력한 비밀번호의 갯수, pws와 pre 비교군, pre와 pre2 비교군

int pcb = 0;
int m = 0;				//첫번째 비밀번호 갯수 카운트
int n = 0;				//두번째 비밀번호 갯수 카운트
int bytes = 0;

//void *thread_function(void *org);

//################### cv

int main(int, char**)
{
	char filename[50] = "/home/pi/Videos/";
	//#################### cv

	//웹캡으로 부터 데이터 읽어오기 위해 준비
	void* s;

	pthread_t p_thread[4];
	int thr_id[4];
	int status[4];
	int a_tracker[4] = { 1,1,1,1 };



	while (true) {
		if (wiringPiSetup() == -1)
		{
			fprintf(stdout, "unable to start wiringPi: %s\n", strerror(errno));
		}
		else
			break;
		delay(2000);
	}
	while (true) {
		if ((fd = serialOpen("/dev/ttyAMA0", 115200)) < 0)
		{
			fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		}
		else
			break;
		delay(2000);
	}
	while (true) {
		if (pthread_mutex_init(&m_lock, NULL) != 0)
		{
			perror("Mutex Init failure");
		}
		else
			break;
		delay(2000);
	}
	while (true) {
		if (!capture.isOpened())
		{
			cout << "cam connect error" << endl;
		}
		else
			break;
		delay(2000);
	}


	//캡처 영상을 320x240으로 지정
	capture.set(CAP_PROP_FRAME_WIDTH, width);
	capture.set(CAP_PROP_FRAME_HEIGHT, height);


//########################## cv

//################################################################################
//cam setting

	namedWindow("input", 1);


	//웹캠에서 캡쳐되는 이미지 크기를 가져옴
	Size size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));

	//웹캠에서 캡처되는 속도를 가져옴
	int fps = capture.get(CAP_PROP_FPS);
	cout << fps << endl;

	//파일로 동영상을 저장하기 위한 준비
	VideoWriter outputVideo;
	outputVideo.open("/home/pi/Videos/output.avi", VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, size, true);

	if (!outputVideo.isOpened())
	{
		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
		return 1;
	}

	//pthread start
	thr_id[0] = pthread_create(&p_thread[0], NULL, tserver_function, (void *)&a_tracker[0]);
	thr_id[1] = pthread_create(&p_thread[1], NULL, tcomu_function, (void *)&a_tracker[1]);
	thr_id[2] = pthread_create(&p_thread[2], NULL, tracker_function, (void *)&a_tracker[2]);
	thr_id[3] = pthread_create(&p_thread[3], NULL, mail_function, (void *)&a_tracker[3]);

	//#############################################################################
	//gpio setting

	char pws[100] = { 0 };
	char pre[100] = { 1, 2, 3, 4 };
	char pre2[100] = { 0 };

	softPwmCreate(SERVO, 0, 200);

	int i = 0, m = 0, n = 0, temp = 0;
	int iv_num = 0;			//틀린 횟수 카운트
	int cnt = 4;

	int sec = 0;

	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);
	pinMode(SW5, INPUT);
	pinMode(SW6, INPUT);
	pinMode(SW7, INPUT);
	pinMode(SW8, INPUT);
	pinMode(SW9, INPUT);
	pinMode(STAR, INPUT);
	pinMode(SW0, INPUT);
	pinMode(WELL, INPUT);

	pinMode(RED, OUTPUT);
	pinMode(GED, OUTPUT);

	softPwmWrite(SERVO, 5);

	//##############################################################################
	//Password

	printf("<<<<< Select * or # >>>>>\n");
	printf(" * : Entering Password\n");
	printf(" # : Password Setting\n");
	printf("Default Password : 1 2 3 4\n\n");

	while (1)
	{
		if (iv_num == 3)
		{
			//비밀번호 3번 틀릴 시, 영상 시작
			while (1)
			{
				//화면에 영상을 보여줌
				imshow("input", frame);

				//동영상 파일에 한프레임을 저장함
				outputVideo << frame;

				//ESC키 누르면 종료
				if (waitKey(1) == 27) break;

				// 종료시간 검출 : 1sec = 30frame
				if (sec >= 900) break;

				sec += 1;
				delay(1);
			}
			sec = 0;
			b = 0;
			mail_x = 1;
			iv_num = 0;
		}

		// * 또는 # 입력을 기다림
		while (1)
		{
			if (NS == 1)
				b = 1;
			else if (b == 1)
				break;

			if (NW == 1)
				b = 2;
			else if (b == 2)
				break;

			delay(10);
		}

		//#########################################################
		// * or #

		switch (b)
		{
		// * 입력 시 비밀번호 입력
		case 1:
		{
			printf("Input Password!\n");

			inputpassword(pws, STAR);

			//비밀번호가 맞는지 확인
			if(strlen(pws) == strlen(pre))
			{
				for (i = 0; i < k; i++)
				{
					if (pws[i] == pre[i]) temp++;
				}
			}

			//결과에 따라 open 또는 nob 출력
			if (cnt == temp)
			{
				printf("Gate Open\n");
				b = 0;
				iv_num = 0;

				digitalWrite(GED, HIGH);
				softPwmWrite(SERVO, 23);
				delay(3000);

				digitalWrite(GED, LOW);
				softPwmWrite(SERVO, 5);

				//send : 클라이언트에 hello 전송
				//send(remotesocket_test, "hello", 5, 0);
			}
			else
			{
				printf("Password Error\n");

				iv_num++;

				digitalWrite(RED, HIGH);
				delay(500);
				digitalWrite(RED, LOW);
				delay(500);
			}

			//##########################################################
			//초기화
			for (i = 0; i < k; i++)
				pws[i] = 0;

			i = 0;
			temp = 0;
			k = 0;

			break;
		}

		//# 입력 시 비밀번호 세팅
		case 2:
		{
			cnt = 0;
			i = 0;

			for (i = 0; i < 100; i++)//비밀번호 초기화
			{
				pre[i] = 0;
				pre2[i] = 0;
			}

			printf("Password Setting Menu\n");

			inputpassword(pre, WELL, m);
			m = pcb;

			printf("Password Check\n");

			inputpassword(pre2, WELL, n);
			n = pcb;

			if(m == 0)
			{
				printf("다시 입력\n");
				break;
			}

			if (m == n)
			{
				for (i = 0; i < m; i++)
					if (pre[i] == pre2[i])//비밀번호 세팅 완료
						cnt++;

				if (cnt == m)
				{
					printf("Password Changed\n");

					for (i = 0; i < 3; i++)
					{
						digitalWrite(GED, HIGH);
						delay(500);
						digitalWrite(GED, LOW);
						delay(500);
					}

					b = 0;
				}
				else
				{
					printf("다시 입력\n");
					cnt = 0;
				}
			}
			else
			{
				printf("다시 입력\n");
			}

			i = 0;
			m = 0;
			n = 0;
			break;
		}
		}
	}

	//pthread exit
	pthread_join(p_thread[0], (void **)&status[0]);
	pthread_join(p_thread[1], (void **)&status[1]);
	pthread_join(p_thread[2], (void **)&status[2]);
	pthread_join(p_thread[3], (void **)&status[3]);

	return 0;
}



void *tracker_function(void *data)
{
	while (1)
	{
		capture >> frame;

		imwrite("copy.jpg", frame);
	}
}
void *tcomu_function(void *data)
{
	while(1)
	{
		pthread_t p_thread;

		int localSocket, remoteSocket; //소켓 변수 선언
		int port = 7777; // 포트 설정
		struct  sockaddr_in localAddr, remoteAddr; //서버, 클라 주소 구조체
		int addrLen = sizeof(struct sockaddr_in);

		localSocket = socket(PF_INET, SOCK_STREAM, 0); //소켓 생성(IPv4, tcp, tcp)
		if (localSocket == -1) {
			perror("socket() call failed!!");
		}

		//소켓 설정
		localAddr.sin_family = AF_INET; //주소체계 설정 - IPv4
		localAddr.sin_addr.s_addr = INADDR_ANY; //아이피주소 자동할당
		localAddr.sin_port = htons(port); //포트설정

		//bind
		//sockaddr_in 구조체로 설정한 서버소켓 설정 적용(적용될 소켓, 설정적용된구조체,크기)
		if (bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0) {
			perror("Can't bind() socket");
			exit(1);
		}

		listen(localSocket, 3);  //클라이언트 접속 대기
		std::cout << "Waiting for connections...\n" << "Server Port:" << port << std::endl;

		// 클라이언트 통신용 소켓 생성
		remotesocket_test = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);

		if (remotesocket_test < 0) {
			perror("accept failed!");
		}
		std::cout << "Connection accepted" << std::endl;
		while (1) {
			//데이터 수신
			if ((bytes = recv(remotesocket_test, recvData, 1023, 0)) == -1) {
		 		cout << "error on recv" << endl;
				delay(500);
			}
			if ((recvData[0] - 48) == 7)		//어플에서 lock off 를 선택 할 경우, 어플에서 라파로 '7'을 전송, 쓰레드2(TCP IP server thread) 가 7을 읽어 lock_mode에 1을 입력.
				lock_mode = 1;
			else if ((recvData[0] - 48) == 8)
				lock_mode = 0;
			else if((recvData[0] - 48) == 9)
				lock_mode = 2;

			if (tarkcingmode == 0) {
				serialPutchar(fd, (recvData[0] - 48));
			}

			if(lock_mode == 1)
				softPwmWrite(SERVO, 23);
			else
				softPwmWrite(SERVO, 5);

			//손 봐야함
			if(lock_mode == 2)
			{
				printf("클라이언트 아웃\n");
				delay(10000);
				break;
			}

			fflush(stdout);
		}
		close(remotesocket_test);
		close(localSocket);
	}
}


void *tserver_function(void *data)
{
	system("sudo sh /home/pi/product/serverrun1.sh");
}

void *mail_function(void *data)
{
	while(1)
	{
		if (mail_x == 1)
		{
			system("sudo ./time_box");
			mail_x = 0;
			printf("메일 전송\n");
		}
	}
}

void inputpassword(char *save, int pin)
{
	int a = 0;
	int i = 0;

	while (1)
	{

		while (1) //1개의 비밀번호 입력을 기다림
		{
			if (N1 == 1) a = 1;
			else if (N2 == 1) a = 2;
			else if (N3 == 1) a = 3;
			else if (N4 == 1) a = 4;
			else if (N5 == 1) a = 5;
			else if (N6 == 1) a = 6;
			else if (N7 == 1) a = 7;
			else if (N8 == 1) a = 8;
			else if (N9 == 1) a = 9;
			else if (N0 == 1)
			{
				save[k] = 0;
				k++;

				while (N0 == 1) delay(10);
				break;
			}
			else if (a != 0)
			{
				save[k] = a;
				k++;
				a = 0;

				break;
			}
			else if (digitalRead(pin) == 1)
			{
				c = 1;
				while (digitalRead(pin) == 1) delay(10);
				break;
			}
			delay(10);
		}

		if (c == 1) // * 입력시 break;
		{
			c = 0;
			break;
		}

		digitalWrite(GED, HIGH);
		delay(100);
		digitalWrite(GED, LOW);

		printf("%d\n", ++i);//인덱스
	}
}

void inputpassword(char *save, int pin, int pc)
{
	int a = 0;
	int i = 0;

	pcb = 0;

	while (1)
	{
		while (1) //1개의 비밀번호 입력을 기다림
		{
			if (N1 == 1) a = 1;
			else if (N2 == 1) a = 2;
			else if (N3 == 1) a = 3;
			else if (N4 == 1) a = 4;
			else if (N5 == 1) a = 5;
			else if (N6 == 1) a = 6;
			else if (N7 == 1) a = 7;
			else if (N8 == 1) a = 8;
			else if (N9 == 1) a = 9;
			else if (N0 == 1)
			{
				save[pcb] = 0;
				pcb++;

				while (N0 == 1) delay(10);
				break;
			}
			else if (a != 0)
			{
				save[pcb] = a;
				pcb++;
				a = 0;

				break;
			}
			else if (digitalRead(pin) == 1)
			{
				c = 1;
				while (digitalRead(pin) == 1) delay(10);
				break;
			}
			delay(10);
		}

		if (c == 1) // * 입력시 break;
		{
			c = 0;
			break;
		}

		digitalWrite(GED, HIGH);
		delay(100);
		digitalWrite(GED, LOW);

		printf("%d\n", ++i);//인덱스
	}

}
