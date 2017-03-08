# -*- coding: utf-8 -*-
import hashlib
import web
import lxml
import time
import os
import urllib2,json
from lxml import etree
import connectData
class SaveData:      
	def GET(self):
            data = web.input(pm25=[],pm10=[],tem=[],dam=[],co2=[],time=[])
            #data = web.input()
            #return (data.pm25+' '+data.pm10+' '+data.tem+' '+data.dam+' '+data.co2)
            #connectData.addfk('123:4','1','22','33','33','ee','ff')
           # connectData.addfk(data.time.encode('utf-8'),'1',data.pm25.encode('utf-8'),data.pm10.encode('utf-8'),data.tem.encode('utf-8'),data.dam.encode('utf-8'),data.co2.encode('utf-8'))
            connectData.addfk(data.time[0].encode('utf-8'),'1',data.pm25[0].encode('utf-8'),data.pm10[0].encode('utf-8'),data.tem[0].encode('utf-8'),data.dam[0].encode('utf-8'),data.co2[0].encode('utf-8'))
            connectData.addfk(data.time[0].encode('utf-8'),'2',data.pm25[1].encode('utf-8'),data.pm10[1].encode('utf-8'),data.tem[1].encode('utf-8'),data.dam[1].encode('utf-8'),data.co2[1].encode('utf-8'))
            connectData.addfk(data.time[0].encode('utf-8'),'3',data.pm25[2].encode('utf-8'),data.pm10[2].encode('utf-8'),data.tem[2].encode('utf-8'),data.dam[2].encode('utf-8'),data.co2[2].encode('utf-8'))
            connectData.addfk(data.time[0].encode('utf-8'),'4',data.pm25[3].encode('utf-8'),data.pm10[3].encode('utf-8'),data.tem[3].encode('utf-8'),data.dam[3].encode('utf-8'),data.co2[3].encode('utf-8'))
            connectData.addfk(data.time[0].encode('utf-8'),'5',data.pm25[4].encode('utf-8'),data.pm10[4].encode('utf-8'),data.tem[4].encode('utf-8'),data.dam[4].encode('utf-8'),data.co2[4].encode('utf-8'))
            #num = len(str(list(connectData.get_fkcontent())[0]))
            return (str(list(connectData.get_fkcontent())))
         