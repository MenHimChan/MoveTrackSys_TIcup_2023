import sensor, image, time, math
from pyb import *
import struct
import json
uart1 = UART(3,9600)
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # use RGB565.GRAYSCALE
sensor.set_framesize(sensor.VGA) # use QQVGA for speed.
sensor.set_brightness(-3)
sensor.set_contrast(3)
sensor.set_gainceiling(2)
#sensor.set_auto_gain(False,gain_db=5)
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
sensor.set_vflip(True)  #因摄像头安装方向倒转 我们没有
sensor.set_hmirror(False)#需要水平、垂直翻转
clock = time.clock() # Tracks FPS.
x_error=0.0; #X轴偏差,就是实际坐标减去中点坐标得到的误差，然后摄像头旋转去减少这个误差即可，以下变量同理
y_error=0.0; #Y轴偏差
pan_error=0.0; #X轴偏差
tilt_error=0.0; #Y轴偏差
thresholds = (205, 255)


def find_light():
    while(True):
        img = sensor.snapshot()
        blobs = img.find_blobs([thresholds], merge=True)
        if blobs == None:
            continue
        for blob in blobs:
            # These values depend on the blob not being circular - otherwise they will be shaky.
            #if blob.elongation() > 0.5:
                #img.draw_edges(blob.min_corners(), color=0)
                #img.draw_line(blob.major_axis_line(), color=0)
                #img.draw_line(blob.minor_axis_line(), color=0)
            ## 这些值始终是稳定的。
            #img.draw_rectangle(blob.rect(), color=127)
            #img.draw_cross(blob.cx(), blob.cy(), color=127)
            ## 注意 - 色块的rotation旋转是0-180内的唯一。
            #img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=40, color=127)

            return [blob.cx(),blob.cy()]

def pro1_1(target_x,target_y,error_d,e_x,e_y,cnt_set=10):
    #tar_x,tar_y 为目标（x,y) error_d——为允许的偏差 ,e_x，e_y为调正复位的x，y的大小
    cnt=0
    while(True):
        new_x,new_y=find_light()
        pan_error = new_x-target_x+e_x #这里计算x轴偏差值
        tilt_error = new_y-target_y+e_y  #这里计算y轴偏差值
        if(pan_error>0 or pan_error==0): #这里因为偏差值有负数，但是呢负数直接通过串口发送给单片机是不行的，所以当为负数时我就+100，也就是+50发的就是50，-50发的就是150
            x_error=pan_error
            thousands_str='+'
        elif pan_error<0 :
            x_error=abs(pan_error)
            thousands_str='-'
        if tilt_error>=0 :
            y_error=tilt_error
            thousands_str_y='+'
        elif tilt_error<0 :
            y_error=abs(tilt_error)
            thousands_str_y='-'
        #这里是画出色块的框

        hundreds =int(x_error/100%10) #取百位
        tens =int(x_error/10%10) #取十位
        ones =int(x_error%10) #取个位
        #将数字转化成ASCLL码发出去
        hundreds_str=str(hundreds)
        tens_str=str(tens)
        ones_str=str(ones)
       # 同上
        hundreds_y =int(y_error/100%10)
        tens_y =int(y_error/10%10)
        ones_y =int(y_error%10)
        hundreds_str_y=str(hundreds_y)
        tens_str_y=str(tens_y)
        ones_str_y=str(ones_y)
       #这里整和数据x轴和y轴偏差发送过去
        combined_str=thousands_str+hundreds_str+tens_str+ones_str+thousands_str_y+hundreds_str_y+ tens_str_y+ones_str_y
        uart1.write(combined_str.encode())
        if x_error<=error_d and y_error<=error_d:
            if cnt>=cnt_set:
                return
            cnt+=1
        else:
            cnt=0
        #uart1.write('\r\n') #发送结尾必须以这个结尾,待会儿STM32端会说明

        print('{}{}'.format(thousands_str,x_error))
        print('{}{}'.format(thousands_str_y,y_error))
        #print('{} {}'.format(new_x,new_y))
        #delay(50)

