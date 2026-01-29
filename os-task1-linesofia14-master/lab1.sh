#!/usr/bin/env bash

# edit the code below and add your code
# отредактируйте код ниже и добавьте свой

# Переменная с номером варианта (константа):
# Объявляем TASKID (Вариант 3)
TASKID=3

# Дополнительные переменные (должны вычисляться динамически):
# Объявляем VAR_1 и вычисляем количество записей в dns-tunneling.log
VAR_1=$(wc -l < dns-tunneling.log)

# Преобразование данных в формат XML
head -n 20 dns-tunneling.log | awk '{
  timestamp = $4;
  client_ip = $5;
  client_port = $6;

   printf "<row>\n";
  printf "\t<timestamp>%s</timestamp>\n", timestamp;
  printf "\t<client_ip>%s</client_ip>\n", client_ip;
  printf "\t<client_port>%s</client_port>\n", client_port;
  printf "</row>\n";
}' | sed '1s/^/<dnslog>\n/g' | sed '$s/$/\n<\/dnslog>/g' > results.txt

# Подсчет записей с IP-адресами из подсети 10.1.*.*
VAR_2=$(grep "<client_ip>10\.1\." results.txt | wc -l)

echo "TASKID=$TASKID"
echo "VAR_1=$VAR_1"
echo "VAR_2=$VAR_2"
