#! /bin/sh
#��򵥵Ļ���FFmpeg���ڴ��д���ӣ��ڴ沥������ ----MinGW�����б���
#Simplest FFmpeg mem Player ----Compile in MinGW 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_mem_player.cpp -g -o simplest_ffmpeg_mem_player.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale
