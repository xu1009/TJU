# -*- coding: utf-8 -*-
import hashlib
import web
import lxml
import time
import os
import urllib2,json
from lxml import etree
import connectData
class WeixinInterface:

    def __init__(self):
        self.app_root = os.path.dirname(__file__)
        self.templates_root = os.path.join(self.app_root, 'templates')
        self.render = web.template.render(self.templates_root)
        self.dts = ','
       
        
    def POST(self):        
        str_xml = web.data() #���post��������
        xml = etree.fromstring(str_xml)#����XML����
        content=xml.find("Content").text#����û������������
        msgType=xml.find("MsgType").text
        fromUser=xml.find("FromUserName").text
        toUser=xml.find("ToUserName").text
        if msgType=="event":
            if xml.find("Event").text=="subscribe":
                return self.render.reply_text(fromUser,toUser,int(time.time()),'sdassa') 
       # connectData.addfk('2012-12-3 13:13:00','a','b','c','d','e','f')   
        else:
            num = len(str(list(connectData.get_fkcontent())[0]))
            self.dts = u'1�ŷ���:\r\n'+(str(list(connectData.get_fkcontent())[0]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n2�ŷ���:\r\n'+(str(list(connectData.get_fkcontent())[1]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n3�ŷ���:\r\n'+(str(list(connectData.get_fkcontent())[2]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n4�ŷ���:\r\n'+(str(list(connectData.get_fkcontent())[3]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n5�ŷ���:\r\n'+(str(list(connectData.get_fkcontent())[4]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')
            return self.render.reply_text(fromUser,toUser,int(time.time()),self.dts)

    def GET(self):
        #��ȡ�������        
        data = web.input()       
        signature=data.signature
        timestamp=data.timestamp
        nonce=data.nonce
        echostr=data.echostr
        #�Լ���token
        token="zhanghao" #�����д����΢�Ź���ƽ̨�������token
        #�ֵ�������
        list=[token,timestamp,nonce]
        list.sort()
        sha1=hashlib.sha1()
        map(sha1.update,list)
        hashcode=sha1.hexdigest()
        #sha1�����㷨        

        #���������΢�ŵ�������ظ�echostr
        if hashcode == signature:
            return echostr