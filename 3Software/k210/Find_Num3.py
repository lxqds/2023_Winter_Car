# Untitled - By: lxq - 周四 1月 19 2023

import sensor, image, time ,lcd
lcd.init(freq=15000000,invert=1)
clock = time.clock()
sensor.reset(freq=24000000,dual_buff=True)
sensor.set_vflip(1)
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.set_contrast(2)
sensor.run(1)
sensor.skip_frames()

num_quantity = 8
num_model = []
templates = []
img = image.Image("/sd/2.jpg")
img = img.to_grayscale(copy=False)
templates.append(image.Image("/sd/1.jpg"))
templates.append(image.Image("/sd/1.jpg"))
templates.append(image.Image("/sd/2.jpg"))
templates.append(image.Image("/sd/3.jpg"))
templates.append(image.Image("/sd/4.jpg"))
templates.append(image.Image("/sd/5.jpg"))
templates.append(image.Image("/sd/6.jpg"))
templates.append(image.Image("/sd/7.jpg"))
templates.append(image.Image("/sd/8.jpg"))
#templates[1] = image.Image("/sd/xianyu.jpg")
#for n in range(1,8):
#    templates[n] = templates[n].to_grayscale(copy=True)

#lcd.display(templates[1])
#while(True):
#    lcd.display(templates[2])
#templates[2] = image.Image("/sd/2.jpg")
#templates[3] = image.Image("/sd/3.jpg")
#templates[4] = image.Image("/sd/4.jpg")
#templates[5] = image.Image("/sd/5.jpg")
#templates[6] = image.Image("/sd/6.jpg")
#templates[7] = image.Image("/sd/7.jpg")
#templates[8] = image.Image("/sd/8.jpg")
#num_model[1] = image.Image("/sd/1.pgm")
#for n in range(1,num_quantity):
#     num_model.append(image.Image("/sd/1.pgm"))
#    num_model.append(image.Image("/sd/"+str(n)+".pgm"))
#img_colorful = sensor.alloc_extra_fb(160,120,sensor.RGB565)
#img_to_matching = sensor.alloc_extra_fb(35,45,sensor.GRAYSCALE)
threshold=(0,30)#黑色色块阈值
scale = 1
fps = 0
img = sensor.snapshot()
img.save("/sd/test.jpg")
img_to_matching = img.draw_image(img,0,0,roi=(0,0,35,40))

def find_max(blobs):#定义找最大的色块
    max_size = 0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob = blob
            max_size = blob[2]*blob[3]
    return max_blob

while(True):
    clock.tick()

    img_colorful = img = sensor.snapshot()
    #img = img.to_grayscale(copy=True)
    #img_colorful = sensor.snapshot()
    blobs = img_colorful.find_blobs([threshold])
    if blobs:
        max_blobs = find_max(blobs)#选择最大的
        #for blob in blobs:
        if  100>max_blobs.h()>10 and max_blobs.w()>50:
            scale = 40/max_blobs.h()
            img_to_matching.draw_image(img_colorful,0,0,roi=(max_blobs.x()-2,max_blobs.y()-2,max_blobs.w()+4,max_blobs.h()+4),x_scale=scale,y_scale=scale)
            for n in (1,num_quantity):
                r = img_to_matching.find_template(templates[n],0.7,step=2,search=image.SEARCH_EX)
                if r:
                    img_colorful.draw_rectangle(max_blobs[0:4],color=(255,255,255))
                    img_colorful.draw_string(max_blobs[0],max_blobs[1],str(n+1),scale=2,color = (255,255,255))
    img_colorful.draw_string(0,0,("%2.1f" %(fps)),color=(255,255,255),scale=2)
    lcd.display(img_colorful)
    fps = clock.fps()
