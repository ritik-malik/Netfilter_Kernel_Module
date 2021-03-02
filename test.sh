#!/bin/bash

# This is the test script for the
# Netfilter Kernel Module in the VM2
#
# It uses NMAP for -
# * TCP-SYN scan
# * TCP-ACK scan
# * TCP-FIN scan
# * UPD scan
# and ICPM requests using ping
#
# It stores all the output to logs.txt file


echo "-----------------------------------"
echo "Sending TCP-SYN packet on port 8080"
nmap --append-output -oN logs.txt 50.0.0.4 -sS -p 8080 > /dev/null

echo "-----------------------------------"
echo "Sending TCP-ACK packet on port 2020"
nmap --append-output -oN logs.txt 50.0.0.4 -sA -p 2020 > /dev/null

echo "-----------------------------------"
echo "Sending TCP-FIN packet on port 5555"
nmap --append-output -oN logs.txt 50.0.0.4 -sF -p 5555 > /dev/null

echo "-----------------------------------"
echo "Sending UDP packet on port 443"
nmap --append-output -oN logs.txt 50.0.0.4 -sU -p 443 > /dev/null

echo "-----------------------------------"
echo "Sending UDP packet on port 53"
nmap --append-output -oN logs.txt 50.0.0.4 -sU -p 53 > /dev/null

echo "-----------------------------------"
echo "Sending 2 ICMP packets"
ping -w 2 50.0.0.4 >> logs.txt

echo "-----------------------------------"
echo "--------All scans completed--------"
echo "-----------------------------------"

echo "Check logs.txt for outputs"
echo 



