# object detector boot.py
# generated by maixhub.com

import sensor, image, lcd, time
import KPU as kpu
import gc, sys

import ustruct#导入ustruct模块方便转换数据

from machine import UART
from board import board_info
from fpioa_manager import fm
fm.register(24,fm.fpioa.UART1_TX,force=True)
fm.register(25,fm.fpioa.UART1_RX,force=True)

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

def lcd_show_except(e):
    import uio
    err_str = uio.StringIO()
    sys.print_exception(e, err_str)
    err_str = err_str.getvalue()
    img = image.Image(size=(224,224))
    img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
    lcd.display(img)

def main(anchors, labels = None, model_addr="/sd/m.kmodel", sensor_window=(224, 224), lcd_rotation=0, sensor_hmirror=False, sensor_vflip=False):
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_windowing(sensor_window)
    sensor.set_hmirror(sensor_hmirror)
    sensor.set_vflip(sensor_vflip)
    sensor.run(1)

    lcd.init(type=1)
    lcd.rotation(lcd_rotation)
    lcd.clear(lcd.WHITE)

    if not labels:
        with open('labels.txt','r') as f:
            exec(f.read())
    if not labels:
        print("no labels.txt")
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "no labels.txt", color=(255, 0, 0), scale=2)
        lcd.display(img)
        return 1
    try:
        img = image.Image("startup.jpg")
        lcd.display(img)
    except Exception:
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "loading model...", color=(255, 255, 255), scale=2)
        lcd.display(img)

    task = kpu.load(model_addr)
    kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]
    try:
        while 1:
            string ="0"
            img = sensor.snapshot()
            t = time.ticks_ms()
            objects = kpu.run_yolo2(task, img)
            t = time.ticks_ms() - t
                Send_Data(0,0,0)
            img.draw_string(0, 200, "t:%dms     %s" %(t,string), scale=2, color=(255, 0, 0))
            lcd.display(img)
    except Exception as e:
        raise e
    finally:
        kpu.deinit(task)


if __name__ == "__main__":
    try:
        labels = ['8', '1', '3', '2', '5', '4', '6', '7']
        anchors = [2.78, 2.56, 4.25, 3.19, 4.06, 4.38, 3.34, 3.53, 5.31, 5.09]
        #main(anchors = anchors, labels=labels, model_addr=0x300000, lcd_rotation=2, sensor_window=(224, 224))
        main(anchors = anchors, labels=labels, model_addr="/sd/m.kmodel", lcd_rotation=2, sensor_window=(224, 224))
    except Exception as e:
        sys.print_exception(e)
        lcd_show_except(e)
    finally:
        gc.collect()