#ab = [[515,246],[537,74],[283,36],[252,209],[515,246]]
def returnRecPoint():
    #sensor.reset()
    sensor.set_framesize(sensor.QQVGA)
    sensor.skip_frames(10)
    cnt = 0
    resu = [[0,0],[0,0],[0,0],[0,0]]
    last_r=0
    #kernel_size = 1 # kernel width = (size*2)+1, kernel height = (size*2)+1
    #kernel = [-2, -2, -2,\
              #-2, +1, -2,\
              #-2, -2, -4]
    while True:
        img = sensor.snapshot()
        #img = img.morph(kernel_size, kernel)
        img.midpoint(1, bias=0.9,threshold = True,offset=10,invert=True)
    # -----矩形框部分-----
        # 在图像中寻找矩形
        for r in img.find_rects():  # threshold = 10000
            # 判断矩形边长是否符合要求
            if r.w() > 20 and r.h() > 20:
                if abs(last_r - r.w())<=2:
                    cnt = cnt  + 1
                else :
                    last_r = r.w()
                    cnt = 0
                # 在屏幕上框出矩形
                img.draw_rectangle(r.rect(), color = (255, 0, 0), scale = 4)
                # 获取矩形角点位置
                corner = r.corners()
                # 在屏幕上圈出矩形角点
                img.draw_circle(corner[0][0], corner[0][1], 5, color = (0, 0, 255), thickness = 2, fill = False)
                img.draw_circle(corner[1][0], corner[1][1], 5, color = (0, 255, 0), thickness = 2, fill = False)
                img.draw_circle(corner[2][0], corner[2][1], 5, color = (255, 0, 0), thickness = 2, fill = False)
                img.draw_circle(corner[3][0], corner[3][1], 5, color = (0, 0, 0), thickness = 2, fill = False)
                if cnt  > 20 :
                    for i in range(4):
                        for j in range(2):
                                resu[i][j]=corner[i][j]*4
                    resu.append(resu[0])
                    print(resu)
                    sensor.set_framesize(sensor.VGA)
                    sensor.skip_frames(10)
                    return resu


def get_more(cur):
    a = cur[:]
    res = a[:]
    for i in range(4):
        length = (len(a))
        for j in range(length - 1):
            num = [0,0]
            for k in range(2):
                num[k] = (res[j][k] + res[j + 1][k]) // 2
            a.insert(2 * j + 1, num)
        res = a[:]
    return res

flag1 = False
#flag1 = True
flag2 = False
flag3 = False
low_threshold=(-4, 4, -4, 4, -4, 4)
fuhao = [[+1,+1],[-1,+1],[-1,-1],[+1,-1]]
dingdian=[(111,466),(342,464),(573,462),(565,267),(557,11),(334,13),(112,16),(111,241),(111,466)]
uart1.write("+800+800".encode())
while(True):
    if uart1.any():
        data = uart1.read()
        data=data.decode('utf-8')
        if (data == '1' ):
            flag1 = True
        elif (data == '2' ):
            flag2 = True
        elif (data == '3' ):
            flag3 = True
    if flag1:
        LED(1).on()
        pro1_1(640//2,480//2,10,-13,+9)
        delay(200)
        uart1.write("ffffffff".encode())
        LED(1).off()
        flag1=False
        continue
    if flag2:
        LED(1).on()
        for i in dingdian:
            pro1_1(i[0],i[1],20,0,0,cnt_set=1)
            delay(5)
        delay(100)
        uart1.write("ffffffff".encode())
        flag2=False
        LED(1).off()
        continue
    if flag3:
        LED(1).on()
        cor=returnRecPoint()
        new = get_more(cor)
        for i in new:
            pro1_1(i[0],i[1],20,0,0,cnt_set=1)
        delay(100)
        uart1.write("ffffffff".encode())
        flag3=False
        LED(1).off()
        continue
    delay(10)
