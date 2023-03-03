# Untitled - By: 27825 - 周日 8月 14 2022

import image
import lcd
import time
import sensor
import KPU as kpu

clock = time.clock()
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.set_hmirror(0)
sensor.run(1)
task = kpu.load(0x300000) #使用kfpkg将 kmodel 与 maixpy 固件打包下载到 flash
anchor = (3.25, 3.13, 2.22, 2.19, 4.62, 4.62, 1.41, 1.38, 0.91, 0.84) #通过K-means聚类算法计算
a = kpu.init_yolo2(task, 0.6, 0.3, 5, anchor)
classes=["pingpong"] #标签名称要和你训练时的标签名称顺序相同
while(True):
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    try:
        code = kpu.run_yolo2(task,img)
    except:
        print("TypeError")
    #a=img.draw_rectangle([0,0,360,20],0xFFFF,1,1)
    if code:
        for i in code:
            a=img.draw_rectangle(i.rect(),(255, 255, 255),1,0)
            a=lcd.display(img)
            # = img.draw_string(0,0, classes[i.classid()], color=(0,255,0), scale=2)
            for i in code:
                lcd.draw_string(i.x()+40,i.y()-30,classes[i.classid()] , lcd.RED,  lcd.WHITE)
                lcd.draw_string(i.x()+40,i.y()-10,str(round((i.value()*100),2))+"%", lcd.RED,  lcd.WHITE)
    else:
        img.draw_string(2,2,("%2.1ffps" %(fps)),color = (256,0,0),scale = 1.5)
        a = lcd.display(img)
    print((fps))

a = kpu.deinit(task)
