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
        str_xml = web.data() #获得post来的数据
        xml = etree.fromstring(str_xml)#进行XML解析
        content=xml.find("Content").text#获得用户所输入的内容
        msgType=xml.find("MsgType").text
        fromUser=xml.find("FromUserName").text
        toUser=xml.find("ToUserName").text
        if msgType=="event":
            if xml.find("Event").text=="subscribe":
                return self.render.reply_text(fromUser,toUser,int(time.time()),'sdassa') 
       # connectData.addfk('2012-12-3 13:13:00','a','b','c','d','e','f')   
        else:
            num = len(str(list(connectData.get_fkcontent())[0]))
            self.dts = u'1号房间:\r\n'+(str(list(connectData.get_fkcontent())[0]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n2号房间:\r\n'+(str(list(connectData.get_fkcontent())[1]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n3号房间:\r\n'+(str(list(connectData.get_fkcontent())[2]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n4号房间:\r\n'+(str(list(connectData.get_fkcontent())[3]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')+u'\r\n5号房间:\r\n'+(str(list(connectData.get_fkcontent())[4]))[10:(num-2):1].replace("\'"," ").replace('u','').replace(',','\r\n').replace('*','.')
            return self.render.reply_text(fromUser,toUser,int(time.time()),self.dts)

    def GET(self):
        #获取输入参数        
        data = web.input()       
        signature=data.signature
        timestamp=data.timestamp
        nonce=data.nonce
        echostr=data.echostr
        #自己的token
        token="zhanghao" #这里改写你在微信公众平台里输入的token
        #字典序排序
        list=[token,timestamp,nonce]
        list.sort()
        sha1=hashlib.sha1()
        map(sha1.update,list)
        hashcode=sha1.hexdigest()
        #sha1加密算法        

        #如果是来自微信的请求，则回复echostr
        if hashcode == signature:
            return echostr