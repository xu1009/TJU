# _*_ coding:utf-8 _*_
import web
import web.db
import sae.const

db = web.database(
    dbn='mysql',
    host=sae.const.MYSQL_HOST,
    port=int(sae.const.MYSQL_PORT),
    user=sae.const.MYSQL_USER,
    passwd=sae.const.MYSQL_PASS,
    db=sae.const.MYSQL_DB
)
 
def addfk(time, numOfRoom,pm25,pm10,tem,dam,co2):
    return db.update('fk1',where='id=$id',vars={'id':(int)(numOfRoom)},time=time, numOfRoom=numOfRoom, pm25=pm25,pm10=pm10,tem=tem,dam=dam,co2=co2)
 
def get_fkcontent():
    return db.select('fk1')