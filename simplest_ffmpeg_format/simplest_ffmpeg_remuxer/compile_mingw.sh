#! /bin/sh
#��򵥵Ļ���FFmpeg�ķ�װ��ʽת���� ----MinGW�����б���
#Simplest FFmpeg Remuxer ----Compile in MinGW 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_remuxer.cpp -g -o simplest_ffmpeg_remuxer.exe \
-I /usr/local/include -L /usr/local/lib -lavformat -lavcodec -lavutil
