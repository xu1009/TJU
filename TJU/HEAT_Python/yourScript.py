#-*- coding=utf-8 -*-
from Tkinter import *
import ttk
import  tkMessageBox
# import Tkinter
import serial
import serial.tools.list_ports
import  binascii
import xlrd
from xlrd import xldate_as_tuple
import xlwt
import time
import threading
numOfSer = []
command = '\xFE\xFE\xFE\xFE\xFE\x68\x20\x45\x20\x00\x16\x00\x00\x00\x01\x03\x90\x1F\x12\xC8\x16'


def GetData():
    global row
    global classOfTst
    global i
    time.sleep(60)
    i+=1
    print classOfTst.get()
    
    ser = serial.Serial(port=classOfTst.get(), baudrate=2400, parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS)
    if ser.isOpen():
        ser.write(command)
        data = binascii.b2a_hex(ser.read(59))
        print data
        SumHeat = ((float)(data[38])) / 10 + ((float)(data[39])) / 100 + (int)(data[41]) + (int)(data[40]) * 10 + (int)(
            data[42]) * 1000 + (int)(data[43]) * 100 + (int)(data[44]) * 100000 + (int)(data[45]) * 10000
        HeatRate = ((float)(data[48])) / 10 + ((float)(data[49])) / 100 + (int)(data[51]) + (int)(data[50]) * 10 + (int)(
            data[52]) * 1000 + (int)(data[53]) * 100 + (int)(data[54]) * 100000 + (int)(data[55]) * 10000
        FlowRate = ((float)(data[58])) / 1000 + ((float)(data[59])) / 10000 + ((float)(data[60])) / 10 + ((float)(
            data[61])) / 100 + (int)(data[62]) * 10 + (int)(data[63]) + (int)(data[64]) * 1000 + (int)(data[65]) * 100
        SumFlow = ((float)(data[68])) / 10 + ((float)(data[69])) / 100 + (int)(data[71]) + (int)(data[70]) * 10 + (int)(
            data[72]) * 1000 + (int)(data[73]) * 100 + (int)(data[74]) * 100000 + (int)(data[75]) * 10000
        InTem = ((float)(data[78])) / 10 + ((float)(data[79])) / 100 + (int)(data[81]) + (int)(data[80]) * 10 + (int)(
            data[82]) * 1000 + (int)(data[83]) * 100
        OutTem = ((float)(data[84])) / 10 + ((float)(data[85])) / 100 + (int)(data[87]) + (int)(data[86]) * 10 + (int)(
            data[88]) * 1000 + (int)(data[89]) * 100
        SumTime = (int)(data[91]) + (int)(data[90]) * 10 + (int)(data[93]) * 100 + (int)(data[92]) * 1000 + (int)(
            data[95]) * 100000 + (int)(data[94]) * 10000
        Time = data[108] + data[109] + data[106] + data[107] + '-' + data[104] + data[105] + '-' + data[102] + data[
            103] + ' ' + data[100] + data[101] + ':' + data[98] + data[99] + ':' + data[96] + data[97]
        dataShow.delete(0.0,END)
        
        showData = '设备时间: '+(str)(Time)+' 累计热量: '+(str)(SumHeat)+' 热量: '+(str)(HeatRate)+' 流量: '+(str)(FlowRate)+' 累计流量: '+(str)(SumFlow)+' 入口温度: '+(str)(InTem)+' 出口温度: '+(str)(OutTem)+' 累计运行时间: '+(str)(SumTime)
        print showData
        dataShow.insert(1.0,showData)
        print  SumHeat, HeatRate, FlowRate, SumFlow, InTem, OutTem, SumTime, Time
        ttt = intervalEntry.get()
        #ttt1 = (float)(ttt.split('.')[0])+(float)(ttt.split('.')[1])/10
        ttt1 = (int)(ttt)
        #ttt1 = ttt1*60
        #print 'ttt1'+tt1
        print ttt1
        if i==ttt1:
            i=0
            row += 1
            print row
            newSheet.write(row, 2, Time)
            newSheet.write(row, 3, SumHeat)
            newSheet.write(row, 4, HeatRate)
            newSheet.write(row, 5, FlowRate)
            newSheet.write(row, 6, SumFlow)
            newSheet.write(row, 7, InTem)
            newSheet.write(row, 8, OutTem)
            newSheet.write(row, 9, SumTime)
            workbook.save('./t1.xls')
        ser.close()
    else:
        tkMessageBox.showwarning('Waring','串口打开失败！')
def runStart():
    global flag1
    global i
    flag1 = False
    # intervalEntry['state'] = 'readonly'
    while True:
        if flag1:
            GetData()
            

def startGetData():
    global flag1
    intervalEntry['state'] = 'readonly'
    # dataShow['state'] = 'readonly'
    flag1=True
    print flag1
def stopGetData():
    global flag1
    intervalEntry['state'] = 'normal'
    # dataShow['state'] = 'normal'
    flag1 =False
    print flag1
def guiDisplay():
    global  root
    global flag1
    global imgDis
    global genderMale
    global genderFemale
    global classOfTst
    global vm
    global vf
    global workbook
    global newSheet
    global dataShow
    global intervalEntry
    global i
    global row
    row = 0
    i=0
    root = Tk()
    root.resizable(False, False)
    workbook = xlwt.Workbook()
    newSheet = workbook.add_sheet(u'sheet1', cell_overwrite_ok=True)
    startButton = Button(root,text = '开始',command=startGetData)
    endButton = Button(root, text='结束',command=stopGetData)
    ComLabel = Label(root,text='COM口')
    timeInterval = Label(root, text='采集时间间隔')
    dataOfNow = Label(root, text='当前数据')
    intervalEntry = Entry(root)
    dataShow = Text(root,height=5)
    name = StringVar()
    classOfTst = ttk.Combobox(root, textvariable=name)
    port_list = list(serial.tools.list_ports.comports())
    if not len(port_list):
        tkMessageBox.showwarning("Waring","没有发现串口！")
    else:
        for port in port_list:
            numOfSer.append(port[0])
        classOfTst['values'] = tuple(numOfSer)
        classOfTst.current(0)
    ComLabel.grid(row=2,column=2)
    classOfTst.grid(row=2,column=3)
    timeInterval.grid(row=2,column=6)
    intervalEntry.grid(row=2,column=7)
    dataOfNow.grid(row=4,column=5)
    dataShow.grid(row=5,column=5)
    startButton.grid(row=6,column=3)
    endButton.grid(row=6,column=7)
    root.mainloop()
t1 = threading.Thread(target=guiDisplay)
t2 = threading.Thread(target=runStart)
# t3 = threading.Thread(target=Keylisten)
t1.start()
t2.start()
# t3.start()
