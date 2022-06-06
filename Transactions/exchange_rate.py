import requests
from tkinter import *
import tkinter as tk
from tkinter import ttk

url = 'https://api.exchangerate-api.com/v4/latest/USD'
iso_list=open('C:/Users/USER/Desktop/currenciesISOnNames.txt')
exchange=open('C:/Users/USER/Desktop/exchange_rate.txt','w')
data= requests.get(url).json()
currencies = data['rates']
print (currencies['USD'])
line=iso_list.readline()

while line:
    rate=round(1/currencies[line.split(':')[0]],7)
    exchange.write(line.split(':')[0] + ' ' + str(rate)+'\n')
    line=iso_list.readline()
iso_list.close()
exchange.close()
    