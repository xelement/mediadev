#! /bin/sh
#��򵥵Ļ���FFmpeg���ڴ��д���ӣ��ڴ沥������----�����б���
#Simplest FFmpeg mem Player----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_mem_player.cpp -g -o simplest_ffmpeg_mem_player.out -lstdc++ \
-framework Cocoa -I /usr/local/include -L /usr/local/lib -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lswscale

