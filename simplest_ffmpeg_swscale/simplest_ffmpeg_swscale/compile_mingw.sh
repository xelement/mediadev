#! /bin/sh
#��򵥵Ļ���FFmpeg��Swscaleʾ�� ----MinGW�����б���
#Simplest FFmpeg Swscale ----Compile in MinGW 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_swscale.cpp -g -o simplest_ffmpeg_swscale.exe \
-I /usr/local/include -L /usr/local/lib -lswscale -lavutil
