# Untitled - By: 27825 - 周二 8月 16 2022

import sensor
import image
import lcd
import time

import ustruct#导入ustruct模块方便转换数据

from machine import UART
from board import board_info
from fpioa_manager import fm
fm.register(24,fm.fpioa.UART1_TX,force=True)
fm.register(25,fm.fpioa.UART1_RX,force=True)

lcd.init(freq=15000000,invert=1)
clock = time.clock()
lcd.init()
sensor.reset(freq=24000000,dual_buff=True)
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
#sensor.set_hmirror(1) #改摄像头 你参数
sensor.set_vflip(1)
sensor.set_auto_gain(False,5)
sensor.set_auto_whitebal(True)
sensor.set_auto_exposure(True)
sensor.set_brightness(0)
sensor.set_saturation(0)
sensor.set_contrast(2)
sensor.run(1)
sensor.skip_frames()
#导入数字模型
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.set_contrast(2)
num_quantity = 8
num_model = []
for n in range(1,num_quantity):
    num_model.append(image.Image('/E/'+str(n)+'.pgm'))
img_colorful = sensor.alloc_extra_fb(160,120,sensor.RGB565)
img_to_matching = sensor.alloc_extra_fb(35,45,sensor.GRAYSCALE)
threshold=(0,70)#黑色色块阈值
scale = 1
while(True):
    clock.tick()
    img = sensor.snapshot()
    img_colorful.draw_image(img,0,0)
    blobs = img.find_blobs([threshold])
    if blobs:
        for blob in blobs:
            if blobs.pixels()>50 and 100>blob.h()>10 and blob.w()>3:
                scale = 40/blob.h()
                img_to_matching.draw_image(img,0,0,roi=(blob.x()-2,blob.y()-2,blob.w()+4,blob.h()+4),x_scale=scale,y_scale=scale)
                for n in (range0,num_quantity):
                    r = img_to_matching.find_template(num_model[n],0.7,step=2,search=image.SEARCH_EX)
                    if r:
                        img_colorful.draw_rectangle(blob[0:4],color=(255,0,0))
                        img_colorful.draw_string(blob[0],blob[1],str(n+1),scale=2,color = (255,0,0))
    img_colorful.draw_string(0,0,str(round(clock.fps(),2)))
    lcd.display(img_colorful)


def find_max(blobs):#定义找最大的色块
    max_size = 0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob = blob
            max_size = blob[2]*blob[3]
    return max_blob

def Send_Data(X,Y,D):
    global uart
    data = ustruct.pack("<bbhhfb",#数据包的格式控制
                              0x23,#数据包头部1
                              0x66,#数据包头部2
                              int(X),#数据1
                              int(Y),#数据2
                              float(D),#数据3
                              0x11)#数据包结束
    uart = UART(UART.UART1,115200,8,0,1,timeout=1000,read_buf_len=4096)
    uart.write(data);

green_threshold = (22, 92, -67, -19, -11, 72)
size_threshold = 2000

K=1360
Distance=0
x_stride=10
y_stride=10
area_threshold =150
fps = 0
while True:
    clock.tick()
    img = sensor.snapshot()#获取图片

    blobs = img.find_blobs([green_threshold], x_stride=10, y_stride=10,area_threshold =150)#找色块

    if blobs:
        max_blobs = find_max(blobs)#选择最大的
        for b in blobs:#遍历blobs里的值
            img.draw_rectangle(max_blobs[0:4])
            img.draw_cross(max_blobs[5],max_blobs[6])
            #计算距离
            Lm = (max_blobs[2]+max_blobs[3])/2
            Distance = K/Lm
            if Distance < 150:
                Send_Data(max_blobs[5],max_blobs[6],Distance)
            else:
                Send_Data(-1,-1,-1)
        img.draw_string(0,2,("x0=%s y0=%s Distance=%s\n "%(max_blobs[5],max_blobs[6],Distance)),color = (0,255,255),scale = 1.5)
    else:
        Send_Data(-1,-1,-1)
    img.draw_string(0,24, ("%2.1ffps" %(fps)), color=(255,255,255), scale=2)

    lcd.rotation(0)
    lcd.display(img)
    fps = clock.fps()


