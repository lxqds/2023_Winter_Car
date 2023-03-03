import sensor,image,lcd,time
from Maix import freq
from machine import UART
from fpioa_manager import fm
import ustruct
import MathMatrix
from machine import Timer
from Maix import FPIOA
from Maix import GPIO

#-----------------------------上电初始化----------------------------------------------------------------------
fpioa = FPIOA()
fpioa.set_function(0,fpioa.GPIOHS0)
fpioa.set_function(17,fpioa.GPIOHS1)
led1 = GPIO(GPIO.GPIOHS0,GPIO.OUT)
led2 = GPIO(GPIO.GPIOHS1,GPIO.OUT)

fm.register(21, fm.fpioa.UART1_TX, force=True)
fm.register(23, fm.fpioa.UART1_RX, force=True)

uart_A = UART(UART.UART1, 115200, 8, 0, 0, timeout=1000, read_buf_len=4096)

lcd.init(freq=15000000,invert = 1)
sensor.reset(freq = 24000000,dual_buff=True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(0)
sensor.set_vflip(0)
sensor.set_auto_gain(False,5)
sensor.set_auto_whitebal(False)
sensor.set_auto_exposure(True)
sensor.set_brightness(2)
sensor.set_saturation(0)
sensor.run(1)
sensor.skip_frames()
#-----------------------------上电初始化--------------------------------------------------------------------

def SendNum(cx,cy,leng,Kal_x,Kal_y,Kal_len): #串口发送函数
    data = ustruct.pack("<bHHfHHf",
                0xAA,
                cx,
                cy,
                leng,
                Kal_x,
                Kal_y,
                Kal_len)
    uart_A.write(data)

def GetSpeed(lastPosion,currPosion,speedtime):#计算速度函数
    speed = [0,0,0]
    speed[0] = (currPosion[0] - lastPosion [0])/speedtime*1000
    speed[1] = (currPosion[1] - lastPosion [1])/speedtime*1000
    speed[2] = (currPosion[2] - lastPosion [2])/speedtime*1000
    return speed

#________________________________初始化噪声、误差协方差矩阵及相关变量___________________________________________
speedTime = 0
A = [[1,speedTime*0.001],[0,1]]
P_cx = [[1,1],[1,1]]
Kcx = 1
Kvx = 1
X_cx,X_vx = 0,0
Qx = [1,1]
Rcx,Rvx = 5,5
#________________________________卡尔曼滤波器函数定义___________________________________________
def Kalman_x_filter(cx,vx):
    global A,P_cx,Kcx,Kvx,X_cx,Qx,Rcx,Rvx,X_vx
    #预测方程
    X_cx = X_cx + X_vx*speedTime*0.001
    #预测协方差矩阵
    P_cx[0][0] = P_cx[0][0] + (P_cx[1][0]+P_cx[0][1])*speedTime*0.001 + P_cx[1][1]*speedTime*speedTime*0.001*0.001 + Qx[0]
    P_cx[0][1] = P_cx[0][1] + P_cx[1][1]*speedTime*0.001
    P_cx[1][0] = P_cx[1][0] + P_cx[1][1]*speedTime*0.001
    P_cx[1][1] = P_cx[1][1] + Qx[1]
    #测量方程即cx vx

    #计算卡尔曼增益
    Kcx = P_cx[0][0]/(P_cx[0][0]+Rcx)
    Kvx = P_cx[1][0]/(P_cx[0][0]+Rcx)

    #计算最优估计值
    X_cx = X_cx + Kcx*(cx - X_cx)
    X_vx = X_vx + Kvx*(vx - X_vx)
    #更新协方差矩阵
    ppre1 =  P_cx[0][0]
    ppre2 =  P_cx[0][1]
    P_cx[0][0] = P_cx[0][0]*(1 - Kcx)
    P_cx[0][1] = P_cx[0][1]*(1 - Kcx)
    P_cx[1][0] = P_cx[1][0] - Kvx*ppre1
    P_cx[1][1] = P_cx[1][1] - Kvx*ppre2

    return X_cx,X_vx

P_cy = [[1,1],[1,1]]
Kcy = 1
Kvy = 1
X_cy,X_vy = 1,1
Qy = [1,1]
Rcy,Rvy = 5,5
X_est,V_est = 1,1
Y_est,YV_est = 1,1
def Kalman_y_filter(cy,vy):
    global A,P_cy,Kcy,Kvy,X_cy,Qy,Rcy,Rvy,X_vy
    #预测方程
    X_cy = X_cy + X_vy*speedTime*0.001
    #预测协方差矩阵
    P_cy[0][0] = P_cy[0][0] + (P_cy[1][0]+P_cy[0][1])*speedTime*0.001 + P_cy[1][1]*speedTime*speedTime*0.001*0.001 + Qy[0]
    P_cy[0][1] = P_cy[0][1] + P_cy[1][1]*speedTime*0.001
    P_cy[1][0] = P_cy[1][0] + P_cy[1][1]*speedTime*0.001
    P_cy[1][1] = P_cy[1][1] + Qy[1]
    #测量方程即cx vx

    #计算卡尔曼增益
    Kcy = P_cy[0][0]/(P_cy[0][0]+Rcx)
    Kvy = P_cy[1][0]/(P_cy[0][0]+Rcx)
    #计算最优估计值
    X_cy = X_cy + Kcx*(cy - X_cy)
    X_vy = X_vy + Kvx*(vy - X_vy)
    #更新协方差矩阵
    ppre1 =  P_cy[0][0]
    ppre2 =  P_cy[0][1]
    P_cy[0][0] = P_cy[0][0]*(1 - Kcx)
    P_cy[0][1] = P_cy[0][1]*(1 - Kcx)
    P_cy[1][0] = P_cy[1][0] - Kvy*ppre1
    P_cy[1][1] = P_cy[1][1] - Kvy*ppre2
    return X_cy,X_vy

P_len = [[1,1],[1,1]]
Klen = 1
Kvlen = 1
X_len,X_vlen = 1,1
Qlen = [1,1]
Rlen,Rvlen = 200,200
getKalmanLen , getKalmanLenv = 1,1

def Kalman_len_filter(length,lenv):
    global A,P_len,Klen,Kvlen,X_len,X_vlen,Qlen,Rlen,Rvlen
    #预测方程
    X_len = X_len + X_vlen*speedTime*0.001
    #预测协方差矩阵
    P_len[0][0] = P_len[0][0] + (P_len[1][0]+P_len[0][1])*speedTime*0.001 + P_len[1][1]*speedTime*speedTime*0.001*0.001 + Qlen[0]
    P_len[0][1] = P_len[0][1] + P_len[1][1]*speedTime*0.001
    P_len[1][0] = P_len[1][0] + P_len[1][1]*speedTime*0.001
    P_len[1][1] = P_len[1][1] + Qlen[1]
    #测量方程即cx vx

    #计算卡尔曼增益
    Klen = P_len[0][0]/(P_len[0][0]+Rlen)
    Kvlen = P_len[1][0]/(P_len[0][0]+Rvlen)
    #计算最优估计值
    X_len = X_len + Klen*(length - X_len)
    X_vlen = X_vlen + Kvlen*(lenv - X_vlen)
    print(Klen)
    #更新协方差矩阵
    ppre1 =  P_len[0][0]
    ppre2 =  P_len[0][1]
    P_len[0][0] = P_len[0][0]*(1 - Klen)
    P_len[0][1] = P_len[0][1]*(1 - Klen)
    P_len[1][0] = P_len[1][0] - Kvlen*ppre1
    P_len[1][1] = P_len[1][1] - Kvlen*ppre2
    return X_len,X_vlen
clock = time.clock()
fps = 0
clock.tick()

green_threshold   = (22, 92, -67, -19, -11, 72)
Length_K = 1100
last_state = [0,0,0] #cx , cy , len
speed_All = [0,0,0] #vx , vy , vlen
Findflag = 0
rect2 = 10
rect3 = 10
while True:
    img = sensor.snapshot()#获取摄像头图片
    blobs = img.find_blobs([green_threshold], x_stride=10, y_stride=10,area_threshold =150)#找色块

    if blobs:#找到后
        #找到目标值后找出最大并得到相应数据
        max_blob = 0
        for b in blobs:
            if b[4] >= max_blob:
                max_blob = b[4]
                max_block = b
        tmp=img.draw_rectangle(max_block[0:4]) #框出目标
        img.draw_cross(max_block[5], max_block[6], color = (0, 0, 0))

        Lm = (max_block[2]+max_block[3])/2  #计算距离
        length = Length_K/Lm  #计算距离
        #找到目标值后找出最大并得到相应数据

        currPosion = [max_block[5],max_block[6],length] #记录当前数值便于计算速度

        if Findflag == 1:#计算速度
            speed_All = GetSpeed(last_state,currPosion,speedTime)
            A = [[1,speedTime],[0,1]]
            X_est,V_est=Kalman_x_filter(currPosion[0],speed_All[0])
            Y_est,YV_est=Kalman_y_filter(currPosion[1],speed_All[1])
            getKalmanLen , getKalmanLenv = Kalman_len_filter(currPosion[2],speed_All[2])
            SendNum(max_block[5],max_block[6],length,int(X_est),int(Y_est),getKalmanLen)#串口发送给stm32
            img.draw_rectangle(int(X_est-(max_block[2])/2),int(Y_est-(max_block[3])/2),max_block[2],max_block[3],color = (255, 0, 0))
            img.draw_cross(int(X_est), int(Y_est), color = (255, 0, 255))
            img.draw_string(0,48, ("L:%2.2f" %(getKalmanLen)), color=(0,255,0), scale=2) #LCD显示
        img.draw_string(0,0, ("cx:%d,cy:%d,l:%2.2f" %(max_block[5],max_block[6],length)), color=(0,255,0), scale=2) #LCD显示

        Findflag = 1
        last_state = currPosion

        rect2 = (max_block[2])
        rect3 = (max_block[3])
        T = speedTime


    else:#未找到色块
        A = [[1,speedTime],[0,1]]
        a=2
        X_est,V_est=Kalman_x_filter(X_est,a*speed_All[0])
        Y_est,YV_est=Kalman_y_filter(Y_est,a*speed_All[1])
        img.draw_rectangle(int(X_est-rect2/2),int(Y_est-rect3/2),rect2,rect3,color = (0, 0, 255))
        img.draw_cross(int(X_est), int(Y_est), color = (255, 0, 0))

        Findflag = 0
    #计算fps
    img.draw_string(0,24, ("%2.1ffps" %(fps)), color=(255,255,255), scale=2)
    fps = clock.fps()
    speedTime = clock.avg()
    clock.tick()
    lcd.display(img)
    #print(fps)


